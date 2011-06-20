#include "executor.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>

#include "util.hpp"

bunsan::executor::executor(const std::string &command): positional(0), next_positional(0)
{
	SLOG("creating executor object from \""<<command<<"\"");
	substring str = command;
	argument arg;
	arg.push_back(str);
	arguments.push_back(arg);
}

void bunsan::executor::process(argument &arg, const boost::property_tree::ptree::value_type &arg_value)
{
	//DLOG();
	if (arg_value.first=="t")
	{
		substring str = arg_value.second.get_value<std::string>();
		SLOG("text type \""<<str<<"\"");
		arg.push_back(str);
	}
	else if (arg_value.first=="p")
	{
		size_t ph = arg_value.second.get_value<size_t>();
		SLOG("positional placeholder \""<<ph<<"\"");
		substring str = ph;
		if (positional.size()<=ph)
			positional.resize(ph+1);
		arg.push_back(str);
	}
	else
	{
		SLOG("unknown type \""<<arg_value.first<<"\"");
		throw std::runtime_error("unknown ["+arg_value.first+"]");
	}
}

bunsan::executor::executor(const boost::property_tree::ptree &command): next_positional(0)
{
	DLOG(creating executor object from boost::property_tree::ptree);
	for (const auto &arg_value: command)
	{
		argument arg;
		if (arg_value.first=="c")
		{
			for (const auto &arg_subvalue: arg_value.second)
			{
				process(arg, arg_subvalue);
			}
		}
		else if (arg_value.first=="e")
		{
			executable(arg_value.second.get_value<std::string>());
			continue;
		}
		else if (arg_value.first=="w")
		{
			current_path(arg_value.second.get_value<std::string>());
			continue;
		}
		else
		{
			process(arg, arg_value);
		}
		arguments.push_back(arg);
	}
	DLOG(created);
}

void bunsan::executor::operator()() const
{
	DLOG(trying to execute);
	int code = sync();
	if (code)
		throw bunsan::return_code(code);
}

bunsan::executor &bunsan::executor::add_argument(const std::string &arg)
{
	SLOG("adding argument \""<<arg<<"\"");
	if (next_positional<positional.size())
	{
		positional[next_positional] = arg;
		++next_positional;
	}
	else
	{
		substring str = arg;
		argument arg;
		arg.push_back(str);
		arguments.push_back(arg);
	}
	return *this;
}

bunsan::executor &bunsan::executor::current_path(const boost::filesystem::path &cwd)
{
	SLOG("setting cwd to "<<cwd);
	current_path_ = cwd;
	return *this;
}

bunsan::executor &bunsan::executor::executable(const boost::filesystem::path &exec_)
{
	SLOG("setting executable to "<<exec_);
	this->exec_ = exec_;
	return *this;
}

class args_visitor: public boost::static_visitor<std::string>
{
public:
	explicit args_visitor(const std::vector<boost::optional<std::string>> *positional_): positional(positional_){}
	std::string operator()(const std::string &str)
	{
		SLOG("returning std::string from std::string=\""<<str<<"\"");
		return str;
	}
	std::string operator()(size_t n)
	{
		SLOG("returning std::string=\""<<positional->at(n).get()<<"\" from size_t=\""<<n<<"\"");
		return positional->at(n).get();
	}
private:
	const std::vector<boost::optional<std::string>> *positional;
};

#include "execute.hpp"

void prepare(std::vector<std::string> &args, args_visitor &visitor, const std::vector<bunsan::executor::argument> &arguments, const std::vector<bunsan::executor::string_opt> &positional)
{
	for (size_t i = 0; i<arguments.size(); ++i)
	{
		for (size_t j = 0; j<arguments[i].size(); ++j)
		{
			args[i] = (boost::apply_visitor(visitor, arguments[i][j]));
		}
	}
}

int bunsan::executor::sync() const
{
	DLOG(trying to execute);
	std::vector<std::string> args(arguments.size());
	args_visitor visitor(&positional);
	prepare(args, visitor, arguments, positional);
	if (exec_)
		return bunsan::sync_execute(current_path_.get_value_or(boost::filesystem::current_path()), exec_.get(), args, false);
	else
	{
		if (args.empty())
			throw std::runtime_error("Nothing to execute");
		boost::filesystem::path exec_ = args.at(0);
		if (exec_.is_absolute())
			return bunsan::sync_execute(current_path_.get_value_or(boost::filesystem::current_path()), exec_, args, false);
		else
			return bunsan::sync_execute(current_path_.get_value_or(boost::filesystem::current_path()), exec_, args, true);
	}
}

bunsan::return_code::return_code(int code_): code(code_), std::runtime_error("command was finished with \""+boost::lexical_cast<std::string>(code_)+"\" return code"){}

bunsan::return_code::operator int() const throw()
{
	return code;
}

