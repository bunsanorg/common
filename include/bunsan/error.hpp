#ifndef BUNSAN_ERROR_HPP
#define BUNSAN_ERROR_HPP

#include <exception>

#include <boost/exception/all.hpp>

namespace bunsan
{
    struct error: virtual std::exception, virtual boost::exception
    {
        error()=default;
        /// \see message
        error(const std::string &message_);
        virtual const char *what() const noexcept;
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
        // tags
        /// Human readable error message
        typedef boost::error_info<struct tag_message, std::string> message;
    };
}

#endif //BUNSAN_ERROR_HPP

