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
	std::string s(1000000, '\0');
	std::generate(s.begin(), s.end(), [](){return static_cast<char>(rand()%256);});
	test(inp, out, s);
}

