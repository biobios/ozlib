#pragma once

#include <bits/namespace.hpp>

#include <bits/reference_traits.hpp>
#include <bits/fundamental_traits.hpp>

namespace OZLIB_NAMESPACE {
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
}  // namespace OZLIB_NAMESPACE
