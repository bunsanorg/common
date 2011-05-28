#include "tempfile.hpp"

#include "util.hpp"

/*bunsan::tempfile::tempfile(): file(boost::filesystem::unique_path(boost::filesystem::temp_directory_path()/"%%%%-%%%%-%%%%-%%%%")), do_auto_remove(true) {}*/

//bunsan::tempfile::tempfile(const boost::filesystem::path &model): file(boost::filesystem::unique_path(model)), do_auto_remove(true) {}

bunsan::tempfile_ptr bunsan::tempfile::in_dir(const boost::filesystem::path &dir)
{
	return bunsan::tempfile::from_model(dir/"%%%%-%%%%-%%%%-%%%%");
}

bunsan::tempfile_ptr bunsan::tempfile::from_model(const boost::filesystem::path &model)
{
	return bunsan::tempfile_ptr(new tempfile(boost::filesystem::unique_path(model)));
}

bunsan::tempfile_ptr bunsan::tempfile::unique()
{
	return bunsan::tempfile::from_model(boost::filesystem::temp_directory_path()/"%%%%-%%%%-%%%%-%%%%");
}

bunsan::tempfile::tempfile(const boost::filesystem::path &file_): file(file_), do_auto_remove(true) {}

boost::filesystem::path bunsan::tempfile::path() const
{
	return file;
}

std::string bunsan::tempfile::native() const
{
	return file.native();
}

bool bunsan::tempfile::auto_remove() const
{
	return do_auto_remove;
}

void bunsan::tempfile::auto_remove(bool do_auto_remove_)
{
	do_auto_remove = do_auto_remove_;
}

bunsan::tempfile::~tempfile() throw()
{
	if (do_auto_remove)
	{
		try
		{
			SLOG("trying to remove tempfile "<<file);
			if (boost::filesystem::exists(file))
				boost::filesystem::remove_all(file);
		}
		catch(std::exception &e)
		{
			DLOG(error);
			SLOG(e.what());
			SLOG("unable to remove "<<file);
		}
		catch(...)
		{
			DLOG(unknown error);
			SLOG("unable to remove "<<file);
		}
	}
	else
	{
		SLOG("skipping removing tempfile "<<file);
	}
}

