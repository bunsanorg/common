#ifndef RUNNER_HPP
#define RUNNER_HPP

namespace bunsan
{
	/*!
	 * \brief class which runs function in constructor
	 */
	class runner
	{
	public:
		template <typename F, typename ... Args>
		runner(F f, Args &...args)
		{
			f(args...);
		}
	};
}

#endif //RUNNER_HPP

