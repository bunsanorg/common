#ifndef BUNSAN_XMLRPC_SERVICE_HPP
#define BUNSAN_XMLRPC_SERVICE_HPP

#include <thread>
#include <mutex>

#include <xmlrpc-c/server_abyss.hpp>
#include <xmlrpc-c/registry.hpp>
#include <xmlrpc-c/girerr.hpp>

#include "bunsan/service.hpp"

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
    private:
        std::thread thread;
        std::mutex slock, tlock;
        void run();
    };
}

#endif //BUNSAN_XMLRPC_SERVICE_HPP

