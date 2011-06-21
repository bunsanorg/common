#include "bunsan/zmq_helpers.hpp"

int main()
{
	using namespace bunsan::zmq_helpers;
	zmq::message_t message;
	std::string s;
	std::vector<std::string> ss;
	std::string msg = "some strange text\nwith\nstrange\t\t\tsymbols\n\n";
	encode(msg, message);
	decode(message, s);
	assert(msg==s);
	std::vector<std::string> msgs0 = {"msg1\n\n", "msg2", "dcjnjvcs\n\n\ndmklc", "o_O", "\n\n\nakmdad", "", "the end!!!111"};
	encode(msgs0, message);
	decode(message, ss);
	assert(msgs0==ss);
	std::vector<std::string> msgs1 = {"", "", "o_O", ""};
	encode(msgs1, message);
	decode(message, ss);
	assert(msgs1==ss);
	return 0;
}

