#include "bunsan/interprocess/sync/file_lock.hpp"

using namespace bunsan::interprocess;

namespace
{
    file_lock_factory global_file_lock_factory;
}

file_lock_factory &file_lock_factory::instance()
{
    return global_file_lock_factory;
}

