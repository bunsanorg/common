#include "bunsan/zmq.hpp"

#include <stdexcept>

bunsan::zmq::socket_t::socket_t(zmq::context_t &context, int type): ::zmq::socket_t(context, type) {}

void bunsan::zmq::socket_t::set_linger(int linger_)
{
	setsockopt(ZMQ_LINGER, linger_);
}

bunsan::zmq::socket_t::~socket_t()
{
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

bool bunsan::zmq::socket_t::send(const std::vector<std::string> &msg, int opt)
{
	size_t size = 0;
	for (const std::string &i: msg)
		size += i.size()+1;
	zmq::message_t msg_(size);
	char *buf = static_cast<char *>(msg_.data());
	for (const std::string &i: msg)
	{
		if (i.find('\0')!=std::string::npos)
			throw std::invalid_argument("string in std::vector<std::string> may not contain \\0 symbol");
		memcpy(buf, i.c_str(), i.size());
		buf += i.size();
		*buf = '\0';
		++buf;
	}
	return send(msg_, opt);
}

bool bunsan::zmq::socket_t::recv(zmq::message_t &msg, int opt)
{
	return recv(&msg, opt);
}

bool bunsan::zmq::socket_t::recv(std::string &msg, int opt)
{
	zmq::message_t msg_;
	bool ret = recv(msg_, opt);
	if (ret)
	{
		msg = std::string(static_cast<char *>(msg_.data()), msg_.size());
	}
	return ret;
}

bool bunsan::zmq::socket_t::recv(std::vector<unsigned char> &msg, int opt)
{
	zmq::message_t msg_;
	bool ret = recv(msg_, opt);
	if (ret)
	{
		msg.resize(msg_.size());
		if (!msg.empty())
			memcpy(&msg[0], msg_.data(), msg_.size());
	}
	return ret;
}

bool bunsan::zmq::socket_t::recv(std::vector<std::string> &msg, int opt)
{
	zmq::message_t msg_;
	bool ret = recv(msg_, opt);
	if (ret)
	{
		char *buf = static_cast<char *>(msg_.data()), *ebuf = static_cast<char *>(msg_.data())+msg_.size();
		size_t size = std::count(buf, ebuf, '\0');
		msg.resize(size);
		for (size_t i = 0; i<size; ++i)
		{
			char *nbuf = std::find(buf, ebuf, '\0');
			msg[i].resize(nbuf-buf);
			std::copy(buf, nbuf, msg[i].begin());
			buf = nbuf+1;
		}
	}
	return ret;
}

