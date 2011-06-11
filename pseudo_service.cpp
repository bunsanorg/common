#include "pseudo_service.hpp"

bunsan::pseudo_service::pseudo_service(): running(false){}

void bunsan::pseudo_service::start()
{
	guard lk(run_lock);
	running = true;
}

void bunsan::pseudo_service::join()
{
	guard lk(run_lock);
	joiner.wait(lk, [running](){return !running;});
}

void bunsan::pseudo_service::stop()
{
	guard lk(run_lock);
	running = false;
	joiner.notify_all();
}

bool bunsan::pseudo_service::is_running()
{
	guard lk(run_lock);
	return running;
}

bunsan::pseudo_service::~pseudo_service()
{
	stop();
}

