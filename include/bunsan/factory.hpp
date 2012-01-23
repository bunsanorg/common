#ifndef BUNSAN_FACTORY_HPP
#define BUNSAN_FACTORY_HPP

#include <map>
#include <string>
#include <functional>
#include <type_traits>

namespace bunsan{namespace factory
{
	template <typename Factory>
	bool register_new(
		std::map<std::string, Factory> *&factories,
		const std::string &type,
		const Factory &f)
	{
		typedef std::map<std::string, Factory> map_type;
		typedef typename map_type::value_type value_type;
		if (!factories)
			factories = new map_type;
		if (factories->find(type)==factories->end())
		{
			factories->insert(value_type(type, f));
			return true;
		}
		return false;
	}
	/*!
	 * \note
	 * we can use Factory::result_type if Factory is instantiation of std::function,
	 * but in general it is not
	 */
	template <typename Factory, typename ... Args>
	typename std::result_of<Factory(Args &&...)>::type instance(
		std::map<std::string, Factory> *&factories,
		const std::string &type,
		Args &&...args)
	{
		if (factories)
		{
			auto iter = factories->find(type);
			if (iter!=factories->end())
				return iter->second(std::forward<Args>(args)...);
		}
		return typename std::result_of<Factory(Args &&...)>::type();
	}
}}

#endif //BUNSAN_FACTORY_HPP

