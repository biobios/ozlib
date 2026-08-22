#pragma once

#include <bits/namespace.hpp>

#include <bits/reference_traits.hpp>

namespace OZLIB_NAMESPACE {
namespace impl {
template <class T>
constexpr typename remove_reference<T>::type&& move(T&& t) noexcept {
    return static_cast<typename remove_reference<T>::type&&>(t);
}
}  // namespace impl

}  // namespace OZLIB_NAMESPACE
