#pragma once

#warning "This logging implementation is deprecated!"

#include <boost/date_time/posix_time/posix_time.hpp>

#include <iostream>
#include <sstream>
#include <stdexcept>

#define SLOG(M) do \
        { \
            std::stringstream buf; \
            buf << "[" << __FILE__ << ":" << __LINE__ << ":" << __func__ << ":" << \
                boost::posix_time::second_clock::local_time() << "] " << M << std::endl; \
            std::cerr << buf.str() << std::flush; \
        } \
        while (false)
#define DLOG(...) SLOG(#__VA_ARGS__)
