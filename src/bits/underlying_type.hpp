#pragma once

#include <bits/fundamental_traits.hpp>

namespace std {
namespace impl {
template <typename T>
struct underlying_type {};

template <typename T>
    requires is_enum_v<T>
struct underlying_type<T> {
    using type = __underlying_type(T);
};

template <typename T>
using underlying_type_t = typename underlying_type<T>::type;
}
}