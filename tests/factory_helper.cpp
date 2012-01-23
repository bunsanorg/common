#include "bunsan/factory_helper.hpp"

#include <boost/assert.hpp>

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
				return m_data+m_data2;
			}
			virtual std::string g()
			{
				return m_data2+m_data;
			}
		private:
			std::string m_data, m_data2;
			static bool factory_reg_hook;
		};
		bool init::factory_reg_hook = test::fact::register_new("init", []
			(const std::string &data_, const std::string data2_)
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
			static bool factory_reg_hook;
		};
		bool init::factory_reg_hook = test::fact2::register_new("init", []()
			{
				fact2_ptr ptr(new init);
				return ptr;
			});
	}
}

int main()
{
	{
		test::fact_ptr ptr = test::fact::instance("init", "hello,", " world");
		BOOST_ASSERT(ptr);
		BOOST_ASSERT(ptr->f()=="hello, world");
		BOOST_ASSERT(ptr->g()==" worldhello,");
		ptr = test::fact::instance("noinit", "hello,", " linux");
		BOOST_ASSERT(!ptr);
	}
	{
		test::fact2_ptr ptr = test::fact2::instance("init");
		BOOST_ASSERT(ptr);
		BOOST_ASSERT(ptr->code()==123);
		ptr = test::fact2::instance("noinit");
		BOOST_ASSERT(!ptr);
	}
}

