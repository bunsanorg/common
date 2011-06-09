#include "process_impl.hpp"

#include <stdexcept>
#include <string>

#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

#include "util.hpp"
#include "helpers.hpp"

bunsan::process_impl::process_impl(pid_t pid_): pid(pid_), completed_(false){}

void bunsan::process_impl::terminate()
{
	if (completed())
		return;
	::kill(pid, SIGTERM);
}

void bunsan::process_impl::kill()
{
	if (completed())
		return;
	::kill(pid, SIGKILL);
}

bool bunsan::process_impl::completed()
{
	if (completed_ || pid==waitpid(pid, &stat, WNOHANG))
		completed_ = true;
	return completed_;
}

void bunsan::process_impl::wait()
{
	if (completed())
		return;
	SLOG("waiting for child "<<pid);
	if (pid!=waitpid(pid, &stat, 0))
	{
		int err = errno;
		SLOG("waitpid error: \""<<bunsan::impl::strerror(err)<<"\"");
		throw std::runtime_error("waitpid error: \""+bunsan::impl::strerror(err)+"\"");
	}
	else
	{
		SLOG("child "<<pid<<" was completed");
		completed_ = true;
	}
}

bool bunsan::process_impl::wait(boost::posix_time::time_duration timeout)
{
	using namespace boost::posix_time;
	if (completed())
		return true;
	SLOG("starting wait loop for child "<<pid);
	ptime begin(microsec_clock::local_time());
	ptime end = begin+timeout;
	useconds_t usec = timeout.total_microseconds()/10;// TODO magic number
#warning unchecked overflow
	if (usec<=0)
		usec = 100;
	while (ptime(microsec_clock::local_time())<end && !completed())
	{
		usleep(usec);
	}
	return completed();
}

int bunsan::process_impl::return_code()
{
	if (!completed())
		wait();
	if (WIFEXITED(stat))
		return WEXITSTATUS(stat);
	else
		return -1;
}

bunsan::process_impl::~process_impl() throw()
{
	if (!completed())
	{
#warning bad implementation
#warning no error check
		terminate();
		if (wait(boost::posix_time::seconds(1)))
		{
			return;
		}
		else
		{
			kill();
			waitpid(pid, &stat, 0);// should return fast
		}
	}
}

