#pragma once

#include <bits/compound_traits.hpp>
#include <bits/declval.hpp>
#include <bits/fundamental_traits.hpp>
#include <bits/ptrdiff_t.hpp>

namespace std {
namespace impl {

template <typename>
struct incrementable_traits;

template <typename T>
    requires is_object_v<T>
struct incrementable_traits<T*> {
    using difference_type = ptrdiff_t;
};

template <typename I>
struct incrementable_traits<const I> : incrementable_traits<I> {};

template <typename T>
    requires requires { typename T::difference_type; }
struct incrementable_traits<T> {
    using difference_type = typename T::difference_type;
};

template <typename T>
    requires(
        !requires { typename T::difference_type; } &&
        requires(const T& a, const T& b) {
            { a - b } -> integral;
        })
struct incrementable_traits<T> {
    using difference_type =
        make_signed_t<decltype(declval<T>() - declval<T>())>;
};

}  // namespace impl
}  // namespace std
