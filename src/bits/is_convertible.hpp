#pragma once

#include <bits/declval.hpp>
#include <bits/is_void.hpp>
#include <bits/void_t.hpp>

namespace std {
namespace impl {
namespace {
namespace helper_is_convertible {
template <typename From, typename To, typename = void>
struct is_convertible {
    using type = typename std::impl::is_void<To>::type;
};

template <typename T>
void test(T);

template <typename From, typename To>
struct is_convertible<
    From, To,
    std::impl::void_t<decltype(helper_is_convertible::test<To>(
        std::impl::declval<From>()))>> {
    using type = true_type;
};
}  // namespace helper_is_convertible
}  // namespace

template <typename From, typename To>
struct is_convertible : helper_is_convertible::is_convertible<From, To>::type {
};

namespace helper_is_convertible {};

template <typename From, typename To>
inline constexpr bool is_convertible_v = is_convertible<From, To>::value;
}  // namespace impl

}  // namespace std