#pragma once

#include <bits/cv_traits.hpp>
#include <bits/nullptr_t.hpp>
#include <bits/same.hpp>
#include <bits/size_t.hpp>
#include <bits/template_constants.hpp>
#include <bits/type_identity.hpp>

namespace std {
namespace impl {

template <typename T>
struct is_void : false_type {};

template <typename T>
    requires same_as<remove_cv_t<T>, void>
struct is_void<T> : true_type {};

template <typename T>
inline constexpr bool is_void_v = is_void<T>::value;

template <typename T>
struct is_null_pointer : false_type {};

template <typename T>
    requires same_as<remove_cv_t<T>, nullptr_t>
struct is_null_pointer<T> : true_type {};

template <typename T>
inline constexpr bool is_null_pointer_v = is_null_pointer<T>::value;

template <typename T>
struct is_integral : false_type {};

template <typename T>
    requires(
        same_as<remove_cv_t<T>, bool> || same_as<remove_cv_t<T>, char> ||
        same_as<remove_cv_t<T>, signed char> ||
        same_as<remove_cv_t<T>, unsigned char> ||
        same_as<remove_cv_t<T>, char8_t> || same_as<remove_cv_t<T>, char16_t> ||
        same_as<remove_cv_t<T>, char32_t> || same_as<remove_cv_t<T>, wchar_t> ||
        same_as<remove_cv_t<T>, short> ||
        same_as<remove_cv_t<T>, unsigned short> ||
        same_as<remove_cv_t<T>, int> || same_as<remove_cv_t<T>, unsigned int> ||
        same_as<remove_cv_t<T>, long> ||
        same_as<remove_cv_t<T>, unsigned long> ||
        same_as<remove_cv_t<T>, long long> ||
        same_as<remove_cv_t<T>, unsigned long long>)
struct is_integral<T> : true_type {};

template <typename T>
inline constexpr bool is_integral_v = is_integral<T>::value;

template <typename T>
struct is_floating_point : false_type {};

template <typename T>
    requires(same_as<remove_cv_t<T>, float> ||
             same_as<remove_cv_t<T>, double> ||
             same_as<remove_cv_t<T>, long double>)
struct is_floating_point<T> : true_type {};

template <typename T>
inline constexpr bool is_floating_point_v = is_floating_point<T>::value;

template <typename T>
struct is_bounded_array : false_type {};

template <typename T, size_t N>
struct is_bounded_array<T[N]> : true_type {};

template <typename T>
inline constexpr bool is_bounded_array_v = is_bounded_array<T>::value;

template <typename T>
struct is_unbounded_array : false_type {};

template <typename T>
struct is_unbounded_array<T[]> : true_type {};

template <typename T>
inline constexpr bool is_unbounded_array_v = is_unbounded_array<T>::value;

namespace helper_is_pointer {
template <typename T>
struct is_pointer_helper : false_type {};

template <typename T>
struct is_pointer_helper<T*> : true_type {};
}  // namespace helper_is_pointer

template <typename T>
struct is_pointer : helper_is_pointer::is_pointer_helper<remove_cv_t<T>> {};

template <typename T>
inline constexpr bool is_pointer_v = is_pointer<T>::value;

template <typename T>
struct is_lvalue_reference : false_type {};

template <typename T>
struct is_lvalue_reference<T&> : true_type {};

template <typename T>
inline constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;

template <typename T>
struct is_rvalue_reference : false_type {};

template <typename T>
struct is_rvalue_reference<T&&> : true_type {};

template <typename T>
inline constexpr bool is_rvalue_reference_v = is_rvalue_reference<T>::value;

template <typename T>
struct is_function : bool_constant<!is_const_v<const T>> {};

template <typename T>
struct is_function<T&> : public false_type {};

template <typename T>
struct is_function<T&&> : public false_type {};

template <typename T>
inline constexpr bool is_function_v = is_function<T>::value;

namespace helper_is_member_pointer {
template <typename T>
struct is_member_object_pointer_helper : false_type {};

template <typename T, typename U>
    requires(!is_function_v<remove_cv_t<T>>)
struct is_member_object_pointer_helper<T U::*> : true_type {};

template <typename T>
struct is_member_function_pointer_helper : false_type {};

template <typename T, typename U>
    requires is_function_v<remove_cv_t<T>>
struct is_member_function_pointer_helper<T U::*> : true_type {};

}  // namespace helper_is_member_pointer

template <typename T>
struct is_member_object_pointer
    : helper_is_member_pointer::is_member_object_pointer_helper<
          remove_cv_t<T>> {};

template <typename T>
inline constexpr bool is_member_object_pointer_v =
    is_member_object_pointer<T>::value;

template <typename T>
struct is_member_function_pointer
    : helper_is_member_pointer::is_member_function_pointer_helper<
          remove_cv_t<T>> {};

template <typename T>
inline constexpr bool is_member_function_pointer_v =
    is_member_function_pointer<T>::value;

#ifdef __GNUC__

template <typename T>
struct is_union : bool_constant<__is_union(T)> {};

template <typename T>
struct is_class : bool_constant<__is_class(T)> {};

template <typename T>
struct is_enum : bool_constant<__is_enum(T)> {};

#else

template <typename T>
struct is_union : false_type {};

template <typename T>
struct is_class : false_type {};

template <typename T>
    requires(!is_union<T>::value &&
             requires { typename type_identity<int T::*>; })
struct is_class<T> : true_type {};

template <typename T>
struct is_enum : false_type {};

template <typename T>
    requires(!(is_void_v<T> || is_integral_v<T> || is_floating_point_v<T> ||
               is_bounded_array_v<T> || is_unbounded_array_v<T> ||
               is_pointer_v<T> || is_function_v<T> ||
               is_lvalue_reference_v<T> || is_rvalue_reference_v<T> ||
               is_class<T>::value || is_union<T>::value))
struct is_enum<T> : true_type {};

#endif

template <typename T>
inline constexpr bool is_union_v = is_union<T>::value;

template <typename T>
inline constexpr bool is_class_v = is_class<T>::value;

template <typename T>
inline constexpr bool is_enum_v = is_enum<T>::value;

// concepts
template <typename T>
concept integral = is_integral_v<T>;

}  // namespace impl
}  // namespace std
