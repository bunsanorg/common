#define BOOST_TEST_MODULE factory
#include <boost/test/unit_test.hpp>

#include "bunsan/factory.hpp"
#include "bunsan/factory_helper.hpp"

#include <memory>
#include <set>

BOOST_AUTO_TEST_SUITE(factory)

struct check_unknown_factory_error
{
    explicit check_unknown_factory_error(const std::string &type_): type(type_) {}

    template <typename T>
    bool operator()(const T &e)
    {
        const std::string *const ft = e.template get<bunsan::unknown_factory_error::factory_type>();
        return ft && *ft == type;
    }

    std::string type;
};

BOOST_AUTO_TEST_CASE(basic)
{
    typedef std::shared_ptr<int> int_ptr;
    typedef bunsan::factory<int_ptr()> bunsan_factory;
    typename bunsan_factory::map_type *map(0);
    BOOST_CHECK_EXCEPTION(bunsan_factory::instance(map, "strange"), bunsan::unknown_factory_error, check_unknown_factory_error("strange"));
    BOOST_CHECK(!bunsan_factory::instance_optional(map, "strange"));
    BOOST_CHECK(  bunsan_factory::register_new(map, "zero", [](){return int_ptr(new int(0));} )  );
    BOOST_CHECK_NE(map, static_cast<typename bunsan_factory::map_type *>(nullptr));
    BOOST_CHECK(!  bunsan_factory::register_new(map, "zero", [](){return int_ptr(new int(-1));} )  );
    BOOST_CHECK_EXCEPTION(bunsan_factory::factory(map, "strange"), bunsan::unknown_factory_error, check_unknown_factory_error("strange"));
    BOOST_CHECK(!bunsan_factory::factory_optional(map, "strange"));
    const typename bunsan_factory::factory_type zero_fact = bunsan_factory::factory(map, "zero");
    BOOST_CHECK_EQUAL(*zero_fact(), 0);
    BOOST_CHECK_EQUAL(*bunsan_factory::instance(map, "zero"), 0);
    BOOST_CHECK(  bunsan_factory::register_new(map, "one", [](){return int_ptr(new int(1));} )  );
    BOOST_CHECK_EQUAL(*bunsan_factory::instance(map, "zero"), 0);
    BOOST_CHECK_EQUAL(*bunsan_factory::instance(map, "one"), 1);
    BOOST_CHECK_EXCEPTION(bunsan_factory::instance(map, ""), bunsan::unknown_factory_error, check_unknown_factory_error(""));
    BOOST_CHECK(!bunsan_factory::instance_optional(map, ""));
    {
        std::set<std::string> set(bunsan_factory::registered_begin(map), bunsan_factory::registered_end(map));
        std::set<std::string> req = {"zero", "one"};
        BOOST_CHECK(req == set);
    }
}

BOOST_AUTO_TEST_CASE(null)
{
    typedef bunsan::factory<std::shared_ptr<int>()> bunsan_factory;
    typename bunsan_factory::map_type *map(0);
    std::set<bunsan_factory::key_type> set(
        bunsan_factory::registered_begin(map),
        bunsan_factory::registered_end(map));
    BOOST_CHECK(set.empty());
}

BOOST_AUTO_TEST_SUITE(helper)

namespace test
{
    class base
    {
    public:
        virtual std::string g()=0;
    };

    class fact: public base
    BUNSAN_FACTORY_BEGIN(fact, const std::string &, const std::string &)
    public:
        virtual std::string f()=0;
    BUNSAN_FACTORY_END(fact)

    class fact2
    BUNSAN_FACTORY_BEGIN(fact2)
    public:
        virtual int code()=0;
    BUNSAN_FACTORY_END(fact2)
}

namespace test
{
    BUNSAN_FACTORY_DEFINE(fact)
}

BUNSAN_FACTORY_DEFINE(test::fact2)

namespace test
{
    namespace facts
    {
        class init: public test::fact
        {
        public:
            explicit init(const std::string data_, const std::string &data2_):
                m_data(data_), m_data2(data2_) {}

            virtual std::string f()
            {
                return m_data + m_data2;
            }

            virtual std::string g()
            {
                return m_data2 + m_data;
            }

        private:
            std::string m_data, m_data2;
            static const bool factory_reg_hook;
        };

        const bool init::factory_reg_hook = test::fact::register_new("init",
            [](const std::string &data_, const std::string data2_)
            {
                fact_ptr ptr(new init(data_, data2_));
                return ptr;
            });
    }

    namespace facts2
    {
        class init: public test::fact2
        {
        public:
            init() {}

            virtual int code()
            {
                return 123;
            }

        private:
            static const bool factory_reg_hook;
        };

        const bool init::factory_reg_hook = test::fact2::register_new("init",
            []()
            {
                fact2_ptr ptr(new init);
                return ptr;
            });
    }
}

BOOST_AUTO_TEST_CASE(all)
{
    {
        {
            const std::set<std::string> set(test::fact::registered_begin(), test::fact::registered_end());
            const std::set<std::string> req = {"init"};
            BOOST_CHECK(req == set);
        }
        BOOST_CHECK_EXCEPTION(test::fact::factory("not_existing_name"), bunsan::unknown_factory_error, check_unknown_factory_error("not_existing_name"));
        BOOST_CHECK(!test::fact::factory_optional("not_existing_name"));
        {
            const test::fact::factory_type fact = test::fact::factory("init");
            const test::fact_ptr ptr = fact("1", "2");
            BOOST_CHECK(ptr->f() == "12");
            BOOST_CHECK(ptr->g() == "21");
        }
        const test::fact_ptr ptr = test::fact::instance("init", "hello,", " world");
        BOOST_REQUIRE(ptr);
        BOOST_CHECK(ptr->f() == "hello, world");
        BOOST_CHECK(ptr->g() == " worldhello,");
        BOOST_CHECK_EXCEPTION(test::fact::instance("noinit", "hello,", " linux"), bunsan::unknown_factory_error, check_unknown_factory_error("noinit"));
        BOOST_CHECK(!test::fact::instance_optional("noinit", "hello,", " linux"));
    }
    {
        {
            const std::set<std::string> set(test::fact2::registered_begin(), test::fact2::registered_end());
            const std::set<std::string> req = {"init"};
            BOOST_CHECK(req == set);
        }
        const test::fact2_ptr ptr = test::fact2::instance("init");
        BOOST_REQUIRE(ptr);
        BOOST_CHECK(ptr->code() == 123);
        BOOST_CHECK_EXCEPTION(test::fact2::instance("noinit"), bunsan::unknown_factory_error, check_unknown_factory_error("noinit"));
        BOOST_CHECK(!test::fact2::instance_optional("noinit"));
    }
}

BOOST_AUTO_TEST_SUITE_END() // helper

BOOST_AUTO_TEST_SUITE_END() // factory
