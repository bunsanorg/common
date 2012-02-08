#include "bunsan/util.hpp"
#include "bunsan/zmq.hpp"

#include <algorithm>

#include <cstdlib>
#include <cassert>

namespace bzmq = bunsan::zmq;

template <typename T>
void test(bzmq::socket_t &inp, bzmq::socket_t &out, const T &msg)
{
	T rep;
	bool ret;
	ret = inp.send(msg);
	assert(ret);
	ret = out.recv(rep);
	assert(ret);
	assert(msg==rep);
}

void test(bzmq::socket_t &inp, bzmq::socket_t &out, const char *msg)
{
	test(inp, out, std::string(msg));
}

#define threads 0

int main()
{
	bzmq::context_t ctx(threads);
	bzmq::socket_t inp(ctx, ZMQ_PUSH);
	bzmq::socket_t out(ctx, ZMQ_PULL);
#if threads
	out.bind("tcp://*:12345");
	inp.connect("tcp://localhost:12345");
#else
	out.bind("inproc://a");
	inp.connect("inproc://a");
#endif
	test(inp, out, "");
	test(inp, out, "hello world");
	{
		std::string s(1000*1000, '\0');
		std::generate(s.begin(), s.end(), [](){return static_cast<char>(rand()%256);});
		test(inp, out, s);
	}
	{
		std::vector<std::string> msgs0 = {"msg1\n\n", "msg2", "dcjnjvcs\n\n\ndmklc", "o_O", "\n\n\nakmdad", "", "the end!!!111"};
		test(inp, out, msgs0);
	}
	{
		std::vector<std::string> msgs1 = {"", "", "o_O", ""};
		test(inp, out, msgs1);
	}
	{
		std::vector<std::string> msgs2;
		std::string buf;
		buf.push_back('1');
		buf.push_back('2');
		buf.push_back('3');
		buf.push_back('\0');
		buf.push_back('5');
		msgs2.push_back(buf);
		try
		{
			test(inp, out, msgs2);
			assert(false);
		}
		catch (std::exception &e)
		{
			SLOG("caught: "<<e.what());
		}
	}
	{
		std::vector<std::string> vec(1000);
		std::generate(vec.begin(), vec.end(),
			[]()
			{
				std::string s(10000, '\0');
				std::generate(s.begin(), s.end(),
					[]()
					{
						return static_cast<char>(rand()%255+1);
					});
				return s;
			});
		test(inp, out, vec);
	}
}

