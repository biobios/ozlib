#pragma once

#include <bits/cv_traits.hpp>
#include <bits/reference_traits.hpp>

namespace std {
namespace impl {
template <typename T>
struct remove_cvref {
    using type = remove_cv_t<remove_reference_t<T>>;
};

template <typename T>
using remove_cvref_t = typename remove_cvref<T>::type;
}  // namespace impl
}  // namespace std