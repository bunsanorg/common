#include "util.hpp"

#include <boost/filesystem.hpp>

void bunsan::reset_dir(const boost::filesystem::path &dir)
{
	SLOG("trying to reset "<<dir);
	if (boost::filesystem::exists(dir))
		boost::filesystem::remove_all(dir);
	boost::filesystem::create_directory(dir);
	SLOG("directory "<<dir<<" was reset");
}

