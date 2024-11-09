#pragma once

#include <bits/cv_traits.hpp>
#include <bits/void_t.hpp>

namespace std {
namespace impl {

#ifdef __GNUC__
template <typename Base, typename Derived>
struct is_base_of : bool_constant<__is_base_of(Base, Derived)> {};
#else

namespace helper {

template <typename Base, typename Derived, typename = void>
struct is_base_of_impl : false_type {};

template <typename Base, typename Derived>
struct is_base_of_impl<Base, Derived,
                       void_t<decltype(static_cast<remove_cv_t<Base>*>(
                           declval<remove_cv_t<Derived>*>()))> > : true_type {};
}  // namespace helper

template <typename Base, typename Derived>
struct is_base_of : helper::is_base_of_impl<Base, Derived> {};
#endif

template <typename Base, typename Derived>
inline constexpr bool is_base_of_v = is_base_of<Base, Derived>::value;

}  // namespace impl
}  // namespace std