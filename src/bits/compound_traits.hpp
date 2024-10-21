#pragma once

#include <bits/function_traits.hpp>

namespace std {
namespace impl {

template <class T>
struct is_object
    : bool_constant<!is_function_v<T> && !is_reference_v<T> && !is_void_v<T>> {
};

template <class T>
inline constexpr bool is_object_v = is_object<T>::value;

}  // namespace impl
}  // namespace std
