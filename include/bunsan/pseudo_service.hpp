#ifndef BUNSAN_PSEUDO_SERVICE_HPP
#define BUNSAN_PSEUDO_SERVICE_HPP

#include <mutex>
#include <condition_variable>

#include "bunsan/service.hpp"

namespace bunsan
{
	class pseudo_service: virtual public service
	{
	public:
		pseudo_service();
		virtual void start();
		virtual void join();
		virtual void stop();
		virtual bool is_running();
		virtual ~pseudo_service();
	private:
		typedef std::unique_lock<std::mutex> guard;
		std::mutex run_lock;
		bool running;
		std::condition_variable joiner;
	};
}

#endif //BUNSAN_PSEUDO_SERVICE_HPP

