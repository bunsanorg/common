#pragma once

#warning "This logging implementation is deprecated!"

#include <bunsan/logging/trivial.hpp>

#define SLOG(M) BUNSAN_LOG_INFO << M

#define DLOG(...) SLOG(#__VA_ARGS__)
