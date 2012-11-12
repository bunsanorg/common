#pragma once

#include <unordered_map>

#include <boost/serialization/collections_save_imp.hpp>
#include <boost/serialization/collections_load_imp.hpp>
#include <boost/serialization/split_free.hpp>

namespace boost{namespace serialization
{
    template <typename Archive, typename Key, typename Tp, typename Hash, typename Pred, typename Alloc>
    void serialize(Archive &ar, std::unordered_map<Key, Tp, Hash, Pred, Alloc> &map, const unsigned int version)
    {
        split_free(ar, map, version);
    }

    template <typename Archive, typename Key, typename Tp, typename Hash, typename Pred, typename Alloc>
    void save(Archive &ar, const std::unordered_map<Key, Tp, Hash, Pred, Alloc> &map, const unsigned int /*version*/)
    {
        stl::save_collection(ar, map);
    }

    template <typename Archive, typename Key, typename Tp, typename Hash, typename Pred, typename Alloc>
    void load(Archive &ar, std::unordered_map<Key, Tp, Hash, Pred, Alloc> &map, const unsigned int /*version*/)
    {
        stl::load_collection<
            Archive,
            std::unordered_map<Key, Tp, Hash, Pred, Alloc>,
            stl::archive_input_map<Archive, std::unordered_map<Key, Tp, Hash, Pred, Alloc>>,
            stl::no_reserve_imp<std::unordered_map<Key, Tp, Hash, Pred, Alloc>>
        >(ar, map);
    }
}}
