#pragma once

#include <bunsan/factory.hpp>

#include <boost/preprocessor/cat.hpp>

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <type_traits>

/*!
 * \def BUNSAN_FACTORY_BEGIN(CLASS, ...)
 *
 * \param CLASS class name
 * \param ... constructor argument types
 *
 * should be used after class name declaration,
 * for example
 *
\code
namespace bunsan
{
    class factory_base
    BUNSAN_FACTORY_BEGIN(factory_base, const std::string &)
    public:
        virtual void f()=0;
    BUNSAN_FACTORY_END(factory_base)
}
\endcode
 */
#ifdef BUNSAN_FACTORY_BEGIN
#   error ASSERTION: BUNSAN_FACTORY_BEGIN is in use
#endif

#define BUNSAN_FACTORY_BEGIN(CLASS, ...) \
{\
public: \
    struct unknown_##CLASS##_error: \
        virtual ::bunsan::unknown_factory_error {}; \
    template <typename T> \
    using shared_ptr = std::shared_ptr<T>; \
    using CLASS##_ptr = shared_ptr<CLASS>; \
    using bunsan_factory = bunsan::factory< \
        CLASS##_ptr (__VA_ARGS__), \
        unknown_##CLASS##_error \
    >; \
    using factory_type = typename bunsan_factory::factory_type; \
    static inline factory_type factory_optional( \
        const typename bunsan_factory::key_type &type) \
    { \
        return bunsan_factory::factory_optional(factories, type); \
    } \
    static inline factory_type factory( \
        const typename bunsan_factory::key_type &type) \
    { \
        return bunsan_factory::factory(factories, type); \
    } \
    template <typename ... Args> \
    static inline typename std::enable_if< \
        bunsan_factory::arguments_size::value == sizeof...(Args), CLASS##_ptr>:: \
    type instance_optional(const typename bunsan_factory::key_type &type, \
                           Args &&...args) \
    { \
        return bunsan_factory::instance_optional( \
            factories, \
            type, \
            std::forward<Args>(args)...\
        ); \
    } \
    template <typename ... Args> \
    static inline typename std::enable_if< \
        bunsan_factory::arguments_size::value == sizeof...(Args), CLASS##_ptr>:: \
    type instance(const typename bunsan_factory::key_type &type, Args &&...args) \
    { \
        return bunsan_factory::instance( \
            factories, \
            type, \
            std::forward<Args>(args)...\
        ); \
    } \
    static inline typename bunsan_factory::const_range registered() \
    { \
        return bunsan_factory::registered(factories); \
    } \
private:

/*!
 * \def BUNSAN_FACTORY_END(CLASS)
 *
 * \param CLASS class name
 *
 * should be used after class members declarations
 *
 * \see BUNSAN_FACTORY_BEGIN
 */
#ifdef BUNSAN_FACTORY_END
#   error ASSERTION: BUNSAN_FACTORY_END is in use
#endif
#define BUNSAN_FACTORY_END(CLASS) \
public: \
    static inline bool register_new( \
        const typename bunsan_factory::key_type &type, \
        const factory_type &f) \
    { \
        return bunsan_factory::register_new(factories, type, f); \
    } \
private: \
    static typename bunsan_factory::map_ptr_type factories; \
}; \
using CLASS##_ptr = CLASS::CLASS##_ptr; \
using unknown_##CLASS##_error = CLASS::unknown_##CLASS##_error;

/*!
 * \def BUNSAN_FACTORY_DEFINE(CLASS)
 *
 * \param CLASS class name
 *
 * \note
 * class name should be qualified
 * depending on surrounding namespaces
 *
 * should be used in implementation file, for example
 *
\code
BUNSAN_FACTORY_DEFINE(bunsan::factory_base)
\endcode
\code
namespace bunsan
{
    BUNSAN_FACTORY_DEFINE(factory_base)
}
\endcode
 */
#ifdef BUNSAN_FACTORY_DEFINE
#   error ASSERTION: BUNSAN_FACTORY_DEFINE is in use
#endif
#define BUNSAN_FACTORY_DEFINE(CLASS) \
    typename CLASS::bunsan_factory::map_ptr_type CLASS::factories;

#ifdef BUNSAN_FACTORY_REGISTER
#   error ASSERTION: BUNSAN_FACTORY_REGISTER is in use
#endif
#define BUNSAN_FACTORY_REGISTER(VAR, FACTORY, NAME, CODE) \
    static const bool BOOST_PP_CAT(bunsan_factory_register_, BOOST_PP_CAT(VAR, __LINE__)) = \
        FACTORY::register_new(NAME, CODE);
#ifdef BUNSAN_FACTORY_REGISTER_TOKEN
#   error ASSERTION: BUNSAN_FACTORY_REGISTER_TOKEN is in use
#endif
#define BUNSAN_FACTORY_REGISTER_TOKEN(FACTORY, TOKEN, CODE) \
    BUNSAN_FACTORY_REGISTER(TOKEN, FACTORY, #TOKEN, CODE)
