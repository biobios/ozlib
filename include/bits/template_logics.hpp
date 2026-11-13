#pragma once

#include <bits/namespace.hpp>

#include <bits/template_constants.hpp>

namespace OZLIB_NAMESPACE {
namespace impl {

template <bool Condition, class T = void>
struct enable_if {};

template <typename T>
struct enable_if<true, T> {
    using type = T;
};

template <bool Condition, class T = void>
using enable_if_t = typename enable_if<Condition, T>::type;

template <bool Condition, class T, class F>
struct conditional {
    using type = F;
};

template <class T, class F>
struct conditional<true, T, F> {
    using type = T;
};

template <bool Condition, class T, class F>
using conditional_t = typename conditional<Condition, T, F>::type;

template <typename... Traits>
struct conjunction : true_type {};

template <typename Trait>
struct conjunction<Trait> : Trait {};

template <typename Trait, typename... Traits>
struct conjunction<Trait, Traits...> : conditional_t<bool(Trait::value), conjunction<Traits...>, Trait> {};

template <typename... Traits>
inline constexpr bool conjunction_v = conjunction<Traits...>::value;

template <typename... Traits>
struct disjunction : false_type {};

template <typename Trait>
struct disjunction<Trait> : Trait {};

template <typename Trait, typename... Traits>
struct disjunction<Trait, Traits...> : conditional_t<bool(Trait::value), Trait, disjunction<Traits...>> {};

template <typename... Traits>
inline constexpr bool disjunction_v = disjunction<Traits...>::value;

template <typename Trait>
struct negation : bool_constant<!bool(Trait::value)> {};

template <typename Trait>
inline constexpr bool negation_v = negation<Trait>::value;

}  // namespace impl
}  // namespace OZLIB_NAMESPACE
