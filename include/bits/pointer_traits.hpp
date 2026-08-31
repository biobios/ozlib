#pragma once

#include <bits/namespace.hpp>

#include <bits/cv_traits.hpp>
#include <bits/pointer_utils.hpp>
#include <bits/ptrdiff_t.hpp>
#include <bits/same.hpp>
#include <bits/template_constants.hpp>

namespace OZLIB_NAMESPACE {
namespace impl {

namespace detail {
template <typename T, typename>
struct remove_pointer_helper {
    using type = T;
};

template <typename T, typename U>
struct remove_pointer_helper<T, U*> {
    using type = U;
};
}  // namespace detail

template <typename T>
struct remove_pointer {
    using type = typename detail::remove_pointer_helper<
        T, remove_cv_t<T>>::type;
};

template <typename T>
using remove_pointer_t = typename remove_pointer<T>::type;

namespace exposition_only {
template <typename T>
struct ptr_traits_elem {};

template <typename T>
    requires requires { typename T::element_type; }
struct ptr_traits_elem<T> {
    using type = typename T::element_type;
};

template <template <typename...> typename SomePointer, typename T, typename... Args>
    requires (!requires {typename SomePointer<T, Args...>::element_type; })
struct ptr_traits_elem<SomePointer<T, Args...>> {
    using type = T;
};

template <typename T>
concept has_elem_type = requires { typename ptr_traits_elem<T>::type; };

}

namespace pointer_traits_helper {
template <typename T>
struct get_difference_type {
    using type = ptrdiff_t;
};

template <typename T>
    requires requires { typename T::difference_type; }
struct get_difference_type<T> {
    using type = typename T::difference_type;
};

template <typename T, typename U>
struct get_rebind {};

template <typename T, typename U>
    requires requires { typename T:: template rebind<U>; }
struct get_rebind<T, U> {
    using type = typename T:: template rebind<U>;
};

template <template <typename...> typename SomePointer, typename T, typename U, typename... Args>
    requires (!requires { typename SomePointer<T, Args...>:: template rebind<U>; })
struct get_rebind<SomePointer<T, Args...>, U> {
    using type = SomePointer<U, Args...>;
};
}

template <typename Ptr>
struct pointer_traits {
    using pointer = Ptr;
    using element_type = typename exposition_only::ptr_traits_elem<Ptr>::type;
    using difference_type = typename pointer_traits_helper::get_difference_type<Ptr>::type;
    template <typename U>
    using rebind = typename pointer_traits_helper::get_rebind<Ptr, U>::type;

    static constexpr pointer pointer_to(element_type& r) noexcept {
        return Ptr::pointer_to(r);
    }
};

template <typename T>
struct pointer_traits<T*> {
    using pointer = T*;
    using element_type = T;
    using difference_type = ptrdiff_t;
    template <typename U>
    using rebind = U*;

    static constexpr pointer pointer_to(element_type& r) noexcept {
        return addressof(r);
    }
};

}  // namespace impl
}  // namespace OZLIB_NAMESPACE
