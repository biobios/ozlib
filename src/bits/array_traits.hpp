#pragma once

#include <bits/size_t.hpp>
#include <bits/template_constants.hpp>

namespace std {
namespace impl {

template <typename>
struct is_bounded_array : false_type {};

template <typename T, size_t size>
struct is_bounded_array<T[size]> : true_type {};

template <typename T>
inline constexpr bool is_bounded_array_v = is_bounded_array<T>::value;

template <typename>
struct is_unbounded_array : false_type {};

template <typename T>
struct is_unbounded_array<T[]> : true_type {};

template <typename T>
inline constexpr bool is_unbounded_array_v = is_unbounded_array<T>::value;

template <typename>
struct is_array : false_type {};

template <typename T>
    requires is_bounded_array_v<T> || is_unbounded_array_v<T>
struct is_array<T> : true_type {};

template <typename T>
inline constexpr bool is_array_v = is_array<T>::value;

template <typename T>
struct remove_extent {
    using type = T;
};

template <typename U, size_t N>
struct remove_extent<U[N]> {
    using type = U;
};

template <typename U>
struct remove_extent<U[]> {
    using type = U;
};

template <typename T>
using remove_extent_t = typename remove_extent<T>::type;

template <class T>
struct remove_all_extents {
    using type = T;
};

template <class T>
struct remove_all_extents<T[]> : remove_all_extents<T> {};

template <class T, size_t N>
struct remove_all_extents<T[N]> : remove_all_extents<T> {};

template <class T>
using remove_all_extents_t = typename remove_all_extents<T>::type;

}  // namespace impl
}  // namespace std
