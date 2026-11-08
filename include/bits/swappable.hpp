#pragma once

#include <bits/namespace.hpp>

#include <bits/template_constants.hpp>

namespace OZLIB_NAMESPACE {
namespace impl {
template <typename T, typename U>
struct is_swappable_with : false_type {};

template <typename T, typename U>
    requires requires(T& a, U& b) {
        swap(a, b);
        swap(b, a);
    }
struct is_swappable_with<T, U> : true_type {};

template <typename T, typename U>
inline constexpr bool is_swappable_with_v = is_swappable_with<T, U>::value;

template <typename T>
struct is_swappable : is_swappable_with<T, T> {};

template <typename T>
inline constexpr bool is_swappable_v = is_swappable<T>::value;

template <typename T, typename U>
struct is_nothrow_swappable_with : false_type {};

template <typename T, typename U>
    requires requires(T& a, U& b) {
        {swap(a, b)} noexcept;
        {swap(b, a)} noexcept;
    }
struct is_nothrow_swappable_with<T, U> : true_type {};

template <typename T, typename U>
inline constexpr bool is_nothrow_swappable_with_v =
    is_nothrow_swappable_with<T, U>::value;

template <typename T>
struct is_nothrow_swappable : is_nothrow_swappable_with<T, T> {};

template <typename T>
inline constexpr bool is_nothrow_swappable_v = is_nothrow_swappable<T>::value;
}
}