#pragma once

#include <bunsan/error.hpp>

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/if.hpp>
#include <boost/preprocessor/stringize.hpp>

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/tuple/to_seq.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/tuple/enum.hpp>

#include <iostream>
#include <string>

namespace bunsan{namespace stream_enum
{
    /// Base exception class for stream_enum errors.
    struct invalid_value_error: virtual error {};

    /// String representation is invalid.
    struct invalid_string_representation_error: virtual invalid_value_error
    {
        typedef boost::error_info<struct tag_string_value, std::string> string_value;
    };

    /// Enum representation is invalid.
    struct invalid_enum_representation_error: virtual invalid_value_error
    {
        typedef boost::error_info<struct tag_enum_value, long long> enum_value;
    };
}}

/*!
 * \brief Macro repeats a macro for each element in a tuple.
 *
 * macro(r, data, item) for each item in tuple
 */
#define BOOST_PP_TUPLE_FOR_EACH(macro, data, tuple) BOOST_PP_SEQ_FOR_EACH(macro, data, BOOST_PP_TUPLE_TO_SEQ(tuple))

/*!
 * \brief Qualified item name.
 *
 * \param IS_SCOPED_NAME tuple (IS_SCOPED, NAME)
 * \param ITEM item
 */
#define BUNSAN_STREAM_ENUM_ITEM(IS_SCOPED_NAME, ITEM) \
    BOOST_PP_IF(BOOST_PP_TUPLE_ELEM(0, IS_SCOPED_NAME), BOOST_PP_TUPLE_ELEM(1, IS_SCOPED_NAME)::, )ITEM

/// Error name for exception derived from ::bunsan::stream_enum::invalid_value_error.
#define BUNSAN_STREAM_ENUM_ERROR(NAME) BOOST_PP_CAT(NAME, _invalid_value_error)

/// Error name for exception derived from ::bunsan::stream_enum::invalid_string_representation_error.
#define BUNSAN_STREAM_ENUM_ERROR_STRING(NAME) BOOST_PP_CAT(NAME, _invalid_string_representation_error)

/// Error name for exception derived from ::bunsan::stream_enum::invalid_enum_representation_error.
#define BUNSAN_STREAM_ENUM_ERROR_ENUM(NAME) BOOST_PP_CAT(NAME, _invalid_enum_representation_error)


/*!
 * \brief Define macro for single-item expansion.
 *
 * \param r not used
 * \param IS_SCOPED_NAME tuple (IS_SCOPED, NAME)
 * \param ITEM item
 */
#define BUNSAN_STREAM_ENUM_ISTREAM_PART(r, IS_SCOPED_NAME, ITEM) \
    if (val == BOOST_PP_STRINGIZE(ITEM)) \
        item = BUNSAN_STREAM_ENUM_ITEM(IS_SCOPED_NAME, ITEM); \
    else

/*!
 * \brief Define std::istream &operator>>(...).
 *
 * \param NAME enum name
 * \param ITEMS tuple of enum items
 * \param IS_SCOPED if scoped enum should be used
 * \param SPECS specifiers for function definition (inline, friend, static)
 */
#define BUNSAN_STREAM_ENUM_ISTREAM(NAME, ITEMS, IS_SCOPED, SPECS) \
    SPECS std::istream &operator>>(std::istream &in, NAME &item) \
    { \
        std::string val; \
        in >> val; \
        BOOST_PP_TUPLE_FOR_EACH(BUNSAN_STREAM_ENUM_ISTREAM_PART, (IS_SCOPED, NAME), ITEMS) \
            BOOST_THROW_EXCEPTION(BUNSAN_STREAM_ENUM_ERROR_STRING(NAME)() << \
                ::bunsan::stream_enum::invalid_string_representation_error::string_value(val)); \
        return in; \
    }

/*!
 * \brief Define macro for single-item expansion.
 *
 * \param r not used
 * \param IS_SCOPED_NAME tuple (IS_SCOPED, NAME)
 * \param ITEM item
 */
#define BUNSAN_STREAM_ENUM_OSTREAM_PART(r, IS_SCOPED_NAME, ITEM) \
    case BUNSAN_STREAM_ENUM_ITEM(IS_SCOPED_NAME, ITEM): \
        out << BOOST_PP_STRINGIZE(ITEM); \
        break;

/*!
 * \brief Define std::ostream &operator<<(...).
 *
 * \param NAME enum name
 * \param ITEMS tuple of enum items
 * \param IS_SCOPED if scoped enum should be used
 * \param SPECS specifiers for function definition (inline, friend, static)
 */
#define BUNSAN_STREAM_ENUM_OSTREAM(NAME, ITEMS, IS_SCOPED, SPECS) \
    SPECS std::ostream &operator<<(std::ostream &out, const NAME &item) \
    { \
        switch (item) \
        { \
            BOOST_PP_TUPLE_FOR_EACH(BUNSAN_STREAM_ENUM_OSTREAM_PART, (IS_SCOPED, NAME), ITEMS) \
        default: \
            BOOST_THROW_EXCEPTION(BUNSAN_STREAM_ENUM_ERROR_ENUM(NAME)() << \
                ::bunsan::stream_enum::invalid_enum_representation_error::enum_value( \
                    static_cast< ::bunsan::stream_enum::invalid_enum_representation_error::enum_value::value_type>(item))); \
        } \
        return out; \
    }

/*!
 * \brief Define exceptions derived from appropriate
 * in ::bunsan::stream_enum namespace.
 *
 * \param NAME enum name
 */
#define BUNSAN_STREAM_ENUM_ERRORS(NAME) \
    struct BUNSAN_STREAM_ENUM_ERROR(NAME): \
        virtual ::bunsan::stream_enum::invalid_value_error {}; \
    struct BUNSAN_STREAM_ENUM_ERROR_STRING(NAME): \
        virtual BUNSAN_STREAM_ENUM_ERROR(NAME), \
        virtual ::bunsan::stream_enum::invalid_string_representation_error \
    {} ; \
    struct BUNSAN_STREAM_ENUM_ERROR_ENUM(NAME): \
        virtual BUNSAN_STREAM_ENUM_ERROR(NAME), \
        virtual ::bunsan::stream_enum::invalid_enum_representation_error \
    {};

/*!
 * \brief Define enum with operator<<() and operator>>() defined.
 *
 * This enum is suitable for boost::lexical_cast<NAME>(std::string)
 * and boost::lexical_cast<std::string>(NAME).
 *
 * \param NAME enum name
 * \param ITEMS tuple of enum items: (val1, val2, val3)
 * \param IS_SCOPED 0 for enum NAME {};, 1 for enum class NAME {};
 * \param SPECS specifiers for function definition (inline, friend, static)
 */
#define BUNSAN_STREAM_ENUM_SCOPE(NAME, ITEMS, IS_SCOPED, SPECS) \
    enum BOOST_PP_IF(IS_SCOPED, class, ) NAME \
    { \
        BOOST_PP_TUPLE_ENUM(ITEMS) \
    }; \
    BUNSAN_STREAM_ENUM_ERRORS(NAME) \
    BUNSAN_STREAM_ENUM_ISTREAM(NAME, ITEMS, IS_SCOPED, SPECS) \
    BUNSAN_STREAM_ENUM_OSTREAM(NAME, ITEMS, IS_SCOPED, SPECS)

/*!
 * \brief Define old-style enum with operator<<() and operator>>() defined.
 *
 * \param NAME enum name
 * \param ITEMS tuple of enum items: (val1, val2, val3)
 */
#define BUNSAN_STREAM_ENUM(NAME, ITEMS) BUNSAN_STREAM_ENUM_SCOPE(NAME, ITEMS, 0, inline)

/*!
 * \brief Define old-style enum with operator<<() and operator>>() defined (in-class definition).
 *
 * \param NAME enum name
 * \param ITEMS tuple of enum items: (val1, val2, val3)
 */
#define BUNSAN_INCLASS_STREAM_ENUM(NAME, ITEMS) BUNSAN_STREAM_ENUM_SCOPE(NAME, ITEMS, 0, friend inline)

/*!
 * \brief Define enum with operator<<() and operator>>() defined.
 *
 * \param NAME enum name
 * \param ITEMS tuple of enum items: (val1, val2, val3)
 */
#define BUNSAN_STREAM_ENUM_CLASS(NAME, ITEMS) BUNSAN_STREAM_ENUM_SCOPE(NAME, ITEMS, 1, inline)

/*!
 * \brief Define enum with operator<<() and operator>>() defined (in-class definition).
 *
 * \param NAME enum name
 * \param ITEMS tuple of enum items: (val1, val2, val3)
 */
#define BUNSAN_INCLASS_STREAM_ENUM_CLASS(NAME, ITEMS) BUNSAN_STREAM_ENUM_SCOPE(NAME, ITEMS, 1, friend inline)
