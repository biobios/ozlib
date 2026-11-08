#pragma once

#include <bits/namespace.hpp>

#include <cstdlib>

namespace OZLIB_NAMESPACE {
namespace ozlib_exception_helper {

#if defined(__EXCEPTIONS) || defined(__cpp_exceptions)

[[noreturn]] inline void throw_out_of_range(const char* msg) {
    // throw OZLIB_NAMESPACE::out_of_range(msg);
}

template <typename T>
[[noreturn]] inline void throw_exception_of_type(const char* msg) {
    throw T(msg);
}

template <typename T>
[[noreturn]] inline void throw_exception_of_type() {
    throw T();
}

#else

[[noreturn]] inline void throw_out_of_range(const char* msg) { OZLIB_NAMESPACE::abort(); }

template <typename T>
[[noreturn]] inline void throw_exception_of_type(const char* msg) { OZLIB_NAMESPACE::abort(); }

template <typename T>
[[noreturn]] inline void throw_exception_of_type() { OZLIB_NAMESPACE::abort(); }

#endif
}  // namespace ozlib_exception_helper
}  // namespace OZLIB_NAMESPACE
