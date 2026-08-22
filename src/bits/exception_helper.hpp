#pragma once

#include <bits/namespace.hpp>

#include <cstdlib>

namespace OZLIB_NAMESPACE {
namespace ozlib_exception_helper {

#if defined(__EXCEPTIONS) || defined(__cpp_exceptions)

[[noreturn]] inline void throw_out_of_range(const char* msg) {
    // throw OZLIB_NAMESPACE::out_of_range(msg);
}

#else

[[noreturn]] inline void throw_out_of_range(const char* msg) { OZLIB_NAMESPACE::abort(); }

#endif
}  // namespace ozlib_exception_helper
}  // namespace OZLIB_NAMESPACE
