#include "bunsan/error.hpp"

bunsan::error::error(const std::string &message_)
{
	(*this)<<message(message_);
}

const char *bunsan::error::what() const throw()
{
	return boost::diagnostic_information_what(*this);
}

