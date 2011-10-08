#include "bunsan/zmq.hpp"

bunsan::zmq::socket_t::socket_t(zmq::context_t &context, int type): ::zmq::socket_t(context, type) {}

void bunsan::zmq::socket_t::set_linger(int linger_)
{
	linger = linger_;
}

bunsan::zmq::socket_t::~socket_t()
{
	if (linger)
		setsockopt(ZMQ_LINGER, &linger.get());
};

bool bunsan::zmq::socket_t::send(const std::string &msg, int opt)
{
	zmq::message_t msg_(msg.size());
	if (!msg.empty())
		memcpy(msg_.data(), msg.data(), msg.size());
	return send(msg_, opt);
}

bool bunsan::zmq::socket_t::send(const char *msg, int opt)
{
	size_t size = strlen(msg);
	zmq::message_t msg_(size);
	memcpy(msg_.data(), msg, size);
	return send(msg_, opt);
}

bool bunsan::zmq::socket_t::send(const std::vector<unsigned char> &msg, int opt)
{
	zmq::message_t msg_(msg.size());
	if (!msg.empty())
		memcpy(msg_.data(), &msg[0], msg.size());
	return send(msg_, opt);
}

bool bunsan::zmq::socket_t::recv(std::string &msg, int opt)
{
	zmq::message_t msg_;
	bool ret = recv(&msg_, opt);
	if (ret)
	{
		msg = std::string(static_cast<char *>(msg_.data()), msg_.size());
	}
	return ret;
}

bool bunsan::zmq::socket_t::recv(std::vector<unsigned char> &msg, int opt)
{
	zmq::message_t msg_;
	bool ret = recv(&msg_, opt);
	if (ret)
	{
		msg.resize(msg_.size());
		if (!msg.empty())
			memcpy(&msg[0], msg_.data(), msg_.size());
	}
	return ret;
}

