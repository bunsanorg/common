#define BOOST_TEST_MODULE asio
#include <boost/test/unit_test.hpp>

#include <bunsan/asio/binary_object_connection.hpp>
#include <bunsan/asio/block_connection.hpp>
#include <bunsan/asio/text_object_connection.hpp>

#include <boost/asio.hpp>
#include <boost/mpl/list.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

BOOST_AUTO_TEST_SUITE(asio)

namespace ba = bunsan::asio;

struct serialization_factory
{
    serialization_factory():
        socket1(io_service),
        socket2(io_service)
    {
        boost::asio::local::connect_pair(socket1, socket2);
    }

    typedef boost::asio::local::stream_protocol::socket socket;
    boost::asio::io_service io_service;
    socket socket1, socket2;
};

BOOST_FIXTURE_TEST_SUITE(serialization, serialization_factory)

BOOST_AUTO_TEST_CASE(block_connection)
{
    std::string bc1_data, bc2_data;
    ba::block_connection<socket> bc1(socket1), bc2(socket2);
    BOOST_CHECK_EQUAL(&bc1.get_io_service(), &io_service);
    BOOST_CHECK_EQUAL(&bc2.get_io_service(), &io_service);
    bc1.async_write("first request",
        [&](const boost::system::error_code &ec)
        {
            BOOST_REQUIRE(!ec);
            bc1.async_read(bc1_data,
                [&](const boost::system::error_code &ec)
                {
                    BOOST_REQUIRE(!ec);
                    BOOST_CHECK_EQUAL(bc1_data, "first response");
                    bc1.async_write("second request",
                        [&](const boost::system::error_code &ec)
                        {
                            BOOST_REQUIRE(!ec);
                            bc1.async_read(bc1_data,
                                [&](const boost::system::error_code &ec)
                                {
                                    BOOST_REQUIRE(!ec);
                                    BOOST_CHECK_EQUAL(
                                        bc1_data,
                                        "second response"
                                    );
                                    bc1.close();
                                });
                        });
                });
        });
    bc2.async_read(bc2_data,
        [&](const boost::system::error_code &ec)
        {
            BOOST_REQUIRE(!ec);
            BOOST_CHECK_EQUAL(bc2_data, "first request");
            bc2.async_write("first response",
                [&](const boost::system::error_code &ec)
                {
                    BOOST_REQUIRE(!ec);
                    bc2.async_read(bc2_data,
                        [&](const boost::system::error_code &ec)
                        {
                            BOOST_REQUIRE(!ec);
                            BOOST_CHECK_EQUAL(bc2_data, "second request");
                            bc2.async_write("second response",
                                [&](const boost::system::error_code &ec)
                                {
                                    BOOST_REQUIRE(!ec);
                                    bc2.async_read(bc2_data,
                                        [&](const boost::system::error_code &ec)
                                        {
                                            BOOST_REQUIRE_EQUAL(
                                                ec,
                                                boost::asio::error::eof
                                            );
                                        });
                                });
                        });
                });
        });
    io_service.run();
}

typedef boost::mpl::list<
            ba::binary_object_connection<serialization_factory::socket>,
            ba::text_object_connection<serialization_factory::socket>
        > object_connections;

template <typename T>
class message
{
public:
    message(): m_data{} {}
    message(const T &data): m_data{data} {}

    message(const message &)=default;
    message(message &&)=default;
    message &operator=(const message &)=default;
    message &operator=(message &&)=default;

    message &operator=(const T &data_)
    {
        m_data = data_;
        return *this;
    }

private:
    friend class boost::serialization::access;

    template <typename P>
    friend std::ostream &operator<<(
        std::ostream &out, const message<P> &msg)
    {
        return out << msg.m_data;
    }

    template <typename P>
    friend bool operator==(const message<T> &msg, const P &data)
    {
        return msg.m_data == data;
    }

    template <typename P>
    friend bool operator==(const P &data, const message<P> &msg)
    {
        return msg.m_data == data;
    }

    template <typename Archive>
    void serialize(Archive &ar, const unsigned int)
    {
        ar & boost::serialization::make_nvp("data", m_data);
    }

    T m_data;
};

template <typename ObjectConnection>
struct session: boost::asio::coroutine
{
    explicit session(serialization_factory::socket &socket_):
        oc(std::make_shared<ObjectConnection>(socket_)),
        msg(std::make_shared<message<int>>()) {}

    std::shared_ptr<ObjectConnection> oc;
    std::shared_ptr<message<int>> msg;
};

template <typename ObjectConnection>
struct server_session: session<ObjectConnection>
{
    using session<ObjectConnection>::session;
    using session<ObjectConnection>::oc;
    using session<ObjectConnection>::msg;

#include <boost/asio/yield.hpp>
    void operator()(
        const boost::system::error_code &ec=
            boost::system::error_code{})
    {
        BOOST_REQUIRE(!ec);
        if (!ec)
        {
            reenter (this)
            {
                *msg = 10;
                yield oc->async_write(*msg, *this);

                yield oc->async_read(*msg, *this);
                BOOST_CHECK_EQUAL(*msg, 100);

                *msg = 20;
                yield oc->async_write(*msg, *this);

                yield oc->async_read(*msg, *this);
                BOOST_CHECK_EQUAL(*msg, 200);

                oc->close();
            }
        }
    }
#include <boost/asio/unyield.hpp>
};

template <typename ObjectConnection>
struct client_session: session<ObjectConnection>
{
    using session<ObjectConnection>::session;
    using session<ObjectConnection>::oc;
    using session<ObjectConnection>::msg;

    bool wait_eof = false;

#include <boost/asio/yield.hpp>
    void operator()(
        const boost::system::error_code &ec=
            boost::system::error_code{})
    {
        if (ec)
        {
            BOOST_CHECK(wait_eof);
            BOOST_CHECK_EQUAL(ec, boost::asio::error::eof);
        }
        else
        {
            reenter (this)
            {
                yield oc->async_read(*msg, *this);
                BOOST_CHECK_EQUAL(*msg, 10);

                *msg = 100;
                yield oc->async_write(*msg, *this);

                yield oc->async_read(*msg, *this);
                BOOST_CHECK_EQUAL(*msg, 20);

                *msg = 200;
                yield oc->async_write(*msg, *this);

                wait_eof = true;
                yield oc->async_read(*msg, *this);
            }
        }
    }
#include <boost/asio/unyield.hpp>
};

BOOST_AUTO_TEST_CASE_TEMPLATE(
    object_connection_coroutine,
    ObjectConnection,
    object_connections)
{
    server_session<ObjectConnection> server(socket1);
    client_session<ObjectConnection> client(socket2);

    BOOST_CHECK_EQUAL(&server.oc->get_io_service(), &io_service);
    BOOST_CHECK_EQUAL(&client.oc->get_io_service(), &io_service);

    server();
    client();

    io_service.run();
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
    object_connection,
    ObjectConnection,
    object_connections)
{
    message<int> oc1_msg, oc2_msg;
    ObjectConnection oc1(socket1), oc2(socket2);
    oc1_msg = 10;
    oc1.async_write(oc1_msg,
        [&](const boost::system::error_code &ec)
        {
            BOOST_REQUIRE(!ec);
            oc1.async_read(oc1_msg,
                [&](const boost::system::error_code &ec)
                {
                    BOOST_REQUIRE(!ec);
                    BOOST_CHECK_EQUAL(oc1_msg, 100);
                    oc1_msg = 20;
                    oc1.async_write(oc1_msg,
                        [&](const boost::system::error_code &ec)
                        {
                            BOOST_REQUIRE(!ec);
                            oc1.async_read(oc1_msg,
                                [&](const boost::system::error_code &ec)
                                {
                                    BOOST_REQUIRE(!ec);
                                    BOOST_CHECK_EQUAL(oc1_msg, 200);
                                    oc1.close();
                                });
                        });
                });
        });
    oc2.async_read(oc2_msg,
        [&](const boost::system::error_code &ec)
        {
            BOOST_REQUIRE(!ec);
            BOOST_CHECK_EQUAL(oc2_msg, 10);
            oc2_msg = 100;
            oc2.async_write(oc2_msg,
                [&](const boost::system::error_code &ec)
                {
                    BOOST_REQUIRE(!ec);
                    oc2.async_read(oc2_msg,
                        [&](const boost::system::error_code &ec)
                        {
                            BOOST_REQUIRE(!ec);
                            BOOST_CHECK_EQUAL(oc2_msg, 20);
                            oc2_msg = 200;
                            oc2.async_write(oc2_msg,
                                [&](const boost::system::error_code &ec)
                                {
                                    BOOST_REQUIRE(!ec);
                                    oc2.async_read(oc2_msg,
                                        [&](const boost::system::error_code &ec)
                                        {
                                            BOOST_REQUIRE_EQUAL(
                                                ec,
                                                boost::asio::error::eof
                                            );
                                        });
                                });
                        });
                });
        });
    io_service.run();
}

BOOST_AUTO_TEST_SUITE_END() // serialization

BOOST_AUTO_TEST_SUITE_END() // asio
