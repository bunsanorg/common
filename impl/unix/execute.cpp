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

int bunsan::sync_execute(const boost::filesystem::path &cwd, const std::vector<std::string> &args)
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
	int code;
	size_t errcount = 0;
	while (errcount++<10 && (code  = system(sout.str().c_str()))==-1);
	if (code==-1)
	{
		DLOG(unable to run program correct);
		return -1;
	}
	else
	{
		SLOG("program was finished with \""<<code<<"\" status code: return code=\""<<WEXITSTATUS(code)<<"\"");
		return WEXITSTATUS(code);
	}
}

