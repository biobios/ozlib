#pragma once

#include <bits/assignable.hpp>
#include <bits/constructible.hpp>
#include <bits/fundamental_traits.hpp>
#include <bits/move.hpp>
#include <bits/size_t.hpp>

namespace std {
namespace ranges {
namespace impl {
namespace swap_fn {

template <typename T>
void swap(T&, T&) = delete;

struct swap_fn {
    template <typename T, typename U>
        requires(std::impl::is_class_v<std::impl::remove_reference_t<T>> ||
                 std::impl::is_enum_v<std::impl::remove_reference_t<T>> ||
                 std::impl::is_class_v<std::impl::remove_reference_t<U>> ||
                 std::impl::is_enum_v<std::impl::remove_reference_t<U>>) &&
                requires(T&& t, U&& u) {
                    void(swap(static_cast<T&&>(t), static_cast<U&&>(u)));
                }
    constexpr void operator()(T&& t, U&& u) const
        noexcept(noexcept(swap(static_cast<T&&>(t), static_cast<U&&>(u)))) {
        swap(static_cast<T&&>(t), static_cast<U&&>(u));
    }

    template <typename T, typename U, std::impl::size_t N>
        requires requires(T& a, U& b, swap_fn fn) {
            { fn(a, b) };
        }
    constexpr void operator()(T (&a)[N], U (&b)[N]) const
        noexcept((*this)(declval<T&>(), declval<U&>())) {
        for (std::impl::size_t i = 0; i < N; ++i) {
            (*this)(a[i], b[i]);
        }
    }

    template <typename T>
        requires(!((std::impl::is_class_v<std::impl::remove_reference_t<T>> ||
                    std::impl::is_enum_v<std::impl::remove_reference_t<T>>) &&
                   requires(T&& t, T&& u) {
                       void(swap(static_cast<T&&>(t), static_cast<T&&>(u)));
                   }) &&
                 std::impl::move_constructible<T> &&
                 std::impl::assignable_from<T&, T>)
    constexpr void operator()(T& a, T& b) const
        noexcept(std::impl::is_nothrow_move_constructible_v<T> &&
                 std::impl::is_nothrow_move_assignable_v<T>) {
        T tmp(std::impl::move(a));
        a = std::impl::move(b);
        b = std::impl::move(tmp);
    }
};
}  // namespace swap_fn

inline namespace cpo_swap {
inline constexpr swap_fn::swap_fn swap{};
}

}  // namespace impl
}  // namespace ranges

namespace impl {

template <typename T>
constexpr void swap(T& a, T& b) noexcept(is_nothrow_move_constructible_v<T> &&
                                         is_nothrow_move_assignable_v<T>) {
    T tmp(std::impl::move(a));
    a = std::impl::move(b);
    b = std::impl::move(tmp);
}

template <typename T, size_t N>
constexpr void swap(T (&a)[N], T (&b)[N]) noexcept(noexcept(swap(*a, *b))) {
    for (size_t i = 0; i < N; ++i) {
        swap(a[i], b[i]);
    }
}

template <typename T>
concept swappable = requires(T& a, T& b) { ranges::impl::swap(a, b); };

template <typename T, typename U>
concept swappable_with = common_reference_with<T, U> && requires(T&& t, U&& u) {
    ranges::impl::swap(std::impl::forward<T>(t), std::impl::forward<T>(t));
    ranges::impl::swap(std::impl::forward<U>(u), std::impl::forward<U>(u));
    ranges::impl::swap(std::impl::forward<T>(t), std::impl::forward<U>(u));
    ranges::impl::swap(std::impl::forward<U>(u), std::impl::forward<T>(t));
};

}  // namespace impl
}  // namespace std
