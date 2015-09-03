#define BOOST_TEST_MODULE log
#include <boost/test/unit_test.hpp>

#include <bunsan/log/trivial.hpp>

#include <thread>

BOOST_AUTO_TEST_SUITE(log_test)

void spam_logs(const char *const msg, const int count) {
  for (int i = 0; i < count; ++i) {
    BUNSAN_LOG_INFO << msg;
  }
}

BOOST_AUTO_TEST_CASE(thread_safety) {
  constexpr int count = 100 * 1000;
  std::thread t(spam_logs, "Hello", count);
  spam_logs("World", count);
  t.join();
}

BOOST_AUTO_TEST_SUITE_END()  // log_test
