#pragma once

#include <cstdlib>

namespace std {
namespace ozlib_exception_helper {

#if defined(__EXCEPTIONS) || defined(__cpp_exceptions)

[[noreturn]] inline void throw_out_of_range(const char* msg) {
    // throw std::out_of_range(msg);
}

#else

[[noreturn]] inline void throw_out_of_range(const char* msg) { std::abort(); }

#endif
}  // namespace ozlib_exception_helper
}  // namespace std
