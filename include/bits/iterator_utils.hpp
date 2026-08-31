#pragma once

#include <bits/namespace.hpp>

#include <bits/size_t.hpp>

namespace OZLIB_NAMESPACE {
template <typename C>
constexpr auto begin(C& c) noexcept(noexcept(c.begin())) -> decltype(c.begin()) {
    return c.begin();
}

template <typename C>
constexpr auto begin(const C& c) noexcept(noexcept(c.begin())) -> decltype(c.begin()) {
    return c.begin();
}

template <typename T, impl::size_t N>
constexpr T* begin(T (&array)[N]) noexcept {
    return array;
}

template <typename C>
constexpr auto end(C& c) noexcept(noexcept(c.end())) -> decltype(c.end()) {
    return c.end();
}

template <typename C>
constexpr auto end(const C& c) noexcept(noexcept(c.end())) -> decltype(c.end()) {
    return c.end();
}

template <typename T, impl::size_t N>
constexpr T* end(T (&array)[N]) noexcept {
    return array + N;
}
}