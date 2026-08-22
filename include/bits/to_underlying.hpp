#pragma once

#include <bits/namespace.hpp>

#include <bits/underlying_type.hpp>

namespace OZLIB_NAMESPACE {
namespace impl {
template <typename T>
constexpr underlying_type_t<T> to_underlying(T value) noexcept {
    return static_cast<underlying_type_t<T>>(value);
}
}
}