#ifndef COMPLEX_SERVICE_HPP
#define COMPLEX_SERVICE_HPP

#include <vector>

#include "service.hpp"

namespace bunsan
{
	class complex_service: virtual public service
	{
	public:
		virtual void start();
		virtual void join();
		virtual void stop();
		virtual bool is_running();
		virtual ~complex_service();
	protected:
		typedef std::vector<service_ptr> container;
		container services;
	};
}

#endif //COMPLEX_SERVICE_HPP

