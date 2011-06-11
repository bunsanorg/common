#include "complex_service.hpp"

void bunsan::complex_service::start()
{
	for (service_ptr &s: services)
		s->start();
}

void bunsan::complex_service::join()
{
	for (service_ptr &s: services)
		s->join();
}

void bunsan::complex_service::stop()
{
	for (service_ptr &s: services)
		s->stop();
}

bool bunsan::complex_service::is_running()
{
	for (service_ptr &s: services)
		if (s->is_running())
			return true;
	return false;
}

bunsan::complex_service::~complex_service()
{
	stop();
	join();
}

