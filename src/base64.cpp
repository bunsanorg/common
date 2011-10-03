#include "bunsan/base64.hpp"

#include <xmlrpc-c/base64.hpp>

std::string bunsan::base64::encode(const std::vector<unsigned char> &data)
{
	return xmlrpc_c::base64FromBytes(data);
}

std::vector<unsigned char> bunsan::base64::decode(const std::string &base64)
{
	return xmlrpc_c::bytesFromBase64(base64);
}

