#pragma once

#include <bits/template_constants.hpp>
#include <bits/type_identity.hpp>

namespace std {
namespace impl {

template <typename T>
struct remove_reference {
    using type = T;
};

template <typename T>
struct remove_reference<T&> {
    using type = T;
};

template <typename T>
struct remove_reference<T&&> {
    using type = T;
};

template <typename T>
using remove_reference_t = typename remove_reference<T>::type;

template <typename T>
struct is_lvalue_reference : false_type {};

template <typename T>
struct is_lvalue_reference<T&> : true_type {};

template <typename T>
inline constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;

template <class T>
struct is_rvalue_reference : false_type {};

template <class T>
struct is_rvalue_reference<T&&> : true_type {};

template <class T>
inline constexpr bool is_rvalue_reference_v = is_rvalue_reference<T>::value;

template <class T>
struct is_reference
    : bool_constant<is_lvalue_reference_v<T> || is_rvalue_reference_v<T>> {};

template <class T>
inline constexpr bool is_reference_v = is_reference<T>::value;

template <typename T>
struct add_lvalue_reference {
    using type = T;
};

template <typename T>
    requires(requires { typename type_identity<remove_reference_t<T>&>::type; })
struct add_lvalue_reference<T> {
    using type = remove_reference_t<T>&;
};

template <typename T>
using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;

template <typename T>
struct add_rvalue_reference {
    using type = T;
};

template <typename T>
    requires(requires { typename type_identity<T&&>::type; })
struct add_rvalue_reference<T> {
    using type = T&&;
};

template <typename T>
using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

}  // namespace impl

}  // namespace std
