#pragma once

#include <bits/declval.hpp>
#include <bits/is_void.hpp>
#include <bits/same.hpp>
#include <bits/size_t.hpp>
#include <bits/template_constants.hpp>

namespace std {
namespace impl {

template <typename T, typename... Args>
struct is_constructible : false_type {};

template <typename T, typename... Args>
    requires(requires(Args... args) { T(args...); } && !is_void_v<T>)
struct is_constructible<T, Args...> : true_type {};

template <typename T>
inline constexpr bool is_constructible_v = is_constructible<T>::value;

template <typename T>
struct is_default_constructible : is_constructible<T>::type {};

template <typename T, size_t N>
struct is_default_constructible<T[N]> : is_constructible<T>::type {};

template <typename T>
inline constexpr bool is_default_constructible_v =
    is_default_constructible<T>::value;

// C++11準拠　C++14へ修正が必要
template <typename T>
struct is_copy_constructible : is_constructible<T, const T&> {};

template <typename T>
inline constexpr bool is_copy_constructible_v = is_copy_constructible<T>::value;

// C++11準拠　C++14へ修正が必要
template <typename T>
struct is_move_constructible : is_constructible<T, T&&> {};

template <typename T>
inline constexpr bool is_move_constructible_v = is_move_constructible<T>::value;

template <typename T, typename... Args>
struct is_nothrow_constructible : false_type {};

template <typename T, typename... Args>
    requires(requires(Args... args) {
        { T(args...) } noexcept;
    } && !is_void_v<T>)
struct is_nothrow_constructible<T, Args...> : true_type {};

template <typename T>
inline constexpr bool is_nothrow_constructible_v =
    is_nothrow_constructible<T>::value;

template <typename T>
struct is_nothrow_default_constructible : is_nothrow_constructible<T> {};

template <typename T, size_t N>
struct is_nothrow_default_constructible<T[N]> : is_nothrow_constructible<T> {};

template <typename T>
inline constexpr bool is_nothrow_default_constructible_v =
    is_nothrow_default_constructible<T>::value;

template <typename T>
struct is_nothrow_copy_constructible : is_nothrow_constructible<T, const T&> {};

template <typename T>
inline constexpr bool is_nothrow_copy_constructible_v =
    is_nothrow_copy_constructible<T>::value;

template <typename T>
struct is_nothrow_move_constructible : is_nothrow_constructible<T, T&&> {};

template <typename T>
inline constexpr bool is_nothrow_move_constructible_v =
    is_nothrow_move_constructible<T>::value;

}  // namespace impl

}  // namespace std
