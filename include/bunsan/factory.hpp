#pragma once

#include <bunsan/error.hpp>

#include <boost/iterator/transform_iterator.hpp>
#include <boost/optional.hpp>

#include <algorithm>
#include <functional>
#include <iterator>
#include <string>
#include <type_traits>
#include <unordered_map>

namespace bunsan
{
    struct unknown_factory_error: virtual error
    {
        typedef boost::error_info<struct tag_factory_type, std::string> factory_type;
    };
}

namespace bunsan{namespace detail
{
    /*!
     * \brief This class is implementation of bunsan::factory concept.
     *
     * bunsan::factory is abstract factory implementation, where each derived
     * class is registered in abstract factory by std::string identifier
     * on library load using bunsan::factory::register_new().
     * Later instance of this type may be created using bunsan::factory::instance().
     *
     * \note We need class name different than "factory" to use it as member function's.
     */
    template <typename Signature, typename UnknownError>
    class factory_base;

    template <typename Result, typename ... Args, typename UnknownError>
    class factory_base<Result (Args...), UnknownError>
    {
    public:
        typedef Result result_type;
        typedef std::function<Result (Args...)> factory_type;
        typedef UnknownError unknown_error;
        typedef std::string key_type;
        typedef std::integral_constant<std::size_t, sizeof...(Args)> arguments_size;
        typedef std::unordered_map<key_type, factory_type> map_type;
        typedef typename map_type::value_type value_type;
        typedef typename map_type::const_iterator map_const_iterator;

        struct iterator_converter: std::unary_function<const value_type &, const key_type &>
        {
            const key_type &operator()(const value_type &pair) const
            {
                return pair.first;
            }
        };

        typedef boost::transform_iterator<iterator_converter, map_const_iterator> const_iterator;

        /*!
         * \brief Registeres new factory with unique identifier.
         *
         * \warning factory should not return null pointer
         *
         * \return false if identifier is already used
         */
        static bool register_new(map_type *&factories,
                                 const key_type &type,
                                 const factory_type &f)
        {
            if (!factories)
                factories = new map_type;
            if (factories->find(type) == factories->end())
            {
                factories->insert(value_type(type, f));
                return true;
            }
            return false;
        }

        /*!
         * \brief Returns factory registered with specified identifier.
         *
         * \throws unknown_factory if factory is not registered
         */
        static factory_type factory(const map_type *const factories,
                                    const key_type &type)
        {
            if (factories)
            {
                const map_const_iterator iter = factories->find(type);
                if (iter != factories->end())
                    return iter->second;
            }
            BOOST_THROW_EXCEPTION(unknown_error() <<
                                  unknown_factory_error::factory_type(type));
        }

        /*!
         * \brief Returns factory registered with specified identifier.
         *
         * \return null if factory is not registered
         */
        static boost::optional<factory_type> factory_optional(const map_type *const factories,
                                                              const key_type &type)
        {
            if (factories)
            {
                const map_const_iterator iter = factories->find(type);
                if (iter != factories->end())
                    return iter->second;
            }
            return boost::optional<factory_type>();
        }

        /*!
         * \brief Returns new instance of specified type.
         *
         * \throws unknown_factory if factory is not registered
         */
        static result_type instance(const map_type *const factories,
                                    const key_type &type,
                                    Args &&...args)
        {
            if (factories)
            {
                const map_const_iterator iter = factories->find(type);
                if (iter != factories->end())
                    // TODO should we assert that it is not nullptr?
                    return iter->second(std::forward<Args>(args)...);
            }
            BOOST_THROW_EXCEPTION(unknown_error() <<
                                  unknown_factory_error::factory_type(type));
        }

        /*!
         * \brief Returns new instance of specified type.
         *
         * \return nullptr if factory is not registered
         */
        static result_type instance_optional(const map_type *const factories,
                                             const key_type &type,
                                             Args &&...args)
        {
            if (factories)
            {
                const map_const_iterator iter = factories->find(type);
                if (iter != factories->end())
                    // TODO should we assert that it is not nullptr?
                    return iter->second(std::forward<Args>(args)...);
            }
            return result_type();
        }

        /// Iterate over registered factory identifiers: begin iterator.
        static const_iterator registered_begin(const map_type *const factories)
        {
            if (factories)
                return const_iterator(factories->begin());
            else
                /// \see factory::null unit test
                return const_iterator(map_const_iterator());
        }

        /// Iterate over registered factory identifiers: end iterator.
        static const_iterator registered_end(const map_type *const factories)
        {
            if (factories)
                return const_iterator(factories->end());
            else
                /// \see factory::null unit test
                return const_iterator(map_const_iterator());
        }
    };
}}

namespace bunsan
{
    template <typename Signature, typename UnknownError=bunsan::unknown_factory_error>
    using factory = detail::factory_base<Signature, UnknownError>;
}
