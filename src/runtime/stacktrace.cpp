#define _GNU_SOURCE

#include "bunsan/runtime/stacktrace.hpp"
#include "bunsan/runtime/demangle.hpp"

#include <iomanip>

#include <cstdlib>
#include <cstring>

#include <boost/assert.hpp>

#include <execinfo.h>
#include <dlfcn.h>

namespace bunsan{namespace runtime
{
// FIXME windows version is not implemented
    stacktrace stacktrace::get(const std::size_t max_size, const std::size_t skip)
    {
        stacktrace trace;
        trace.resize(max_size + skip);
        const std::size_t real_size = ::backtrace(trace.data(), max_size + skip);
        BOOST_ASSERT(real_size <= max_size + skip);
        trace.resize(real_size);
        trace.erase(trace.begin(), trace.begin() + skip);
        return trace;
    }

    std::ostream &operator<<(std::ostream &out, const stacktrace &trace)
    {
        for (void *const function: trace)
        {
            Dl_info info;
            if (dladdr(function, &info))
            {
                const std::size_t offset = static_cast<char *>(function) - static_cast<char *>(info.dli_saddr);
                out << info.dli_fname << '(';
                if (info.dli_sname)
                    out << demangle(info.dli_sname);
                else
                    out << "???";
                if (offset)
                    out << "+0x" << std::hex << offset;
                out << ") [";
                if (info.dli_saddr)
                    out << info.dli_saddr;
                else
                    out << "???";
                out << "] {addr = 0x" << std::hex << function << "}\n";
            }
            else
            {
                out << "0x" << std::hex << function << "\n";
            }
        }
        return out;
    }
}}
