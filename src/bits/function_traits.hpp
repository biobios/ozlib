#pragma once

#include <bits/cv_traits.hpp>
#include <bits/template_constants.hpp>

namespace std {
namespace impl {

template <typename T>
struct is_function : bool_constant<!is_const_v<const T>> {};

template <typename T>
struct is_function<T&> : public false_type {};

template <typename T>
struct is_function<T&&> : public false_type {};

template <typename T>
inline constexpr bool is_function_v = is_function<T>::value;

}  // namespace impl
}  // namespace std
