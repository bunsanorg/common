#pragma once

#include <bunsan/filesystem/error.hpp>

#include <boost/filesystem/fstream.hpp>

namespace bunsan{namespace filesystem
{
    template <typename ... Args>
    using basic_filebuf = boost::filesystem::basic_filebuf<Args...>;

    template <typename Fstream, std::ios_base::openmode BaseOpenmode>
    class basic_stream: public Fstream
    {
    public:
        basic_stream()
        {
            this->exceptions(std::ios_base::badbit);
        }

        explicit basic_stream(const boost::filesystem::path &path):
            Fstream(path),
            m_path(path),
            m_openmode(BaseOpenmode)
        {
            if (!this->is_open())
                BOOST_THROW_EXCEPTION(system_error("open") <<
                                      error::path(m_path) <<
                                      error::openmode(m_openmode));
            this->exceptions(std::ios_base::badbit);
        }

        basic_stream(const boost::filesystem::path &path,
                     const std::ios_base::openmode mode):
            Fstream(path, mode),
            m_path(path),
            m_openmode(mode | BaseOpenmode)
        {
            if (!this->is_open())
                BOOST_THROW_EXCEPTION(system_error("open") <<
                                      error::path(m_path) <<
                                      error::openmode(m_openmode));
            this->exceptions(std::ios_base::badbit);
        }

        void open(const boost::filesystem::path &path)
        {
            m_path = path;
            m_openmode = BaseOpenmode;
            Fstream::open(m_path);
            if (!this->is_open())
                BOOST_THROW_EXCEPTION(system_error("open") <<
                                      error::path(m_path) <<
                                      error::openmode(m_openmode));
        }

        void open(const boost::filesystem::path &path,
                  const std::ios_base::openmode mode)
        {
            m_path = path;
            m_openmode = mode | BaseOpenmode;
            Fstream::open(m_path, m_openmode);
            if (!this->is_open())
                BOOST_THROW_EXCEPTION(system_error("open") <<
                                      error::path(m_path) <<
                                      error::openmode(m_openmode));
        }

        using Fstream::exceptions;

        void exceptions(const std::ios_base::iostate except)
        {
            Fstream::exceptions(except | std::ios_base::badbit);
        }

        void close()
        {
            Fstream::close();
            if (this->fail())
                BOOST_THROW_EXCEPTION(system_error("close") <<
                                      error::path(m_path) <<
                                      error::openmode(m_openmode));
        }

    private:
        boost::filesystem::path m_path;
        std::ios_base::openmode m_openmode = BaseOpenmode;
    };

    template <typename CharT, typename Traits=std::char_traits<CharT>>
    using basic_ifstream = basic_stream<boost::filesystem::basic_ifstream<CharT, Traits>, std::ios_base::in>;

    template <typename CharT, typename Traits=std::char_traits<CharT>>
    using basic_ofstream = basic_stream<boost::filesystem::basic_ofstream<CharT, Traits>, std::ios_base::out>;

    template <typename CharT, typename Traits=std::char_traits<CharT>>
    using basic_fstream = basic_stream<boost::filesystem::basic_fstream<CharT, Traits>, std::ios_base::openmode{}>;

    typedef basic_filebuf<char> filebuf;
    typedef basic_ifstream<char> ifstream;
    typedef basic_ofstream<char> ofstream;
    typedef basic_fstream<char> fstream;

    typedef basic_filebuf<wchar_t> wfilebuf;
    typedef basic_ifstream<wchar_t> wifstream;
    typedef basic_ofstream<wchar_t> wofstream;
    typedef basic_fstream<wchar_t> wfstream;
}}
