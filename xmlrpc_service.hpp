#ifndef XMLRPC_SERVICE_HPP
#define XMLRPC_SERVICE_HPP

#include <thread>
#include <mutex>

#include <xmlrpc-c/server_abyss.hpp>
#include <xmlrpc-c/registry.hpp>
#include <xmlrpc-c/girerr.hpp>

#include "service.hpp"

namespace bunsan
{
	class xmlrpc_service: virtual public bunsan::service
	{
	public:
		virtual void start();
		virtual void join();
		virtual void stop();
		virtual bool is_running();
		virtual ~xmlrpc_service();
	protected:
		typedef std::unique_lock<std::mutex> guard;
		typedef xmlrpc_c::registryPtr registry_ptr;
		typedef std::shared_ptr<xmlrpc_c::serverAbyss> server_ptr;
		virtual void create_server()=0;
		server_ptr server;
		registry_ptr registry;
		std::thread thread;
		std::mutex lock;
	private:
		void run();
	};
}

#endif //XMLRPC_SERVICE_HPP

