#pragma once

#include <locale>
#include <string>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>

namespace bunsan{namespace property_tree
{
    /*!
     * \brief Read info from filename with relative path bug fix
     *
     * \warning This function is not safe, it should be used only from single thread.
     */
    template <typename Ptree>
    void read_info(const std::string &filename, Ptree &pt, const std::locale &loc = std::locale())
    {
        boost::filesystem::initial_path();
        boost::filesystem::current_path(boost::filesystem::absolute(boost::filesystem::path(filename)).parent_path());
        boost::property_tree::info_parser::read_info(boost::filesystem::path(filename).filename().string(), pt, loc);
        boost::filesystem::current_path(boost::filesystem::initial_path());
    }
}}
