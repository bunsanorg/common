#pragma once

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

#include <deque>
#include <string>

namespace bunsan{namespace asio
{
    template <typename Source, typename Sink>
    class buffer_connection
    {
    public:
        typedef boost::function<
            void (boost::system::error_code, std::size_t)
        > handler;

    public:
        buffer_connection(
            Source &source,
            Sink &sink,
            const handler &handle_read,
            const handler &handle_write):
                m_strand(source.get_io_service()),
                m_handle_read(handle_read),
                m_handle_write(handle_write),
                m_source(source),
                m_sink(sink)
        {}

        buffer_connection(Source &source, Sink &sink):
            buffer_connection(source, sink, handler{}, handler{}) {}

        bool close_sink_on_eof() const { return m_close_sink_on_eof; }

        /// \warning not thread safe, must be executed before start()
        void set_close_sink_on_eof(const bool value=true)
        {
            m_close_sink_on_eof = value;
        }

        void start()
        {
            m_strand.dispatch(boost::bind(
                &buffer_connection::spawn_reader,
                this
            ));
        }

        boost::asio::io_service &get_io_service()
        {
            return m_source.get_io_service();
        }

        void close()
        {
            m_strand.dispatch(boost::bind(
                &buffer_connection::close_,
                this
            ));
        }

        void terminate()
        {
            m_strand.post(boost::bind(
                &buffer_connection::terminate_,
                this
            ));
        }

    private:
        void close_()
        {
            if (m_last)
                return;

            m_last = true;
            if (m_queue.empty())
                m_sink.close();
        }

        void terminate_()
        {
            m_last = true;
            m_terminated = true;
            m_source.close();
            m_sink.close();
        }

        void spawn_reader()
        {
            if (m_terminated)
                return;

            m_source.async_read_some(
                boost::asio::buffer(m_inbound_data),
                m_strand.wrap(boost::bind(
                    &buffer_connection::handle_read,
                    this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred
                ))
            );
        }

        void spawn_writer()
        {
            if (m_terminated)
                return;

            const std::vector<char> &buffer = m_queue.front();
            boost::asio::async_write(
                m_sink,
                boost::asio::buffer(buffer),
                m_strand.wrap(boost::bind(
                    &buffer_connection::handle_write,
                    this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred
                ))
            );
        }

        void handle_read(
            const boost::system::error_code &ec,
            const std::size_t size)
        {
            if (m_handle_read)
                m_handle_read(ec, size);

            m_queue.emplace_back(
                m_inbound_data,
                m_inbound_data + size
            );

            if (m_queue.size() == 1)
            { // queue was empty before, writer is required
                spawn_writer();
            }

            if (ec || m_last)
                m_last = true;
            else
                spawn_reader();
        }

        void handle_write(
            const boost::system::error_code &ec,
            const std::size_t size)
        {
            if (m_handle_write)
                m_handle_write(ec, size);

            if (ec)
                return;

            m_queue.pop_front();
            if (m_queue.empty())
            {
                if (m_last && m_close_sink_on_eof)
                    m_sink.close();
            }
            else
            {
                spawn_writer();
            }
        }

    private:
        boost::asio::io_service::strand m_strand;
        handler m_handle_read;
        handler m_handle_write;
        Source &m_source;
        Sink &m_sink;

        char m_inbound_data[4096];
        std::deque<std::vector<char>> m_queue;
        bool m_last = false;
        bool m_terminated = false;

        bool m_close_sink_on_eof = true;
    };
}}
