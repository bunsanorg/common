#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>

#include <boost/date_time/posix_time/posix_time.hpp>

#define SLOG(M) do\
        {\
            std::stringstream buf;\
            buf<<"["<<__FILE__<<":"<<__LINE__<<":"<<__func__<<":"<<boost::posix_time::second_clock::local_time()<<"] "<<M<<std::endl;\
            std::cerr<<buf.str()<<std::flush;\
        } while (false)
#define DLOG(...) SLOG(#__VA_ARGS__)

#include <locale>
#include <string>

#include <boost/optional.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>

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
     * \brief recreate directory
     */
    void reset_dir(const boost::filesystem::path &dir);

    /*!
     * \brief read info from filename with relative path bug fix
     *
     * it is not safe function, it should be used only from single thread
     */
    template <typename Ptree>
    void read_info(const std::string &filename, Ptree &pt, const std::locale &loc = std::locale())
    {
        boost::filesystem::initial_path();
        boost::filesystem::current_path(boost::filesystem::absolute(boost::filesystem::path(filename)).parent_path());
        boost::property_tree::info_parser::read_info(boost::filesystem::path(filename).filename().string(), pt, loc);
        boost::filesystem::current_path(boost::filesystem::initial_path());
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
