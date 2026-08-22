#pragma once

#include <bits/namespace.hpp>

namespace OZLIB_NAMESPACE {
namespace impl {
template <typename T>
constexpr T* launder(T* p) noexcept {
    return __builtin_launder(p);
}
}
}