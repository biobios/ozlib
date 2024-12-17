#pragma once

#include <bits/common_traits.hpp>
#include <bits/declval.hpp>
#include <bits/forward.hpp>
#include <bits/reference_traits.hpp>
#include <bits/same.hpp>

namespace std {
namespace impl {
template <typename T, typename D>
struct is_assignable : false_type {};

template <typename T, typename D>
    requires requires { declval<T>() = declval<T>(); }
struct is_assignable<T, D> : true_type {};

template <typename T, typename D>
inline constexpr bool is_assignable_v = is_assignable<T, D>::value;

template <typename T>
struct is_copy_assignable : is_assignable<T, const T&> {};

template <typename T>
inline constexpr bool is_copy_assignable_v = is_copy_assignable<T>::value;

template <typename T>
struct is_move_assignable : is_assignable<T, T&&> {};

template <typename T>
inline constexpr bool is_move_assignable_v = is_move_assignable<T>::value;

template <typename T, typename D>
struct is_nothrow_assignable : false_type {};

template <typename T, typename D>
    requires requires {
        { declval<T>() = declval<D>() } noexcept;
    }
struct is_nothrow_assignable<T, D> : true_type {};

template <typename T, typename D>
inline constexpr bool is_nothrow_assignable_v =
    is_nothrow_assignable<T, D>::value;

template <typename T>
struct is_nothrow_copy_assignable : is_nothrow_assignable<T, const T&> {};

template <typename T>
inline constexpr bool is_nothrow_copy_assignable_v =
    is_nothrow_copy_assignable<T>::value;

template <typename T>
struct is_nothrow_move_assignable : is_nothrow_assignable<T, T&&> {};

template <typename T>
inline constexpr bool is_nothrow_move_assignable_v =
    is_nothrow_move_assignable<T>::value;

template <typename LHS, typename RHS>
concept assignable_from =
    is_lvalue_reference_v<LHS> &&
    common_reference_with<const remove_reference_t<LHS>&,
                          const remove_reference_t<RHS>&> &&
    requires(LHS lhs, RHS&& rhs) {
        { lhs = std::impl::forward<RHS>(rhs) } -> same_as<LHS>;
    };

}  // namespace impl
}  // namespace std
