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
        // +1 for stacktrace::get()
        (*this) << stacktrace(runtime::stacktrace::get(skip + 2));
    return *this;
}
