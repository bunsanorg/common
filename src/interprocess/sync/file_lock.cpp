#include "bunsan/interprocess/sync/file_lock.hpp"

#include <boost/thread/locks.hpp>
#include <boost/assert.hpp>

using namespace bunsan::interprocess;

namespace
{
    file_lock_factory global_file_lock_factory;
}

file_lock_factory &file_lock_factory::instance()
{
    return global_file_lock_factory;
}

const boost::filesystem::path &file_lock_factory::get_path_from_pointer(const mutex_ptr &mtx)
{
    BOOST_ASSERT(mtx);
    return mtx->path;
}

bool file_lock_factory::try_find_mutex(const boost::filesystem::path &path, mutex_ptr &mtx)
{
    index_path_type &by_path = m_instances.get<tag_path>();
    index_path_type::iterator it = by_path.find(path);
    if (it!=by_path.end())
    {
        mtx = *it;
        return true;
    }
    else
        return false;
}

file_lock file_lock_factory::get(const boost::filesystem::path &path)
{
    {
        boost::shared_lock<boost::shared_mutex> lk(m_lock);
        mutex_ptr mtx;
        if (try_find_mutex(path, mtx))
            return file_lock(this, mtx);
    }
    boost::unique_lock<boost::shared_mutex> lk(m_lock);
    mutex_ptr mtx;
    if (!try_find_mutex(path, mtx))
    {
        mtx.reset(new mutex);
        mtx->path = path;
        m_instances.insert(mtx);
    }
    return file_lock(this, mtx);
}

