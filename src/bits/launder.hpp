#pragma once

namespace std {
namespace impl {
template <typename T>
constexpr T* launder(T* p) noexcept {
    return __builtin_launder(p);
}
}
}