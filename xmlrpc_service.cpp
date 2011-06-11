#include "xmlrpc_service.hpp"

#include "util.hpp"

void bunsan::xmlrpc_service::run()
{
	server_ptr server_;
	{
		guard lk(slock);
		create_server();
		server_ = server;
	}
	server_->run();
	{
		guard lk(slock);
		server.reset();
	}
}

void bunsan::xmlrpc_service::start()
{
	server_ptr server_;
	{
		guard lk(slock);
		server_ = server;
	}
	if (server_)
	{
		server_->terminate();
		{
			guard lk(tlock);
			if (thread.joinable())
				thread.join();
		}
	}
	{
		guard lk(tlock);
		thread = std::thread(&bunsan::xmlrpc_service::run, this);
	}
}

void bunsan::xmlrpc_service::join()
{
	guard lk(tlock);
	if (thread.joinable())
		thread.join();
}

void bunsan::xmlrpc_service::stop()
{
	DLOG(trying to stop);
	server_ptr server_;
	{
		guard lk(slock);
		server_ = server;
	}
	if (server_)
		server_->terminate();
}

bool bunsan::xmlrpc_service::is_running()
{
	guard lk(slock);
	return static_cast<bool>(server);
}

bunsan::xmlrpc_service::~xmlrpc_service()
{
	DLOG(trying to destroy);
	stop();
	join();
}

