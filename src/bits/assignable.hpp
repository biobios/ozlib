#pragma once

#include <bits/declval.hpp>

namespace std {
namespace impl {
template <typename T, typename D>
struct is_assignable : false_type {};

template <typename T, typename D>
    requires requires { declval<T>() = declval<T>(); }
struct is_assignable<T, D> : true_type {};

template <typename T, typename D>
inline constexpr bool is_assignable_v = is_assignable<T, D>::value;

}  // namespace impl
}  // namespace std
