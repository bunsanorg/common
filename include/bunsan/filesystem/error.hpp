#pragma once

#include <bunsan/error.hpp>
#include <bunsan/system_error.hpp>

#include <boost/filesystem/path.hpp>

namespace bunsan{namespace filesystem
{
    struct error: virtual bunsan::error
    {
        typedef boost::error_info<struct tag_path, boost::filesystem::path> path;
        typedef boost::error_info<struct tag_source_path, boost::filesystem::path> source_path;
        typedef boost::error_info<struct tag_destination_path, boost::filesystem::path> destination_path;

        typedef boost::error_info<struct tag_openmode, std::ios_base::openmode> openmode;
    };

    struct system_error: bunsan::system_error, virtual error
    {
        using bunsan::system_error::system_error;
    };
}}

namespace boost
{
    std::string to_string(const bunsan::filesystem::error::openmode &openmode);
}
