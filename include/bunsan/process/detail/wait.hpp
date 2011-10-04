#ifndef BUNSAN_PROCESS_DETAIL_WAIT_HPP
#define BUNSAN_PROCESS_DETAIL_WAIT_HPP

#include <chrono>

#include <boost/process/child.hpp>

namespace bunsan{namespace process{namespace detail{
	/// wait for child completion for infinite time
	int wait(const boost::process::child &child);
	int wait(boost::process::child &&child);
	/// wait for child completion for specified time
	int wait(const boost::process::child &child, const std::chrono::seconds &timeout);
	int wait(boost::process::child &&child, const std::chrono::seconds &timeout);
}}}

#endif //BUNSAN_PROCESS_DETAIL_WAIT_HPP

