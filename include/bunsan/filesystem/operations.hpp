#pragma once

#include "bunsan/config.hpp"

#include <boost/system/system_error.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

namespace bunsan{namespace filesystem
{
    /*!
     * \brief recreate directory
     */
    void reset_dir(const boost::filesystem::path &dir);

    using boost::filesystem::copy_option;

    void copy_tree(const boost::filesystem::path &from,
                   const boost::filesystem::path &to);

    void copy_tree(const boost::filesystem::path &from,
                   const boost::filesystem::path &to,
                   boost::system::error_code &ec);

    void copy_tree(const boost::filesystem::path &from,
                   const boost::filesystem::path &to,
                   const BOOST_SCOPED_ENUM(copy_option) option);

    void copy_tree(const boost::filesystem::path &from,
                   const boost::filesystem::path &to,
                   const BOOST_SCOPED_ENUM(copy_option) option,
                   boost::system::error_code &ec);
}}
