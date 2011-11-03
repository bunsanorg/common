#ifndef BUNSAN_FACTORY_HPP
#define BUNSAN_FACTORY_HPP

#include <map>
#include <string>
#include <functional>

namespace bunsan{namespace factory
{
	template <typename Factory>
	bool register_new(std::map<std::string, Factory> *&factories, const std::string &type, const Factory f)
	{
		if (!factories)
			factories = new std::map<std::string, Factory>;
		if (factories->find(type)==factories->end())
		{
			(*factories)[type] = f;
			return true;
		}
		else
		{
			return false;
		}
	}
	template <typename Factory, typename ... Args>
	auto instance(std::map<std::string, Factory> *&factories, const std::string &type, Args &&...args)
		->decltype(factories->find(type)->second(std::forward<Args>(args)...))
	{
		if (factories)
		{
			auto iter = factories->find(type);
			if (iter!=factories->end())
				return iter->second(args...);
		}
		return decltype(factories->find(type)->second(std::forward<Args>(args)...))();
	}
}}

#endif //BUNSAN_FACTORY_HPP

