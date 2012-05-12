#include "bunsan/tempfile.hpp"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/assert.hpp>

using bunsan::tempfile;
using boost::filesystem::path;

const path &getpath(const tempfile &tmp)
{
    return tmp.path();
}

const path &getpath(const path &path_)
{
    return path_;
}

template <typename T>
bool exists(const T &file)
{
    return boost::filesystem::exists(getpath(file));
}

template <typename T>
void touch(const T &file)
{
    boost::filesystem::ofstream tmp(getpath(file));
}

int main()
{
    boost::filesystem::path tmppath;
    {
        tempfile tmp = tempfile::unique();
        tmppath = tmp.path();
        BOOST_ASSERT(tmp.auto_remove());
        BOOST_ASSERT(!exists(tmp));
        touch(tmp);
        BOOST_ASSERT(exists(tmp));
    }
    BOOST_ASSERT(!exists(tmppath));
    {
        tempfile tmp = tempfile::unique();
        tmppath = tmp.path();
        touch(tmp);
        BOOST_ASSERT(exists(tmp));
        tempfile tmp2;
        BOOST_ASSERT(tmp.auto_remove());
        BOOST_ASSERT(!tmp2.auto_remove());
        swap(tmp, tmp2);
        BOOST_ASSERT(!tmp.auto_remove());
        BOOST_ASSERT(tmp2.auto_remove());
        BOOST_ASSERT(tmp2.path()==tmppath);
    }
    {
        tempfile tmp = tempfile::unique("%%%%-%%%%-%%%%-%%%%.tgz");
        touch(tmp);
        BOOST_ASSERT(tmp.path().extension()==".tgz");
    }
}

