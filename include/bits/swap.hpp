#pragma once

#include <bits/namespace.hpp>

#include <bits/assignable.hpp>
#include <bits/constructible.hpp>
#include <bits/fundamental_traits.hpp>
#include <bits/move.hpp>
#include <bits/reference_traits.hpp>
#include <bits/size_t.hpp>

namespace OZLIB_NAMESPACE {
namespace impl {
namespace ranges {
namespace swap_fn {

template <typename T>
void swap(T&, T&) = delete;

struct swap_fn {
    template <typename T, typename U>
        requires(is_class_v<remove_reference_t<T>> ||
                 is_enum_v<remove_reference_t<T>> ||
                 is_class_v<remove_reference_t<U>> ||
                 is_enum_v<remove_reference_t<U>>) &&
                requires(T&& t, U&& u) {
                    void(swap(static_cast<T&&>(t), static_cast<U&&>(u)));
                }
    constexpr void operator()(T&& t, U&& u) const
        noexcept(noexcept(swap(static_cast<T&&>(t), static_cast<U&&>(u)))) {
        swap(static_cast<T&&>(t), static_cast<U&&>(u));
    }

    template <typename T, typename U, OZLIB_NAMESPACE::impl::size_t N>
        requires requires(T& a, U& b, swap_fn fn) {
            { fn(a, b) };
        }
    constexpr void operator()(T (&a)[N], U (&b)[N]) const
        noexcept((*this)(OZLIB_NAMESPACE::impl::declval<T&>(), OZLIB_NAMESPACE::impl::declval<U&>())) {
        for (OZLIB_NAMESPACE::impl::size_t i = 0; i < N; ++i) {
            (*this)(a[i], b[i]);
        }
    }

    template <typename T>
        requires(!((OZLIB_NAMESPACE::impl::is_class_v<OZLIB_NAMESPACE::impl::remove_reference_t<T>> ||
                    OZLIB_NAMESPACE::impl::is_enum_v<OZLIB_NAMESPACE::impl::remove_reference_t<T>>) &&
                   requires(T&& t, T&& u) {
                       void(swap(static_cast<T&&>(t), static_cast<T&&>(u)));
                   }) &&
                 OZLIB_NAMESPACE::impl::move_constructible<T> &&
                 OZLIB_NAMESPACE::impl::assignable_from<T&, T>)
    constexpr void operator()(T& a, T& b) const
        noexcept(OZLIB_NAMESPACE::impl::is_nothrow_move_constructible_v<T> &&
                 OZLIB_NAMESPACE::impl::is_nothrow_move_assignable_v<T>) {
        T tmp(OZLIB_NAMESPACE::impl::move(a));
        a = OZLIB_NAMESPACE::impl::move(b);
        b = OZLIB_NAMESPACE::impl::move(tmp);
    }
};
}  // namespace swap_fn

inline namespace cpo_swap {
inline constexpr swap_fn::swap_fn swap{};
}

}  // namespace ranges


template <typename T>
constexpr void swap(T& a, T& b) noexcept(is_nothrow_move_constructible_v<T> &&
                                         is_nothrow_move_assignable_v<T>) {
    T tmp(OZLIB_NAMESPACE::impl::move(a));
    a = OZLIB_NAMESPACE::impl::move(b);
    b = OZLIB_NAMESPACE::impl::move(tmp);
}

template <typename T, size_t N>
constexpr void swap(T (&a)[N], T (&b)[N]) noexcept(noexcept(swap(*a, *b))) {
    for (size_t i = 0; i < N; ++i) {
        swap(a[i], b[i]);
    }
}

template <typename T>
concept swappable = requires(T& a, T& b) { ranges::swap(a, b); };

template <typename T, typename U>
concept swappable_with = common_reference_with<T, U> && requires(T&& t, U&& u) {
    ranges::swap(impl::forward<T>(t), impl::forward<T>(t));
    ranges::swap(impl::forward<U>(u), impl::forward<U>(u));
    ranges::swap(impl::forward<T>(t), impl::forward<U>(u));
    ranges::swap(impl::forward<U>(u), impl::forward<T>(t));
};

}  // namespace impl
}  // namespace OZLIB_NAMESPACE
