#pragma once

#include <unordered_set>

#include <boost/serialization/collections_save_imp.hpp>
#include <boost/serialization/collections_load_imp.hpp>
#include <boost/serialization/split_free.hpp>

namespace boost{namespace serialization
{
    template <typename Archive, typename Value, typename Hash, typename Pred, typename Alloc>
    void serialize(Archive &ar, std::unordered_set<Value, Hash, Pred, Alloc> &set, const unsigned int version)
    {
        split_free(ar, set, version);
    }

    template <typename Archive, typename Value, typename Hash, typename Pred, typename Alloc>
    void save(Archive &ar, const std::unordered_set<Value, Hash, Pred, Alloc> &set, const unsigned int /*version*/)
    {
        stl::save_collection(ar, set);
    }

    template <typename Archive, typename Value, typename Hash, typename Pred, typename Alloc>
    void load(Archive &ar, std::unordered_set<Value, Hash, Pred, Alloc> &set, const unsigned int /*version*/)
    {
        stl::load_collection<
            Archive,
            std::unordered_set<Value, Hash, Pred, Alloc>,
            stl::archive_input_set<Archive, std::unordered_set<Value, Hash, Pred, Alloc>>,
            stl::no_reserve_imp<std::unordered_set<Value, Hash, Pred, Alloc>>
        >(ar, set);
    }
}}
