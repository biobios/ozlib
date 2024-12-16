#pragma once

#include <bits/compound_traits.hpp>
#include <bits/template_constants.hpp>

namespace std {
namespace impl {

template <typename>
struct is_signed : false_type {};

template <typename T>
    requires is_arithmetic_v<T>
struct is_signed<T> : bool_constant<T(-1) < T(0)> {};

template <typename T>
inline constexpr bool is_signed_v = is_signed<T>::value;

template <typename>
struct is_unsigned : false_type {};

template <typename T>
    requires is_arithmetic_v<T>
struct is_unsigned<T> : bool_constant<T(0) < T(-1)> {};

template <typename T>
inline constexpr bool is_unsigned_v = is_unsigned<T>::value;

// concepts
template <typename T>
concept signed_integral = integral<T> && is_signed_v<T>;

template <typename T>
concept unsigned_integral = integral<T> && is_unsigned_v<T>;

}  // namespace impl
}  // namespace std