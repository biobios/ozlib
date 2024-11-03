#pragma once

namespace std {
namespace impl {

template <class T>
constexpr T* addressof(T& r) noexcept {
    return reinterpret_cast<T*>(
        &const_cast<char&>(reinterpret_cast<const volatile char&>(r)));
}

template <class T>
const T* addressof(const T&&) = delete;

}  // namespace impl
}  // namespace std