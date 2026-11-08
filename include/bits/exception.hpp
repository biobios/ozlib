#pragma once

#include <bits/namespace.hpp>

namespace OZLIB_NAMESPACE {
namespace impl {
class exception {
public:
    constexpr exception() noexcept = default;
    constexpr exception(const exception&) noexcept = default;
    constexpr exception& operator=(const exception&) noexcept = default;
    constexpr virtual const char* what() const noexcept {
        return "exception";
    }
};
}
}