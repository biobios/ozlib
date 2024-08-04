#pragma once

#include <bits/remove_reference.hpp>

namespace std {
namespace impl {
template <class T>
constexpr typename remove_reference<T>::type&& move(T&& t) noexcept {
    return static_cast<typename remove_reference<T>::type&&>(t);
}
}  // namespace impl

}  // namespace std
