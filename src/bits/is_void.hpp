#pragma once

#include <bits/cv_ope.hpp>
#include <bits/same.hpp>
#include <bits/template_constants.hpp>

namespace std {
namespace impl {

template <typename T>
struct is_void : false_type {};

template <typename T>
    requires same_as<remove_cv_t<T>, void>
struct is_void<T> : true_type {};

template <typename T>
inline constexpr bool is_void_v = is_void<T>::value;

}  // namespace impl
}  // namespace std
