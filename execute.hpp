#ifndef IMPL_EXECUTE_HPP
#define IMPL_EXECUTE_HPP

#include <string>
#include <vector>

#include <boost/optional.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>

namespace bunsan
{
	namespace process
	{
		class context
		{
		public:
			context()=default;
			context(const context &)=default;
			inline context(context &&ctx):	current_path_(std::move(ctx.current_path_)),
							executable_(std::move(ctx.executable_)),
							argv_(std::move(ctx.argv_)),
							use_path_(ctx.use_path_){}
			context &operator=(const context &)=default;
			inline context &operator=(context &&ctx)
			{
				current_path_ = std::move(ctx.current_path_);
				executable_ = std::move(ctx.executable_);
				argv_ = std::move(ctx.argv_);
				use_path_ = ctx.use_path_;
				return *this;
			}
			inline bool operator==(const context &ctx) const
			{
				return	current_path_==ctx.current_path_ &&
					executable_==ctx.executable_ &&
					argv_==ctx.argv_ &&
					use_path_==ctx.use_path_;
			}
			inline context &current_path(const boost::filesystem::path &current_path_)
			{
				this->current_path_ = current_path_;
				return *this;
			}
			inline const boost::filesystem::path &current_path() const
			{
				return current_path_.get();
			}
			inline context &executable(const boost::filesystem::path &executable_)
			{
				this->executable_ = executable_;
				return *this;
			}
			inline const boost::filesystem::path &executable() const
			{
				return executable_.get();
			}
			inline context &argv(const std::vector<std::string> &argv_)
			{
				this->argv_ = argv_;
				return *this;
			}
			inline const std::vector<std::string> &argv() const
			{
				return argv_;
			}
			inline context &use_path(bool use_path_)
			{
				this->use_path_ = use_path_;
				return *this;
			}
			inline bool use_path() const
			{
				return use_path_;
			}
			inline void build()
			{
				if (!current_path_)
					current_path_ = boost::filesystem::current_path();
				if (!executable_)
				{
					if (use_path_)
					{
						if (argv_.empty())
							throw std::runtime_error("Nothing to execute!");
						executable_ = argv_.at(0);
					}
					else
						executable_ = boost::filesystem::absolute(argv_.at(0));
				}
			}
		private:
			boost::optional<boost::filesystem::path> current_path_;
			boost::optional<boost::filesystem::path> executable_;
			std::vector<std::string> argv_;
			bool use_path_;
		};
		int sync_execute(const context &ctx);
		inline int sync_execute(const boost::filesystem::path &cwd, const boost::filesystem::path &executable, const std::vector<std::string> &args, bool use_path=true)
		{
			return sync_execute(context().current_path(cwd).executable(executable).argv(args).use_path(use_path));
		}
		inline int sync_execute(const boost::filesystem::path &cwd, const std::vector<std::string> &args, bool use_path=true)
		{
			return sync_execute(context().current_path(cwd).argv(args).use_path(use_path));
		}
	}
	using process::sync_execute;
}

#endif //IMPL_EXECUTE_HPP

