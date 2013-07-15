#pragma once

#include "bunsan/filesystem/error.hpp"

#include <boost/filesystem/fstream.hpp>

namespace bunsan{namespace filesystem
{
    template <typename ... Args>
    using basic_filebuf = boost::filesystem::basic_filebuf<Args...>;

    template <typename Fstream>
    class basic_stream: public Fstream
    {
    public:
        basic_stream()
        {
            this->exceptions(std::ios_base::badbit);
        }

        explicit basic_stream(const boost::filesystem::path &path):
            Fstream(path)
        {
            if (!this->is_open())
                BOOST_THROW_EXCEPTION(system_error("open") << error::path(path));
            this->exceptions(std::ios_base::badbit);
        }

        basic_stream(const boost::filesystem::path &path, const std::ios_base::openmode mode):
            Fstream(path, mode)
        {
            if (!this->is_open())
                BOOST_THROW_EXCEPTION(system_error("open") << error::path(path) << error::openmode(mode));
            this->exceptions(std::ios_base::badbit);
        }

        void open(const boost::filesystem::path &path)
        {
            Fstream::open(path);
            if (!this->is_open())
                BOOST_THROW_EXCEPTION(system_error("open") << error::path(path));
        }

        void open(const boost::filesystem::path &path, const std::ios_base::openmode mode)
        {
            Fstream::open(path, mode);
            if (!this->is_open())
                BOOST_THROW_EXCEPTION(system_error("open") << error::path(path) << error::openmode(mode));
        }

        using Fstream::exceptions;

        void exceptions(const std::ios_base::iostate except)
        {
            Fstream::exceptions(except | std::ios_base::badbit);
        }
    };

    template <typename CharT, typename Traits=std::char_traits<CharT>>
    using basic_ifstream = basic_stream<boost::filesystem::basic_ifstream<CharT, Traits>>;

    template <typename CharT, typename Traits=std::char_traits<CharT>>
    using basic_ofstream = basic_stream<boost::filesystem::basic_ofstream<CharT, Traits>>;

    template <typename CharT, typename Traits=std::char_traits<CharT>>
    using basic_fstream = basic_stream<boost::filesystem::basic_fstream<CharT, Traits>>;

    typedef basic_filebuf<char> filebuf;
    typedef basic_ifstream<char> ifstream;
    typedef basic_ofstream<char> ofstream;
    typedef basic_fstream<char> fstream;

    typedef basic_filebuf<wchar_t> wfilebuf;
    typedef basic_ifstream<wchar_t> wifstream;
    typedef basic_ofstream<wchar_t> wofstream;
    typedef basic_fstream<wchar_t> wfstream;
}}
