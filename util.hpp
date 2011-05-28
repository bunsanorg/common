#ifndef UTIL_HPP
#define UTIL_HPP

#include <iostream>

#include <boost/date_time/posix_time/posix_time.hpp>

#define SLOG(M) std::cerr<<"["<<__FILE__<<":"<<__LINE__<<":"<<__func__<<":"<<boost::posix_time::second_clock::local_time()<<"] "<<M<<std::endl
#define DLOG(M) SLOG(#M)

#include <boost/filesystem/path.hpp>

namespace bunsan
{
	void reset_dir(const boost::filesystem::path &dir);
}

#endif //UTIL_HPP

