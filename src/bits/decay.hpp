#pragma once

#include <bits/array_traits.hpp>
#include <bits/cv_ope.hpp>
#include <bits/function_traits.hpp>
#include <bits/reference_traits.hpp>
#include <bits/same.hpp>
#include <bits/size_t.hpp>

namespace std {
namespace impl {

template <class T>
struct decay {
    using type = remove_cv_t<remove_reference_t<T>>;
};

template <class T>
    requires is_array_v<remove_reference<T>>
struct decay<T> {
    using type = remove_all_extents_t<remove_reference_t<T>>;
};

template <class T>
    requires is_function_v<remove_reference_t<T>>
struct decay<T> {
    using type = remove_reference_t<T>*;
};

}  // namespace impl
}  // namespace std
