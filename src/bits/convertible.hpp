#pragma once

#include <bits/declval.hpp>

namespace std {
namespace impl {

template <typename From, typename To>
struct is_convertible : false_type {};

template <typename From, typename To>
    requires(requires(void(func)(To)) { func(declval<From>()); })
struct is_convertible<From, To> : true_type {};

template <typename From, typename To>
inline constexpr bool is_convertible_v = is_convertible<From, To>::value;

template <class From, class To>
concept convertible_to = impl::is_convertible_v<From, To> &&
                         requires(impl::add_rvalue_reference_t<From> (&f)()) {
                             static_cast<To>(f());
                         };

}  // namespace impl

}  // namespace std