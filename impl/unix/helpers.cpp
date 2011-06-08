#include "helpers.hpp"

#include <cstring>

std::string bunsan::impl::strerror(int err)
{
	static const size_t buflen = 200;
#warning buffer size
	char msg[buflen];
	strerror_r(err, msg, buflen);
	return msg;
}

