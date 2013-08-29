#pragma once

#include <bunsan/error.hpp>
#include <bunsan/runtime/demangle.hpp>
#include <bunsan/system_error.hpp>

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>
#include <boost/system/system_error.hpp>

#include <ios>
#include <stdexcept>
#include <system_error>
#include <type_traits>

namespace bunsan
{
    namespace exception_detail
    {
        typedef boost::error_info<struct tag_original_what, std::string> original_what;
        typedef boost::error_info<struct tag_original_type_name, std::string> original_type_name;

        /// \todo use std::exception_ptr
        template <typename T>
        struct error_info_injector: T, boost::exception
        {
            explicit error_info_injector(const T &e): T(e)
            {
                (*this) << error::stacktrace(runtime::stacktrace::get(2));
                // note: we use original value from e
                (*this) << original_what(e.what()) << original_type_name(runtime::type_name(e));
            }

            const char *what() const noexcept override
            {
                return boost::diagnostic_information_what(*this);
            }
        };
    }

    template <typename T>
    typename std::enable_if<!std::is_base_of<boost::exception, T>::value,
    exception_detail::error_info_injector<T>>::type enable_error_info(const T &obj)
    {
        return exception_detail::error_info_injector<T>(obj);
    }

/*!
 * \brief Begin exceptions wrapping statement
 *
 * These macros add stack trace and other useful info
 * to thrown exception. If exception is derived from boost::exception
 * it's type is preserved. Otherwise predefined list of exceptions from <stdexcept>,
 * <system_error> and <boost/system/system_error.hpp> is used to catch and wrap.
 * Note, that if exception is derived from one of that types and not explicitly
 * specified (via BUNSAN_EXCEPTIONS_WRAP_END_EXCEPT or BUNSAN_EXCEPTIONS_WRAP_END_ERROR_INFO_EXCEPT),
 * some information may be lost.
 *
 * \code{.cpp}
 * BUNSAN_EXCEPTIONS_WRAP_BEGIN()
 * {
 *     throw std::runtime_error("hello, world");
 * }
 * BUNSAN_EXCEPTIONS_WRAP_END()
 * \endcode
 */
#define BUNSAN_EXCEPTIONS_WRAP_BEGIN() \
    try

#define BUNSAN_EXCEPTIONS_WRAP_WRAPPER(r, ERROR_INFO, ERROR) \
    catch (ERROR &e) \
    { \
        BOOST_THROW_EXCEPTION(::bunsan::enable_error_info(e) \
            ERROR_INFO); \
    }

#define BUNSAN_EXCEPTIONS_WRAP_END_(ERROR_INFO, ...) \
    catch (::boost::exception &e) \
    { \
        if (!::boost::get_error_info< ::bunsan::error::stacktrace>(e)) \
            e << ::bunsan::error::stacktrace(::bunsan::runtime::stacktrace::get()); \
        if (!::boost::get_error_info< ::boost::throw_function>(e)) \
            e << ::boost::throw_function(BOOST_CURRENT_FUNCTION); \
        if (!::boost::get_error_info< ::boost::throw_file>(e)) \
            e << ::boost::throw_file(__FILE__); \
        if (!::boost::get_error_info< ::boost::throw_line>(e)) \
            e << ::boost::throw_line(__LINE__); \
        (void) (e ERROR_INFO); \
        throw; \
    } \
    catch (::std::ios_base::failure &e) \
    /* TODO: should be removed when std::ios_base::failure is derived from std::system_error */ \
    { \
        BOOST_THROW_EXCEPTION(::bunsan::system_error(e.what()) << \
            ::bunsan::exception_detail::original_type_name(::bunsan::runtime::type_name(e)) \
            ERROR_INFO); \
    } \
    catch (::std::bad_alloc &) \
    /* should not be caught, there are no enough memory */ \
    { \
        throw; \
    } \
    BOOST_PP_SEQ_FOR_EACH(BUNSAN_EXCEPTIONS_WRAP_WRAPPER, ERROR_INFO, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))

/// Predefined list of exceptions.
#define BUNSAN_EXCEPTIONS_WRAP_LIST \
        ::std::domain_error, ::std::invalid_argument, ::std::length_error, \
            ::std::out_of_range, ::std::logic_error, \
        ::std::range_error, ::std::overflow_error, ::std::underflow_error, \
            ::std::system_error, ::boost::system::system_error, ::std::runtime_error, \
        ::std::exception

/// Default footer.
#define BUNSAN_EXCEPTIONS_WRAP_END() \
    BUNSAN_EXCEPTIONS_WRAP_END_(, BUNSAN_EXCEPTIONS_WRAP_LIST)

/// Catch exceptions specified as arguments before default list.
#define BUNSAN_EXCEPTIONS_WRAP_END_EXCEPT(...) \
    BUNSAN_EXCEPTIONS_WRAP_END_(, __VA_ARGS__, BUNSAN_EXCEPTIONS_WRAP_LIST)

/*!
 * \brief Add error info to wrapped exception.
 *
 * \param ERROR_INFO boost::error_info instance (streaming expression)
 *
 * \code{.cpp}
 * BUNSAN_EXCEPTIONS_WRAP_END_ERROR_INFO_EXCEPT(bunsan::filesystem::error::source_path(path1) <<
 *     bunsan::filesystem::error::destination_path(path2))
 * \endcode
 */
#define BUNSAN_EXCEPTIONS_WRAP_END_ERROR_INFO(ERROR_INFO) \
    BUNSAN_EXCEPTIONS_WRAP_END_(<< ERROR_INFO, BUNSAN_EXCEPTIONS_WRAP_LIST)

/// Add error info to wrapped exception + catch additional exceptions.
#define BUNSAN_EXCEPTIONS_WRAP_END_ERROR_INFO_EXCEPT(ERROR_INFO, ...) \
    BUNSAN_EXCEPTIONS_WRAP_END_(<< ERROR_INFO, __VA_ARGS__, BUNSAN_EXCEPTIONS_WRAP_LIST)
}
