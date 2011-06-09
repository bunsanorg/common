#include "helpers.hpp"

#include <cstring>

#include "util.hpp"

template <typename Ret>
std::string tstrerror(int err, char *buf, size_t buflen)
{
	return __FILE__ ": error function implementation";
}

template <>
std::string tstrerror<int>(int err, char *buf, size_t buflen)
{
#warning possible infinite recursion
	SLOG("errno="<<err);
	if (strerror_r(err, buf, buflen))
	{
		return bunsan::impl::strerror(errno);
	}
	else
	{
		return buf;
	}
}

template <>
std::string tstrerror<char *>(int err, char *buf, size_t buflen)
{
	SLOG("errno="<<err);
	return strerror_r(err, buf, buflen);
}

std::string bunsan::impl::strerror(int err)
{
	static const size_t buflen = 256;
#warning buffer size
	char msg[buflen+1];
	msg[buflen] = '\0';
	return tstrerror<decltype(strerror_r(err, msg, buflen))>(err, msg, buflen);
}

