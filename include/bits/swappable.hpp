#pragma once

#include <bits/namespace.hpp>

#include <bits/swap.hpp>
#include <bits/template_constants.hpp>

namespace OZLIB_NAMESPACE {
namespace impl {
namespace adl_swap {
using OZLIB_NAMESPACE::swap;

template <typename T, typename U>
concept swappable_with = requires(T& a, U& b) {
    swap(a, b);
    swap(b, a);
};

template <typename T, typename U>
concept nothrow_swappable_with = swappable_with<T, U> && requires(T& a, U& b) {
    { swap(a, b) } noexcept;
    { swap(b, a) } noexcept;
};
}  // namespace adl_swap

template <typename T, typename U>
struct is_swappable_with : bool_constant<adl_swap::swappable_with<T, U>> {};

template <typename T, typename U>
inline constexpr bool is_swappable_with_v = is_swappable_with<T, U>::value;

template <typename T>
struct is_swappable : is_swappable_with<T, T> {};

template <typename T>
inline constexpr bool is_swappable_v = is_swappable<T>::value;

template <typename T, typename U>
struct is_nothrow_swappable_with : bool_constant<adl_swap::nothrow_swappable_with<T, U>> {};

template <typename T, typename U>
inline constexpr bool is_nothrow_swappable_with_v =
    is_nothrow_swappable_with<T, U>::value;

template <typename T>
struct is_nothrow_swappable : is_nothrow_swappable_with<T, T> {};

template <typename T>
inline constexpr bool is_nothrow_swappable_v = is_nothrow_swappable<T>::value;
}
}
