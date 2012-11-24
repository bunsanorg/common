#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>

#include <string>

#include <boost/optional.hpp>
#include <boost/property_tree/ptree.hpp>

namespace bunsan
{
    namespace error_message
    {
        constexpr const char *invalid_optional = "boost::optional was not initialized";
    }

    // exception overload
    template <typename T, typename E>
    T &get(boost::optional<T> &o, const E &e)
    {
        if (!o)
            throw e;
        return o.get();
    }

    template <typename T, typename E>
    const T &get(const boost::optional<T> &o, const E &e)
    {
        if (!o)
            throw e;
        return o.get();
    }

    // std::string overload
    template <typename T>
    T &get(boost::optional<T> &o, const std::string &msg=error_message::invalid_optional)
    {
        if (!o)
            throw std::invalid_argument(msg);
        return o.get();
    }

    template <typename T>
    const T &get(const boost::optional<T> &o, const std::string &msg=error_message::invalid_optional)
    {
        if (!o)
            throw std::invalid_argument(msg);
        return o.get();
    }

    // const char * overload
    template <typename T>
    T &get(boost::optional<T> &o, const char *msg)
    {
        if (!o)
            throw std::invalid_argument(msg);
        return o.get();
    }

    template <typename T>
    const T &get(const boost::optional<T> &o, const char *msg)
    {
        if (!o)
            throw std::invalid_argument(msg);
        return o.get();
    }

    /*!
     * \brief if node does not exists, set it to value (empty Ptree by default), return node
     */
    template <typename Ptree>
    Ptree &ptree_get(Ptree &ptree, const typename Ptree::path_type &path, const Ptree &value=Ptree())
    {
        return ptree.put_child(path, ptree.get_child(path, value));
    }
}
