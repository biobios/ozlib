#pragma once

#include <bits/template_constants.hpp>

namespace std {
namespace impl {
template <class T>
struct is_lvalue_reference : false_type {};

template <class T>
struct is_lvalue_reference<T&> : true_type {};

template <class T>
inline constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;
}  // namespace impl
}  // namespace std