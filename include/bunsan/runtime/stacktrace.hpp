#pragma once

#include <iostream>
#include <vector>

namespace bunsan{namespace runtime
{
    class stacktrace: public std::vector<void *>
    {
    public:
        stacktrace()=default;
        stacktrace(const stacktrace &)=default;
        stacktrace &operator=(const stacktrace &)=default;
        stacktrace(stacktrace &&)=default;
        stacktrace &operator=(stacktrace &&)=default;

    public:
        /*!
         * \brief Get stack trace.
         *
         * \param skip skip first entries (1 means stacktrace::get() will not be included into trace)
         * \param max_size maximum stack trace size
         */
        static stacktrace get(const std::size_t skip=1, const std::size_t max_size=1024);

    private:
        friend std::ostream &operator<<(std::ostream &out, const stacktrace &trace);
    };

    std::ostream &operator<<(std::ostream &out, const stacktrace &trace);
}}
