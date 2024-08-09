#pragma once

#include <bits/template_constants.hpp>

namespace std {
namespace impl {

template <typename T>
struct is_const : false_type {};

template <typename T>
struct is_const<const T> : true_type {};

template <typename T>
inline constexpr bool is_const_v = is_const<T>::value;

}  // namespace impl
}  // namespace std
