#include "process_impl.hpp"

#include <stdexcept>
#include <string>

#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

#include "util.hpp"

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
		DLOG(waitpid error);
		throw std::runtime_error("waitpid error");
	}
	else
	{
		SLOG("child "<<pid<<" was completed");
		completed_ = true;
	}
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
		terminate();
		if (completed())
			return;
		else
			sleep(1);
		if (!completed())
		{
			kill();
			waitpid(pid, &stat, 0);
		}
	}
}

