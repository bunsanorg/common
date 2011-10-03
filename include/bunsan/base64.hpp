#ifndef BUNSAN_BASE64_HPP
#define BUNSAN_BASE64_HPP

#include <string>
#include <vector>

namespace bunsan{namespace base64
{
	std::string encode(const std::vector<unsigned char> &data);
	std::vector<unsigned char> decode(const std::string &base64);
}}

#endif //BUNSAN_BASE64_HPP

