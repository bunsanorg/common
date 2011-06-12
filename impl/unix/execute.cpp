#include "execute.hpp"

#include <sstream>
#include <vector>
#include <map>

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <boost/scoped_array.hpp>

#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#include "util.hpp"
#include "process.hpp"
#include "process_impl.hpp"
#include "helpers.hpp"

template <typename S>
void quote(S &out, const std::string &str)
{
	out<<'"';
	for (size_t j = 0; j<str.size(); ++j)
	{
		if (str[j]=='"' || str[j]=='\\')
			out<<'\\';
		out<<str[j];
	}
	out<<'"';
}

/*int bunsan::impl::sync_execute(const boost::filesystem::path &cwd, const std::vector<std::string> &args)
{
	std::stringstream sout;
	DLOG(trying to execute the following args);
	sout<<"cd ";
	quote(sout, cwd.native());
	sout<<" && ";
	for (size_t i = 0; i<args.size(); ++i)
	{
		if (i)
			sout<<' ';
		quote(sout, args[i]);
	}
	SLOG(sout.str());
	int code = system(sout.str().c_str());
	SLOG("program was finished with \""<<code<<"\" status code: return code=\""<<WEXITSTATUS(code)<<"\"");
	return WEXITSTATUS(code);
}*/

int bunsan::sync_execute(const boost::filesystem::path &cwd, const boost::filesystem::path &file, const std::vector<std::string> &args, const boost::optional<boost::filesystem::path> &stdin_file, bool use_path)
{
	process_ptr p = async_execute(cwd, file, args, stdin_file, use_path);
	return p->return_code();
}

bunsan::process_ptr bunsan::async_execute(const boost::filesystem::path &cwd, const boost::filesystem::path &file, const std::vector<std::string> &args, const boost::optional<boost::filesystem::path> &stdin_file, bool use_path)
{
	static const size_t arglen = 256;
	SLOG("trying to execute the following args from dir "<<cwd);
	boost::scoped_array<char [arglen]> argv_st(new char [args.size()][arglen]);
	std::vector<char *> argv(args.size()+1);
	for (size_t i = 0; i<args.size(); ++i)
	{
		SLOG("["<<i<<"]: \""<<args[i]<<"\"");
		argv[i] = argv_st[i];
#warning error possible
		strncpy(argv[i], args[i].c_str(), arglen);
	}
	argv[args.size()] = 0;
	DLOG(trying to fork);
	pid_t pid = fork();
	int err = errno;
	if (pid==-1)
	{
		DLOG(fork error);
		throw std::runtime_error(bunsan::impl::strerror(err));
	}
	else if (!pid)
	{// child
		DLOG(child: trying to chdir);
		if (!chdir(cwd.c_str()))
		{
			DLOG(child: success);
			if (stdin_file)
			{
				DLOG(child: trying to reopen stdin);
				if (!freopen(stdin_file.get().c_str(), "r", stdin))
				{
					DLOG(child: error);
					DLOG(child: do _exit);
					_exit(251);
				}
			}
			DLOG(child: trying to exec);
			int ret;
			if (use_path)
				ret = execvp(file.c_str(), &argv[0]);
			else
				ret = execv(file.c_str(), &argv[0]);
			if (ret==-1)
			{
				int err = errno;
				SLOG(bunsan::impl::strerror(err));
				_exit(252);
			}
		}
		else
		{
			DLOG(child: error);
			DLOG(child: do _exit);
			_exit(254);
		}
		DLOG(should not happened);
		_exit(253);
		return process_ptr(); //should not happened //XXX
	}
	else
	{//parent
		process_ptr p(new process_impl(pid));
		return p;
		/*int code;
		DLOG(waiting child);
#warning error possible, need check
		if (pid!=waitpid(pid, &code, 0))
			throw std::runtime_error("waitpid error: \""+strerror(errno)+"\"");
		DLOG(child completed);
		return WEXITSTATUS(code);*/
	}
}

#if 0
int bunsan::impl::sync_execute(const boost::filesystem::path &cwd, const std::vector<std::string> &args, const std::map<std::string, std::string> &environ)
{
	throw std::runtime_error(std::string(__FILE__)+":"+std::string(__LINE__)+": unimplemented yet");
}
#endif

