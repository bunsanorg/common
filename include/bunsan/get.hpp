#pragma once

#include "bunsan/error.hpp"

#include <iostream>
#include <sstream>
#include <stdexcept>

#include <string>

#include <boost/optional.hpp>
#include <boost/property_tree/ptree.hpp>

namespace bunsan
{
    struct uninitialized_optional_error: virtual error {};

    // std::string overload
    template <typename T, typename Error=uninitialized_optional_error>
    T &get(boost::optional<T> &o, const std::string &msg)
    {
        if (!o)
            BOOST_THROW_EXCEPTION(Error() << error::message(msg));
        return o.get();
    }

    template <typename T, typename Error=uninitialized_optional_error>
    T &get(boost::optional<T> &o)
    {
        if (!o)
            BOOST_THROW_EXCEPTION(Error());
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
