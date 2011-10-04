#ifndef BUNSAN_PROCESS_DETAIL_LAUNCH_HPP
#define BUNSAN_PROCESS_DETAIL_LAUNCH_HPP

#include <boost/process/child.hpp>

#include "bunsan/process/context.hpp"

namespace bunsan{namespace process{namespace detail{
	/// launch specified context and return child handler
	boost::process::child launch(const bunsan::process::context &ctx);
	boost::process::child launch(bunsan::process::context &&ctx);
}}}

#endif //BUNSAN_PROCESS_DETAIL_LAUNCH_HPP

