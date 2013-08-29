#pragma once

#include <bunsan/runtime/stacktrace.hpp>

#include <boost/exception/all.hpp>

#include <exception>

namespace bunsan
{
    struct error: virtual std::exception, virtual boost::exception
    {
        /*!
         * \brief Create error object with stacktrace tag.
         *
         * \see stacktrace
         */
        error();

        /*!
         * \brief Create error object with message and stacktrace tags.
         *
         * \see message
         * \see stacktrace
         */
        error(const std::string &message_);

        const char *what() const noexcept override;

        /*!
         * \tparam ErrorInfo boost::error_info instantiation
         * \return pointer to data associated with ErrorInfo
         */
        template <typename ErrorInfo>
        const typename ErrorInfo::value_type *get() const noexcept
        {
            return boost::get_error_info<ErrorInfo>(*this);
        }

        /*!
         * \copydoc get
         */
        template <typename ErrorInfo>
        typename ErrorInfo::value_type *get() noexcept
        {
            return boost::get_error_info<ErrorInfo>(*this);
        }

        /*!
         * \brief Enable stack trace if not provided.
         *
         * \param skip number of nested function to omit in stack trace
         */
        error &enable_stacktrace(const std::size_t skip=0);

        /// Human readable error message
        typedef boost::error_info<struct tag_message, std::string> message;

        typedef boost::error_info<struct tag_stacktrace, runtime::stacktrace> stacktrace;
    };
}
