#include "bunsan/error.hpp"

bunsan::error::error(const std::string &message_)
{
	(*this)<<error_message(message_);
}

const char *bunsan::error::message() const throw()
{
	const std::string *message_ = boost::get_error_info<error_message>(*this);
	return message_?message_->c_str():"no message";
}

const char *bunsan::error::what() const throw()
{
	return boost::diagnostic_information_what(*this);
}

