#include "bunsan/process/error.hpp"

#include <boost/format.hpp>

using namespace bunsan::process;

error::error(const std::string &msg): bunsan::error(msg) {}

non_zero_exit_status_error::non_zero_exit_status_error(int exit_status_):
	error((boost::format("Program has completed with non-zero exit status = %1") % exit_status_).str())
{
	(*this)<<error_exit_status(exit_status_);
}

int non_zero_exit_status_error::exit_status() const throw()
{
	const int *exit_status_ = boost::get_error_info<error_exit_status>(*this);
	BOOST_ASSERT(exit_status_);
	return *exit_status_;
}

