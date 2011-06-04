#ifndef PROCESS_IMPL_HPP
#define PROCESS_IMPL_HPP

#include <boost/noncopyable.hpp>

#include <sys/types.h>

#include "process.hpp"

namespace bunsan
{
	class process_impl: public process, private boost::noncopyable
	{
	public:
		process_impl(pid_t pid_);
		virtual void terminate();
		virtual void kill();
		virtual void wait();
		virtual bool completed();
		virtual int return_code();
		virtual ~process_impl() throw();
	private:
		pid_t pid;
		int stat;
		bool completed_;
	};
}

#endif //PROCESS_IMPL_HPP

