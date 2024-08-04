#pragma once

#include <bits/template_constants.hpp>

namespace std {
namespace impl {

template <class T, class U>
struct is_same : false_type {};

template <class T>
struct is_same<T, T> : true_type {};

template <class T, class U>
inline constexpr bool is_same_v = is_same<T, U>::value;
namespace {
namespace helper_same_as {
template <class T, class U>
concept same_as_impl = is_same_v<T, U>;
}
}  // namespace
template <typename T, typename U>
concept same_as =
    (helper_same_as::same_as_impl<T, U> && helper_same_as::same_as_impl<U, T>);

namespace helper_same_as {};
}  // namespace impl
}  // namespace std
