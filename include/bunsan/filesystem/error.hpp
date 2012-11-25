#pragma once

#include "bunsan/error.hpp"
#include "bunsan/system_error.hpp"

#include <iostream>
#include <utility>

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

    struct system_error: virtual error, virtual bunsan::system_error
    {
        template <typename ... Args>
        explicit system_error(Args &&...args):
            bunsan::system_error(std::forward<Args>(args)...) {}
    };
}}
