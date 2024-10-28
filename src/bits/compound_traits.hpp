#pragma once

#include <bits/fundamental_traits.hpp>

namespace std {
namespace impl {

template <class T>
struct is_reference
    : bool_constant<is_lvalue_reference_v<T> || is_rvalue_reference_v<T>> {};

template <class T>
inline constexpr bool is_reference_v = is_reference<T>::value;

template <class T>
struct is_arithmetic
    : bool_constant<is_integral_v<T> || is_floating_point_v<T>> {};

template <class T>
inline constexpr bool is_arithmetic_v = is_arithmetic<T>::value;

template <class T>
struct is_fundamental : bool_constant<is_arithmetic_v<T> || is_void_v<T> ||
                                      is_null_pointer_v<T>> {};

template <class T>
inline constexpr bool is_fundamental_v = is_fundamental<T>::value;

template <class T>
struct is_object
    : bool_constant<!is_function_v<T> && !is_reference_v<T> && !is_void_v<T>> {
};

template <class T>
inline constexpr bool is_object_v = is_object<T>::value;

template <class T>
struct is_member_pointer : bool_constant<is_member_object_pointer_v<T> ||
                                         is_member_function_pointer_v<T>> {};

template <class T>
inline constexpr bool is_member_pointer_v = is_member_pointer<T>::value;

template <class T>
struct is_scalar
    : bool_constant<is_arithmetic_v<T> || is_enum_v<T> || is_pointer_v<T> ||
                    is_member_pointer_v<T> || is_null_pointer_v<T>> {};

template <class T>
inline constexpr bool is_scalar_v = is_scalar<T>::value;

template <class T>
struct is_array
    : bool_constant<is_bounded_array_v<T> || is_unbounded_array_v<T>> {};

template <class T>
inline constexpr bool is_array_v = is_array<T>::value;

template <class T>
struct is_compound
    : bool_constant<is_array_v<T> || is_function_v<T> || is_pointer_v<T> ||
                    is_reference_v<T> || is_class_v<T> || is_union_v<T> ||
                    is_enum_v<T> || is_member_pointer_v<T>> {};

}  // namespace impl
}  // namespace std
