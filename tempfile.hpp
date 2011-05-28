#ifndef TEMPFILE_HPP
#define TEMPFILE_HPP

#include <string>
#include <memory>

#include <boost/noncopyable.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp>

namespace bunsan
{
	class tempfile: private boost::noncopyable
	{
	public:
		typedef std::shared_ptr<tempfile> tempfile_ptr;
		explicit tempfile(const boost::filesystem::path &file_);
		boost::filesystem::path path() const;
		std::string native() const;
		bool auto_remove() const;
		void auto_remove(bool do_auto_remove_);
		~tempfile() throw();
		static tempfile_ptr in_dir(const boost::filesystem::path &dir);
		static tempfile_ptr unique();
		static tempfile_ptr from_model(const boost::filesystem::path &model);
	private:
		bool do_auto_remove;
		boost::filesystem::path file;
	};
	typedef tempfile::tempfile_ptr tempfile_ptr;
}

#endif //TEMPFILE_HPP

