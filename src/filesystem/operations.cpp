#include "bunsan/filesystem/operations.hpp"

namespace bunsan{namespace filesystem
{
    void copy_tree(const boost::filesystem::path &from,
                   const boost::filesystem::path &to)
    {
        copy_tree(from, to, copy_option::fail_if_exists);
    }

    void copy_tree(const boost::filesystem::path &from,
                   const boost::filesystem::path &to,
                   boost::system::error_code &ec)
    {
        copy_tree(from, to, copy_option::fail_if_exists, ec);
    }

    void copy_tree(const boost::filesystem::path &from,
                   const boost::filesystem::path &to,
                   const BOOST_SCOPED_ENUM(copy_option) option)
    {
        boost::system::error_code ec;
        copy_tree(from, to, option, ec);
        if (ec)
            BOOST_FILESYSTEM_THROW(boost::filesystem::filesystem_error(
                                   "bunsan::filesystem::copy_tree", from, to, ec));
    }

    void copy_tree(const boost::filesystem::path &from,
                   const boost::filesystem::path &to,
                   const BOOST_SCOPED_ENUM(copy_option) option,
                   boost::system::error_code &ec)
    {
        switch (option)
        {
        case copy_option::fail_if_exists:
            if (boost::filesystem::exists(to))
                ec.assign(EEXIST, boost::system::system_category());
            break;
        case copy_option::overwrite_if_exists:
            boost::filesystem::remove_all(to, ec);
            break;
        }
        if (ec)
            return;
        const boost::filesystem::file_status s(boost::filesystem::symlink_status(from, ec));
        if (ec)
            return;
        boost::filesystem::copy(from, to, ec);
        if (ec)
            return;
        if (is_directory(from, ec) && !ec)
        {
            const boost::filesystem::directory_iterator end;
            for (boost::filesystem::directory_iterator i(from); !ec && i != end; ++i)
                copy_tree(i->path(), to / i->path().filename(), ec);
        }
    }
}}
