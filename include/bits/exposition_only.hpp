#pragma once

#include <bits/namespace.hpp>

#include <bits/convertible.hpp>

namespace OZLIB_NAMESPACE {
namespace impl {
namespace exposition_only {

template <typename T>
concept boolean_testable_impl = convertible_to<T, bool>;

template <typename T>
concept boolean_testable = boolean_testable_impl<T> && requires(T&& t) {
    { !forward<T>(t) } -> boolean_testable_impl;
};

template <typename T>
using with_reference = T&;

template <typename T>
concept can_reference = requires { typename with_reference<T>; };

template <typename T>
concept dereferenceable = requires(T& t) {
    { *t } -> can_reference;
};

namespace detail {
template <typename FROM, typename TO>
struct copycv_helper {
    using type = TO;
};

template <typename FROM, typename TO>
struct copycv_helper<FROM const, TO> {
    using type = TO const;
};

template <typename FROM, typename TO>
struct copycv_helper<FROM volatile, TO> {
    using type = TO volatile;
};

template <typename FROM, typename TO>
struct copycv_helper<FROM const volatile, TO> {
    using type = TO const volatile;
};
} // namespace detail

template <typename FROM, typename TO>
using copycv = typename detail::copycv_helper<FROM, TO>::type;
}  // namespace exposition_only
}  // namespace impl
}  // namespace OZLIB_NAMESPACE