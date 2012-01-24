#ifndef BUNSAN_ERROR_HPP
#define BUNSAN_ERROR_HPP

#include <exception>

#include <boost/exception/all.hpp>

namespace bunsan
{
	namespace error_detail
	{
		typedef boost::error_info<struct error_message_tag, std::string> error_message;
		struct error: virtual std::exception, virtual boost::exception
		{
			error()=default;
			error(const std::string &message);
			virtual const std::string &message() const throw();
			virtual const char *what() const throw();
		};
	}
	using error_detail::error;
}

#endif //BUNSAN_ERROR_HPP

