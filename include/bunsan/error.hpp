#ifndef BUNSAN_ERROR_HPP
#define BUNSAN_ERROR_HPP

#include <exception>

#include <boost/exception/all.hpp>

namespace bunsan
{
	struct error: virtual std::exception, virtual boost::exception
	{
		error()=default;
		error(const std::string &message_);
		virtual const char *message() const throw();
		virtual const char *what() const throw();
	private:
		typedef boost::error_info<struct error_message_tag, std::string> error_message;
	};
}

#endif //BUNSAN_ERROR_HPP

