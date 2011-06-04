#ifndef SERVICE_HPP
#define SERVICE_HPP

namespace bunsan
{
	class service
	{
	public:
		/*!
		 * \brief start service
		 * non-blocking call, thread will run in different thread
		 */
		virtual void start()=0;
		/*!
		 * \brief join service thread
		 */
		virtual void join()=0;
		/*!
		 * \brief stop service
		 */
		virtual void stop()=0;
		/*!
		 * \return wether service is running
		 */
		virtual bool is_running()=0;
		virtual inline ~service(){}
	};
}

#endif //SERVICE_HPP

