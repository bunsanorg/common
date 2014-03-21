#define BOOST_TEST_MODULE asio
#include <boost/test/unit_test.hpp>

#include <bunsan/asio/block_connection.hpp>

#include <boost/asio.hpp>

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

BOOST_AUTO_TEST_SUITE_END() // serialization

BOOST_AUTO_TEST_SUITE_END() // asio
