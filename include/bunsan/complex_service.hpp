#ifndef BUNSAN_COMPLEX_SERVICE_HPP
#define BUNSAN_COMPLEX_SERVICE_HPP

#include <vector>

#include "bunsan/service.hpp"

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

#endif //BUNSAN_COMPLEX_SERVICE_HPP

