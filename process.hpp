#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <memory>

#include <boost/date_time/posix_time/posix_time_types.hpp>

namespace bunsan
{
	class process
	{
	public:
		virtual void terminate()=0;
		virtual void kill()=0;
		virtual void wait()=0;
		/*!
		 * \return false if timed out
		 * \return true if process was completed
		 */
		virtual bool wait(boost::posix_time::time_duration timeout)=0;
		virtual bool completed()=0;
		virtual int return_code()=0;
		virtual inline ~process() throw(){}
	};
	typedef std::shared_ptr<process> process_ptr;
}

#endif //PROCESS_HPP

