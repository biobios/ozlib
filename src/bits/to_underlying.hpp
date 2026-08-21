#pragma once

#include <bits/underlying_type.hpp>

namespace std {
namespace impl {
template <typename T>
constexpr underlying_type_t<T> to_underlying(T value) noexcept {
    return static_cast<underlying_type_t<T>>(value);
}
}
}