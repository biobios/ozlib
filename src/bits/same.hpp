#pragma once

#include <bits/template_constants.hpp>

namespace std {
namespace impl {

template <class T, class U>
struct is_same : false_type {};

template <class T>
struct is_same<T, T> : true_type {};

template <class T, class U>
inline constexpr bool is_same_v = is_same<T, U>::value;
}  // namespace impl
}  // namespace std
