#pragma once

#include <bits/size_t.hpp>
#include <bits/template_constants.hpp>

namespace std {
namespace impl {

template <typename>
struct is_array : false_type {};

template <typename T, size_t size>
struct is_array<T[size]> : true_type {};

template <typename T>
struct is_array<T[]> : true_type {};

template <typename T>
inline constexpr bool is_array_v = is_array<T>::value;

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
