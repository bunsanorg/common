#pragma once

#include <boost/serialization/split_free.hpp>
#include <boost/serialization/nvp.hpp>

#include <boost/filesystem/path.hpp>

BOOST_SERIALIZATION_SPLIT_FREE(boost::filesystem::path)

namespace boost{namespace serialization
{
    template <typename Archive>
    void save(Archive &ar, const boost::filesystem::path &path, const unsigned int /*version*/)
    {
        ar & boost::serialization::make_nvp("path", path.generic_string());
    }

    template <typename Archive>
    void load(Archive &ar, boost::filesystem::path &path, const unsigned int /*version*/)
    {
        std::string str;
        ar & boost::serialization::make_nvp("path", str);
        path = str;
    }
}}
