#include "xmlrpc_service.hpp"

#include "util.hpp"

void bunsan::xmlrpc_service::run()
{
	server_ptr server_;
	{
		guard lk(lock);
		create_server();
		server_ = server;
	}
	server_->run();
	{
		guard lk(lock);
		server.reset();
	}
}

void bunsan::xmlrpc_service::start()
{
	guard lk(lock);
	if (!server)
	{
		thread = std::thread(&bunsan::xmlrpc_service::run, this);
	}
}

void bunsan::xmlrpc_service::join()
{
	thread.join();
}

void bunsan::xmlrpc_service::stop()
{
	guard lk(lock);
	DLOG(trying to stop);
	if (server)
		server->terminate();
}

bool bunsan::xmlrpc_service::is_running()
{
	guard lk(lock);
	return static_cast<bool>(server);
}

bunsan::xmlrpc_service::~xmlrpc_service()
{
	guard lk(lock);
	DLOG(trying to destroy);
	if (server)
	{
		DLOG(terminating);
		server->terminate();
		thread.join();
	}
}

