#pragma once

#include <bits/cv_ope.hpp>
#include <bits/same.hpp>
#include <bits/template_constants.hpp>

namespace std {
namespace impl {

namespace {
namespace helper_remove_pointer {
template <typename T, typename>
struct remove_pointer_helper {
    using type = T;
};

template <typename T, typename U>
struct remove_pointer_helper<T, U*> {
    using type = U;
};
}  // namespace helper_remove_pointer
}  // namespace

template <typename T>
struct remove_pointer {
    using type = typename helper_remove_pointer::remove_pointer_helper<
        T, remove_cv_t<T>>::type;
};

namespace helper_remove_pointer {}

template <typename T>
using remove_pointer_t = typename remove_pointer<T>::type;

template <typename T>
struct is_pointer : false_type {};

template <typename T>
    requires(!is_same_v<remove_pointer_t<T>, T>)
struct is_pointer<T> : true_type {};

template <typename T>
inline constexpr bool is_pointer_v = is_pointer<T>::value;

}  // namespace impl
}  // namespace std
