#pragma once

#include <bits/is_lvalue_reference.hpp>
#include <bits/remove_reference.hpp>

namespace std {
namespace impl {

template <class T>
constexpr T&& forward(typename remove_reference<T>::type& t) noexcept {
    return static_cast<T&&>(t);
}

template <class T>
constexpr T&& forward(typename remove_reference<T>::type&& t) noexcept {
    static_assert(!is_lvalue_reference<T>::value,
                  "Can not forward an rvalue as an lvalue.");
    return static_cast<T&&>(t);
}

}  // namespace impl
}  // namespace std
