#pragma once

#include <bits/is_lvalue_reference.hpp>
#include <bits/template_constants.hpp>

namespace std {
namespace impl {

template <class T>
struct is_rvalue_reference : false_type {};

template <class T>
struct is_rvalue_reference<T&&> : true_type {};

template <class T>
inline constexpr bool is_rvalue_reference_v = is_rvalue_reference<T>::value;

template <class T>
struct is_reference
    : bool_constant<is_lvalue_reference_v<T> || is_rvalue_reference_v<T>> {};

template <class T>
inline constexpr bool is_reference_v = is_reference<T>::value;

}  // namespace impl

}  // namespace std
