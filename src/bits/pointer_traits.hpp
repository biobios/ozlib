#pragma once

#include <bits/cv_traits.hpp>
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

}  // namespace impl
}  // namespace std
