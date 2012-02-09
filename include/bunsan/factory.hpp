#ifndef BUNSAN_FACTORY_HPP
#define BUNSAN_FACTORY_HPP

#include <map>
#include <string>
#include <functional>
#include <algorithm>
#include <iterator>
#include <type_traits>

#include <boost/iterator/transform_iterator.hpp>

namespace bunsan
{
	template <typename Signature>
	class factory;
	template <typename Result, typename ... Args>
	class factory<Result (Args...)>
	{
	public:
		typedef Result result_type;
		typedef std::function<Result (Args...)> factory_type;
		typedef std::string key_type;
		typedef std::map<key_type, factory_type> map_type;
		typedef typename map_type::value_type value_type;
		typedef typename map_type::const_iterator map_const_iterator;
		struct pair_first
		{
			const key_type &operator()(const value_type &pair) const
			{
				return pair.first;
			}
		};
		typedef std::function<const key_type &(const value_type &)> iterator_converter;
		typedef boost::transform_iterator<iterator_converter, map_const_iterator> const_iterator;
		//
		static bool register_new(
			map_type *&factories,
			const key_type &type,
			const factory_type &f)
		{
			if (!factories)
				factories = new map_type;
			if (factories->find(type)==factories->end())
			{
				factories->insert(value_type(type, f));
				return true;
			}
			return false;
		}
		static result_type instance(
			const map_type *const factories,
			const key_type &type,
			Args &&...args)
		{
			if (factories)
			{
				auto iter = factories->find(type);
				if (iter!=factories->end())
					return iter->second(std::forward<Args>(args)...);
			}
			return result_type();
		}
		static const_iterator registered_begin(const map_type *const factories)
		{
			if (factories)
				return const_iterator(factories->begin(), pair_first());
			else
				/// \see factory_null unit test
				return const_iterator(map_const_iterator(), pair_first());
		}
		static const_iterator registered_end(const map_type *const factories)
		{
			if (factories)
				return const_iterator(factories->end(), pair_first());
			else
				/// \see factory_null unit test
				return const_iterator(map_const_iterator(), pair_first());
		}
	};
}

#endif //BUNSAN_FACTORY_HPP

