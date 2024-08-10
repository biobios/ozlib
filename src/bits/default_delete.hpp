#pragma once

#include <bits/convertible.hpp>
#include <bits/is_void.hpp>

namespace std {
namespace impl {
template <typename T>
struct default_delete {
    constexpr default_delete() noexcept = default;
    template <typename From>
        requires is_convertible_v<From*, T*>
    default_delete(const default_delete<From>&) noexcept {}
    void operator()(T* ptr) const {
        static_assert(!is_void_v<T>, "can't delete pointer to incomplete type");
        static_assert(sizeof(T) > 0, "can't delete pointer to incomplete type");
        delete ptr;
    }
};
template <typename T>
struct default_delete<T[]> {
    constexpr default_delete() noexcept = default;

    template <typename From>
        requires is_convertible_v<From (*)[], T (*)[]>
    default_delete(const default_delete<From[]>&) noexcept {}

    template <typename From>
        requires is_convertible_v<From (*)[], T (*)[]>
    void operator()(From* ptr) const {
        static_assert(sizeof(T) > 0, "can't delete pointer to incomplete type");
        delete[] ptr;
    }
};
}  // namespace impl
}  // namespace std
