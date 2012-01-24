#include "bunsan/error.hpp"

bunsan::error::error(const std::string &message)
{
	(*this)<<error_message(message);
}

const std::string &bunsan::error::message() const throw()
{
	return *boost::get_error_info<error_message>(*this);
}

const char *bunsan::error::what() const throw()
{
	return boost::diagnostic_information_what(*this);
}

