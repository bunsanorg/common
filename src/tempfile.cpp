#include "bunsan/tempfile.hpp"

#include "bunsan/util.hpp"

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

bunsan::tempfile::tempfile(const boost::filesystem::path &file_, bool do_auto_remove_): file(file_), do_auto_remove(do_auto_remove_) {}

boost::filesystem::path bunsan::tempfile::path() const
{
	return file;
}

boost::filesystem::path::string_type bunsan::tempfile::native() const
{
	return file.native();
}

std::string bunsan::tempfile::generic_string() const
{
	return file.generic_string();
}

std::string bunsan::tempfile::string() const
{
	return file.string();
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

