#ifndef BUNSAN_ZMQ_HELPERS_HPP
#define BUNSAN_ZMQ_HELPERS_HPP

#include <string>
#include <vector>
#include <algorithm>

#include <boost/optional.hpp>

#include <zmq.hpp>

namespace bunsan{namespace zmq_helpers
{
	/// autoclosing socket
	class socket: public zmq::socket_t
	{
		int linger;
	public:
		socket(zmq::context_t &context, int type);
		void set_linger(int linger_);
		using zmq::socket_t::setsockopt;
		using zmq::socket_t::getsockopt;
		template <typename T>
		inline void setsockopt(int option, const T *value)
		{
			zmq::socket_t::setsockopt(option, value, sizeof(T));
		}
		template <typename T>
		inline void setsockopt(int option, const T &value)
		{
			zmq::socket_t::setsockopt(option, &value, sizeof(T));
		}
		template <typename T>
		inline void getsockopt(int option, T &value)
		{
			getsockopt(option, &value);
		}
		template <typename T>
		inline void getsockopt(int option, T *value)
		{
			size_t len = sizeof(T);
			zmq::socket_t::getsockopt(option, value, &len);
		}
		~socket();
	};
	typedef socket socket_t;
	// generic
	template <typename T>
	void encode(const T &c, zmq::message_t &msg);
	template <typename T>
	void decode(/*const */zmq::message_t &msg, T &c);
	template <typename T>
	bool send(const T &c, zmq::socket_t &sock, int opt=0)
	{
		zmq::message_t msg;
		encode(c, msg);
		return sock.send(msg, opt);
	}
	inline bool send(const char *c, zmq::socket_t &sock, int opt=0)
	{
		return send(std::string(c), sock, opt);
	}
	template <typename T>
	bool recv(zmq::socket_t &sock, T &c, int opt=0)
	{
		zmq::message_t msg;
		if (sock.recv(&msg, opt))
		{
			decode(msg, c);
			return true;
		}
		else
			return false;
	}
	template <typename T, typename I>
	bool recv_more(zmq::socket_t &sock, T &c, I &more)
	{
		if (more)
		{
			zmq::message_t msg;
			sock.recv(&msg, ZMQ_RCVMORE);
			decode(msg, c);
			size_t more_size = sizeof(more);
			sock.getsockopt(ZMQ_RCVMORE, &more, &more_size);
		}
		return more;
	}
	template <typename T, typename I>
	bool recv_more(zmq::socket_t &sock, boost::optional<T> &c, I &more)
	{
		if (more)
		{
			T c_;
			recv_more(sock, c_, more);
			c = c_;
		}
		return more;
	}
	// string
	template <>
	void encode(const std::string &c, zmq::message_t &msg);
	template <>
	void decode(/*const */zmq::message_t &msg, std::string &c);
	// strings
	template <>
	void encode(const std::vector<std::string> &c, zmq::message_t &msg);
	template <>
	void decode(/*const */zmq::message_t &msg, std::vector<std::string> &c);
	// binary
	template <>
	void encode(const std::vector<unsigned char> &c, zmq::message_t &msg);
	template <>
	void decode(/*const */zmq::message_t &msg, std::vector<unsigned char> &c);
}}

#endif //BUNSAN_ZMQ_HELPERS_HPP

