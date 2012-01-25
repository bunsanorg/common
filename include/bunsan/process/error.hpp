#ifndef BUNSAN_PROCESS_ERROR_HPP
#define BUNSAN_PROCESS_ERROR_HPP

#include "bunsan/error.hpp"

namespace bunsan{namespace process
{
	struct error: virtual bunsan::error
	{
		error()=default;
		explicit error(const std::string &msg);
	};
	class non_zero_exit_status_error: public virtual error
	{
	public:
		non_zero_exit_status_error()=default;
		explicit non_zero_exit_status_error(int exit_status_);
		virtual int exit_status() const throw();
	private:
		typedef boost::error_info<struct error_exit_status_tag, int> error_exit_status;
	};
}}

#endif //BUNSAN_PROCESS_ERROR_HPP

