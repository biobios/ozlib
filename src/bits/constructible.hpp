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
struct is_default_constructible : is_constructible<T>::type {};

template <typename T, size_t n>
struct is_default_constructible<T[n]> : is_constructible<T>::type {};

// C++11準拠　C++14へ修正が必要
template <typename T>
struct is_copy_constructible : is_constructible<T, const T&> {};

// C++11準拠　C++14へ修正が必要
template <typename T>
struct is_move_constructible : is_constructible<T, T&&> {};

}  // namespace impl

}  // namespace std
