#if defined(__linux) || defined(__APPLE__) || defined(__sun)

#   ifndef _GNU_SOURCE
#       define _GNU_SOURCE
#   endif

#   define BOOST_HAVE_EXECINFO
#   define BOOST_HAVE_DLADDR
#endif

#include <bunsan/runtime/demangle.hpp>
#include <bunsan/runtime/stacktrace.hpp>

#include <boost/assert.hpp>
#include <boost/optional.hpp>
#include <boost/scope_exit.hpp>

#include <iomanip>

#include <cstdlib>
#include <cstring>

#ifdef BOOST_HAVE_EXECINFO
#   include <execinfo.h>
#endif

#ifdef BOOST_HAVE_DLADDR
#   include <dlfcn.h>
#endif

#ifdef BOOST_WINDOWS
#   include <windows.h>
#   include <psapi.h>
#endif

#ifdef BOOST_MSVC
#   include <dbghelp.h>
#endif

namespace bunsan{namespace runtime
{
    stacktrace stacktrace::get(const std::size_t skip_, const std::size_t max_size)
    {
        const std::size_t skip = skip_ + 1; // we should skip stacktrace::get()
        stacktrace trace;
#if defined(BOOST_HAVE_EXECINFO)
        trace.resize(max_size + skip);
        const std::size_t real_size = ::backtrace(trace.data(), max_size + skip);
        BOOST_ASSERT(real_size <= max_size + skip);
        trace.resize(real_size);
        if (skip <= real_size)
            trace.erase(trace.begin(), trace.begin() + skip);
        else
            trace.clear();
#elif defined(BOOST_WINDOWS)
        typedef USHORT (WINAPI *capture_func_type)(ULONG, ULONG, PVOID *, PULONG);
#   if defined(BOOST_MSVC)
        const static capture_func_type capture_stack_trace = &RtlCaptureStackBackTrace;
#   else
        const static capture_func_type capture_stack_trace =
            []()
            {
                capture_func_type capture_stack_trace = nullptr;
                const HMODULE h = GetModuleHandle("kernel32.dll");
                if (h)
                    capture_stack_trace = (capture_func_type)GetProcAddress(h, "RtlCaptureStackBackTrace");
                return capture_stack_trace;
            }();
#   endif
        constexpr std::size_t UNWIND_LIMIT = 62; // see msdn
        if (capture_stack_trace && skip < UNWIND_LIMIT)
        {
            const std::size_t size = std::min(max_size, UNWIND_LIMIT - skip);
            trace.resize(size);
            const std::size_t real_size = capture_stack_trace(skip, size, trace.data(), 0);
            trace.resize(real_size);
        }
#endif
        return trace;
    }

    namespace
    {
        void format_fallback(std::ostream &out, void *const addr)
        {
            out << std::showbase << std::hex << addr << '\n';
        }

        void format_all_fallback(std::ostream &out, const stacktrace &trace)
        {
            for (void *const function: trace)
                format_fallback(out, function);
        }

        void format(std::ostream &out,
                    const char *const fname,
                    const void *const fbase,
                    std::ptrdiff_t module_offset,
                    const char *const sname,
                    const void *const saddr,
                    std::ptrdiff_t offset,
                    const void *const addr)
        {
            if (!offset && saddr)
                offset = static_cast<const char *>(addr) - static_cast<const char *>(saddr);
            if (fname || fbase)
            {
                if (fname)
                    out << fname;
                else
                    out << "module";
                if (fbase)
                    out << " at " << std::showbase << std::hex << fbase;
                if (!module_offset && fbase)
                    module_offset = static_cast<const char *>(addr) - static_cast<const char *>(fbase);
                if (module_offset)
                    out << "+" << std::showbase << std::hex << module_offset;
            }
            out << '(';
            if (sname)
                out << demangle(sname);
            else
                out << "???";
            if (offset)
                out << "+" << std::showbase << std::hex << offset;
            out << ") [" << std::showbase << std::hex << addr << ']';
            if (saddr)
                out << " {saddr = " << std::showbase << std::hex << saddr << "}\n";
        }
    }

    std::ostream &operator<<(std::ostream &out, const stacktrace &trace)
    {
#if defined(BOOST_HAVE_DLADDR)
        for (void *const function: trace)
        {
            Dl_info info;
            if (dladdr(function, &info))
                format(out, info.dli_fname, info.dli_fbase, 0, info.dli_sname, info.dli_saddr, 0, function);
            else
                format_fallback(out, function);
        }
#elif defined(BOOST_HAVE_EXECINFO)
        char **strings = nullptr;
        BOOST_SCOPE_EXIT_ALL(strings) { free(strings); };
        strings = backtrace_symbols(trace.data(), trace.size());
        if (strings)
        {
            // TODO it is possible to parse strings[i] and get slightly better result
            for (std::size_t i = 0; i < trace.size(); ++i)
                out << strings[i] << '\n';
        }
        else
        {
            format_all_fallback(out, trace);
        }
#elif defined(BOOST_WINDOWS)
#   if defined(BOOST_MSVC)
        struct msvc_context
        {
            HANDLE hProcess = 0;
            bool syms_ready = false;
        };
        static const msvc_context context =
            []()
            {
                SymSetOptions(SYMOPT_DEFERRED_LOADS);
                msvc_context context = {
                    .hProcess = GetCurrentProcess(),
                    .syms_ready = SymInitialize(hProcess, nullptr, true)
                };
                return context;
            }();
        for (void *const function: trace)
        {
            if (context.syms_ready)
            {
                DWORD64 dwDisplacement = 0;
                DWORD64 dwAddress = reinterpret_cast<DWORD64>(function);
                std::vector<char> buffer(sizeof(SYMBOL_INFO) + MAX_SYM_NAME);
                PSYMBOL_INFO pSymbol = reinterpret_cast<PSYMBOL_INFO>(buffer.data());
                pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
                pSymbol->MaxNameLen = MAX_SYM_NAME;
                if (SymFromAddr(context.hProcess, dwAddress, &dwDisplacement, pSymbol))
                    format(out, nullptr, nullptr, 0, pSymbol->Name, pSymbol->Address, dwDisplacement, function);
                else
                    format_fallback(out, function);
            }
            else
            {
                format_fallback(out, function);
            }
        }
#   else
        const HANDLE hProc = GetCurrentProcess();
        std::vector<HMODULE> modules(1024);
        DWORD size;
        bool has_module_list = false;
        do
        {
            if (has_module_list)
                modules.resize(modules.size() * 2);
            has_module_list = EnumProcessModules(hProc, modules.data(), sizeof(HMODULE) * modules.size(), &size);
            size /= sizeof(HMODULE);
        }
        while (has_module_list && size > modules.size());
        if (has_module_list)
        {
            std::vector<boost::optional<MODULEINFO>> module_info(size);
            std::vector<std::string> names(size);

            for (std::size_t i = 0; i < size; ++i)
            {
                MODULEINFO info;
                if (GetModuleInformation(hProc, modules[i], &info, sizeof(MODULEINFO)))
                    module_info[i] = info;
            }

            for (void *const function: trace)
            {
                std::string fname;
                void *fbase = nullptr;
                std::ptrdiff_t module_offset = 0;
                bool found = false;
                for (std::size_t i = 0; !found && i < size; ++i)
                {
                    if (module_info[i])
                    {
                        char *const addr = static_cast<char *>(function);
                        char *const maddr = static_cast<char *>(module_info[i]->lpBaseOfDll);
                        if (maddr <= addr && addr < maddr + module_info[i]->SizeOfImage)
                        {
                            fbase = maddr;
                            module_offset = addr - maddr;
                            if (names[i].empty())
                            {
                                char module_name[MAX_PATH];
                                const DWORD length = GetModuleBaseNameA(hProc, modules[i], module_name, sizeof(module_name));
                                if (length)
                                    names[i].assign(module_name, length);
                                else
                                    names[i] = "???";
                            }
                            fname = names[i];
                            found = true;
                        }
                    }
                }
                if (found)
                    format(out, fname.c_str(), fbase, module_offset, nullptr, nullptr, 0, function);
                else
                    format_fallback(out, function);
            }
        }
        else
        {
            format_all_fallback(out, trace);
        }
#   endif
#else
        format_all_fallback(out, trace);
#endif
        return out;
    }
}}
