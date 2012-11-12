#pragma once

#include <map>
#include <string>
#include <memory>
#include <functional>
#include <type_traits>

#include "bunsan/factory.hpp"

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
    typedef std::shared_ptr<CLASS> CLASS##_ptr; \
    typedef bunsan::factory<CLASS##_ptr(__VA_ARGS__)> bunsan_factory; \
    typedef typename bunsan_factory::factory_type factory_type; \
    template <typename ... Args> \
    static inline typename std::enable_if<bunsan_factory::arguments_size::value == sizeof...(Args), \
    CLASS##_ptr>::type instance(const typename bunsan_factory::key_type &type, Args &&...args) \
    { \
        return bunsan_factory::instance(factories, type, std::forward<Args>(args)...); \
    } \
    static inline typename bunsan_factory::const_iterator registered_begin() \
    { \
        return bunsan_factory::registered_begin(factories); \
    } \
    static inline typename bunsan_factory::const_iterator registered_end() \
    { \
        return bunsan_factory::registered_end(factories); \
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
protected: \
    static inline bool register_new(const typename bunsan_factory::key_type &type, const factory_type &f) \
    { \
        return bunsan_factory::register_new(factories, type, f); \
    } \
private: \
    static typename bunsan_factory::map_type *factories; \
}; \
typedef CLASS::CLASS##_ptr CLASS##_ptr;

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
    typename CLASS::bunsan_factory::map_type *CLASS::factories;
