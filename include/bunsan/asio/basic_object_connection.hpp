#pragma once

#include <bunsan/asio/block_connection.hpp>

#include <boost/asio.hpp>

#include <sstream>

namespace bunsan{namespace asio
{
    template <typename IArchive, typename OArchive, typename Connection>
    class basic_object_connection
    {
    public:
        explicit basic_object_connection(Connection &connection):
            m_block_connection(connection) {}

        template <typename T, typename Handler>
        void async_write(const T &obj, Handler handler)
        {
            std::ostringstream sout;
            {
                OArchive oa(sout);
                oa << obj;
            }
            m_block_connection.async_write(sout.str(), handler);
        }

        template <typename T, typename Handler>
        void async_read(T &obj, Handler handler)
        {
            m_block_connection.async_read(data_,
                [this, &obj, handler](const boost::system::error_code &ec)
                {
                    handle_read(ec, obj, handler);
                });
        }

        boost::asio::io_service &get_io_service()
        {
            return m_block_connection.get_io_service();
        }

        void close()
        {
            m_block_connection.close();
        }

    private:
        template <typename T, typename Handler>
        void handle_read(
            const boost::system::error_code &ec,
            T &obj,
            Handler handler)
        {
            if (ec)
            {
                handler(ec);
            }
            else
            {
                std::istringstream sin(data_);
                try
                {
                    IArchive ia(sin);
                    ia >> obj;
                }
                catch (std::exception &)
                {
                    boost::system::error_code error(
                        boost::asio::error::invalid_argument);
                    handler(ec);
                    return;
                }
                handler(ec);
            }
        }

    private:
        block_connection<Connection> m_block_connection;
        std::string data_;
    };
}}
