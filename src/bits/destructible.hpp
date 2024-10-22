#pragma once

#include <bits/template_constants.hpp>

namespace std {
namespace impl {
// traits
template <typename T>
struct is_nothrow_destructible : false_type {};

template <typename T>
    requires(requires(T t) { t.~T(); })
struct is_nothrow_destructible<T> : true_type {};

template <typename T>
inline constexpr bool is_nothrow_destructible_v =
    is_nothrow_destructible<T>::value;

// concepts
template <typename T>
concept destructible = is_nothrow_destructible_v<T>;

}  // namespace impl
}  // namespace std
