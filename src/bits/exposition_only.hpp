#pragma once

#include <bits/convertible.hpp>

namespace std {
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
}  // namespace exposition_only
}  // namespace impl
}  // namespace std