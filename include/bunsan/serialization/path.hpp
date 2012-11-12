#pragma once

#include <boost/serialization/split_free.hpp>

#include <boost/filesystem/path.hpp>

BOOST_SERIALIZATION_SPLIT_FREE(boost::filesystem::path)

namespace boost{namespace serialization
{
    // FIXME nvp
    template <typename Archive>
    void save(Archive &ar, const boost::filesystem::path &path, const unsigned int /*version*/)
    {
        ar & path.generic_string();
    }

    template <typename Archive>
    void load(Archive &ar, boost::filesystem::path &path, const unsigned int /*version*/)
    {
        std::string str;
        ar & str;
        path = str;
    }
}}
