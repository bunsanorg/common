#ifndef SIMPLE_SERVICE_HPP
#define SIMPLE_SERVICE_HPP

namespace bunsan
{
	class simple_service
	{
	public:
		virtual void join()=0;
		virtual void stop()=0;
		virtual inline ~simple_service(){}
	};
}

#endif //SIMPLE_SERVICE_HPP

