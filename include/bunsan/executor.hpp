#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include <stdexcept>
#include <string>

#include <boost/property_tree/ptree.hpp>
#include <boost/variant.hpp>
#include <boost/filesystem/path.hpp>

#include "bunsan/execute.hpp"
#include "bunsan/util.hpp"

namespace bunsan
{
	class return_code: public std::runtime_error
	{
	public:
		explicit return_code(int code_);
		operator int() const throw();
	private:
		int code;
	};
	class executor
	{
	public:
		explicit executor(const std::string &command);
		explicit executor(const boost::property_tree::ptree &command);
		executor(const executor &)=default;
		executor(executor &&)=default;
		executor &operator=(const executor &)=default;
		executor &operator=(executor &&)=default;
		/*!
		 * \brief run command with specified arguments
		 * \throws return_code if return code is not zero
		 */
		void operator()() const;
		/*!
		 * \return executing context
		 */
		bunsan::process::context context() const;
		/*!
		 * \brief run command synchronous with specified arguments
		 * \return return code
		 */
		int sync() const;
		/*!
		 * \brief add positional argument
		 * \return *this
		 */
		executor &add_argument(const std::string &arg);
		/*!
		 * \brief set named argument
		 */
		executor &set_argument(const std::string &key, const std::string &arg);
		/*!
		 * \brief set current work directory for process
		 */
		executor &current_path(const boost::filesystem::path &cwd);
		/*!
		 * \brief set executable file (without 0 argument will be used)
		 */
		executor &executable(const boost::filesystem::path &exec_);
		template <typename ... Args>
		executor &add_argument(const std::string &arg, const Args &...args)
		{
			add_argument(arg);
			return add_argument(args...);
		}
		template <typename ... Args>
		int sync(const Args &...args) const
		{
			executor tmp(*this);
			return tmp.add_argument(args...).sync();
		}
		template <typename ... Args>
		void operator()(const Args &...args) const
		{
			executor tmp(*this);
			tmp.add_argument(args...)();
		}
		template <typename T, typename ... Args>
		static int run_sync_from(const boost::filesystem::path &cwd, const T &command, const Args &...args)
		{
			executor exc(command);
			return exc.current_path(cwd).sync(args...);
		}
		template <typename T, typename ... Args>
		static int run_sync(const T &command, const Args &...args)
		{
			executor exc(command);
			return exc.sync(args...);
		}
		template <typename T, typename ... Args>
		static void exec_from(const boost::filesystem::path &cwd, const T &command, const Args &...args)
		{
			executor exc(command);
			exc.current_path(cwd)(args...);
		}
		template <typename T, typename ... Args>
		static void exec(const T &command, const Args &...args)
		{
			executor exc(command);
			exc(args...);
		}
	public:
		static constexpr const char *current_path_key = "current_path";
		static constexpr const char *executable_key = "executable";
		static constexpr const char *use_path_key = "use_path";
		typedef boost::variant<size_t, std::string> reference;
		typedef boost::variant<reference, std::string> token;
		typedef std::vector<token> string;
		typedef boost::optional<std::string> string_opt;
		typedef std::map<std::string, std::string> dict;
	private:
		std::vector<string> arguments;
		std::vector<string_opt> positional;
		size_t next_positional;
		dict named;
		class token_visitor;
		class ref_visitor;
		void prepare(std::vector<std::string> &args, token_visitor &visitor) const;
		void process(string &arg, const boost::property_tree::ptree::value_type &arg_value);
	};
}

#endif //EXECUTOR_HPP

