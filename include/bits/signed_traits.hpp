#pragma once

#include <bits/namespace.hpp>

#include <bits/arithmetic_traits.hpp>
#include <bits/exposition_only.hpp>
#include <bits/underlying_type.hpp>

namespace OZLIB_NAMESPACE {
namespace impl {
namespace detail {
template <typename T>
struct make_signed_helper {
    using type = T;
};

template <>
struct make_signed_helper<unsigned char> {
    using type = signed char;
};

template <>
struct make_signed_helper<unsigned short> {
    using type = short;
};

template <>
struct make_signed_helper<unsigned int> {
    using type = int;
};

template <>
struct make_signed_helper<unsigned long> {
    using type = long;
};

template <>
struct make_signed_helper<unsigned long long> {
    using type = long long;
};

}

template <typename T>
struct make_signed;

template <typename T>
    requires (is_integral_v<T> && !same_as<remove_cv_t<T>, bool>)
struct make_signed<T> {
    using type = exposition_only::copycv<T, typename detail::make_signed_helper<remove_cv_t<T>>::type>;
};

template <typename T>
    requires is_enum_v<T>
struct make_signed<T> : make_signed<underlying_type_t<T>> {};

template <typename T>
using make_signed_t = typename make_signed<T>::type;

namespace detail {
template <typename T>
struct make_unsigned_helper {
    using type = T;
};

template <>
struct make_unsigned_helper<signed char> {
    using type = unsigned char;
};

template <>
struct make_unsigned_helper<short> {
    using type = unsigned short;
};

template <>
struct make_unsigned_helper<int> {
    using type = unsigned int;
};

template <>
struct make_unsigned_helper<long> {
    using type = unsigned long;
};

template <>
struct make_unsigned_helper<long long> {
    using type = unsigned long long;
};

}

template <typename T>
struct make_unsigned;

template <typename T>
    requires (is_integral_v<T> && !same_as<remove_cv_t<T>, bool>)
struct make_unsigned<T> {
    using type = exposition_only::copycv<T, typename detail::make_unsigned_helper<remove_cv_t<T>>::type>;
};

template <typename T>
    requires is_enum_v<T>
struct make_unsigned<T> : make_unsigned<underlying_type_t<T>> {};

}
}