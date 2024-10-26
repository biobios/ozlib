#pragma once

#include <bits/assignable.hpp>
#include <bits/constructible.hpp>
#include <bits/move.hpp>
#include <bits/size_t.hpp>

namespace std {
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

}  // namespace impl
}  // namespace std
