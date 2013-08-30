#include <bunsan/error.hpp>

bunsan::error::error()
{
    enable_stacktrace();
}

bunsan::error::error(const std::string &message_)
{
    enable_stacktrace();
    (*this) << message(message_);
}

const char *bunsan::error::what() const noexcept
{
    return boost::diagnostic_information_what(*this);
}

bunsan::error &bunsan::error::enable_stacktrace(const std::size_t skip)
{
    if (!get<stacktrace>())
        // +1 for enable_stackrace()
        (*this) << stacktrace(runtime::stacktrace::get(skip + 1));
    return *this;
}

bunsan::error &bunsan::error::enable_nested(const boost::exception_ptr &ptr)
{
    (*this) << nested_exception(ptr);
    return *this;
}

bunsan::error &bunsan::error::enable_nested_current()
{
    return enable_nested(boost::current_exception());
}
