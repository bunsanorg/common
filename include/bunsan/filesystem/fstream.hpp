#pragma once

#include "bunsan/filesystem/error.hpp"

#include <boost/filesystem/fstream.hpp>

namespace bunsan{namespace filesystem
{
    template <typename ... Args>
    using basic_filebuf = boost::filesystem::basic_filebuf<Args...>;

/// File stream with exceptions enabled.
#define BUNSAN_FILESYSTEM_BASIC_FSTREAM(FSTREAM) \
    template <typename CharT, typename Traits=std::char_traits<CharT>> \
    class basic_##FSTREAM: public boost::filesystem::basic_##FSTREAM<CharT, Traits> \
    { \
    public: \
        basic_##FSTREAM() \
        { \
            this->exceptions(std::ios_base::badbit); \
        } \
         \
        explicit basic_##FSTREAM(const boost::filesystem::path &path): \
            boost::filesystem::basic_##FSTREAM<CharT, Traits>(path) \
        { \
            if (!this->is_open()) \
                BOOST_THROW_EXCEPTION(system_error("open") << error::path(path)); \
            this->exceptions(std::ios_base::badbit); \
        } \
         \
        basic_##FSTREAM(const boost::filesystem::path &path, const std::ios_base::openmode mode): \
            boost::filesystem::basic_##FSTREAM<CharT, Traits>(path, mode) \
        { \
            if (!this->is_open()) \
                BOOST_THROW_EXCEPTION(system_error("open") << error::path(path) << error::openmode(mode)); \
            this->exceptions(std::ios_base::badbit); \
        } \
         \
        void open(const boost::filesystem::path &path) \
        { \
            boost::filesystem::basic_##FSTREAM<CharT, Traits>::open(path); \
            if (!this->is_open()) \
                BOOST_THROW_EXCEPTION(system_error("open") << error::path(path)); \
        } \
         \
        void open(const boost::filesystem::path &path, const std::ios_base::openmode mode) \
        { \
            boost::filesystem::basic_##FSTREAM<CharT, Traits>::open(path, mode); \
            if (!this->is_open()) \
                BOOST_THROW_EXCEPTION(system_error("open") << error::path(path) << error::openmode(mode)); \
        } \
         \
        using boost::filesystem::basic_##FSTREAM<CharT, Traits>::exceptions; \
         \
        void exceptions(const std::ios_base::iostate except) \
        { \
            boost::filesystem::basic_##FSTREAM<CharT, Traits>::exceptions(except | std::ios_base::badbit); \
        } \
    };

    BUNSAN_FILESYSTEM_BASIC_FSTREAM(ifstream)
    BUNSAN_FILESYSTEM_BASIC_FSTREAM(ofstream)
    BUNSAN_FILESYSTEM_BASIC_FSTREAM(fstream)

    typedef basic_filebuf<char> filebuf;
    typedef basic_ifstream<char> ifstream;
    typedef basic_ofstream<char> ofstream;
    typedef basic_fstream<char> fstream;

    typedef basic_filebuf<wchar_t> wfilebuf;
    typedef basic_ifstream<wchar_t> wifstream;
    typedef basic_ofstream<wchar_t> wofstream;
    typedef basic_fstream<wchar_t> wfstream;
}}
