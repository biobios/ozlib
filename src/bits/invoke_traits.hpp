#pragma once

#include <bits/cvref_traits.hpp>
#include <bits/fundamental_traits.hpp>
#include <bits/inheritance_traits.hpp>
#include <bits/template_logics.hpp>

namespace std {
namespace impl {

template <typename T>
class reference_wrapper;

namespace helper {
template <typename VOID, typename T, typename... Args>
struct invoke_result_impl {
    using type =
        decltype(std::impl::declval<T>()(std::impl::declval<Args>()...));
};

template <typename Ret, typename T>
struct invoke_result_impl<void, Ret(T::*), T*> {
    using type =
        decltype(*(std::impl::declval<T*>()).*(std::impl::declval<Ret(T::*)>));
};

template <typename Ret, typename T, typename Arg>
struct invoke_result_impl<
    enable_if_t<is_same_v<reference_wrapper<T>, remove_cvref_t<Arg>> &&
                !is_function_v<Ret>>,
    Ret(T::*), Arg> {
    using type = decltype((std::impl::declval<Arg>.get()).*
                          (std::impl::declval<Ret(T::*)>()));
};

template <typename Ret, typename T, typename Arg>
struct invoke_result_impl<
    enable_if_t<is_base_of_v<T, remove_cvref_t<Arg>> && !is_function_v<Ret>>,
    Ret(T::*), Arg> {
    using type = decltype((std::impl::declval<Arg>()).*
                          (std::impl::declval<Ret(T::*)>()));
};

template <typename Ret, typename T, typename T1, typename... Args>
struct invoke_result_impl<
    enable_if_t<!is_same_v<reference_wrapper<T>, remove_cvref_t<T1>> &&
                !is_base_of_v<T, remove_cvref_t<T1>>>,
    Ret (T::*)(Args...), T1, Args...> {
    using type = decltype((*std::impl::declval<T1>()).*
                          (std::impl::declval<Ret (T::*)(Args...)>(
                              std::impl::declval<Args>()...)));
};

template <typename Ret, typename T, typename T1, typename... Args>
struct invoke_result_impl<
    enable_if_t<is_same_v<reference_wrapper<T>, remove_cvref_t<T1>>>,
    Ret (T::*)(Args...), T1, Args...> {
    using type = decltype(((std::impl::declval<T1>.get()).*
                           declval<Ret (T::*)(Args...)>())(
        std::impl::declval<Args>()...));
};

template <typename Ret, typename T, typename T1, typename... Args>
struct invoke_result_impl<enable_if_t<is_base_of_v<T, remove_cvref_t<T1>>>,
                          Ret (T::*)(Args...), T1, Args...> {
    using type = decltype(((std::impl::declval<T1>()).*
                           std::impl::declval<Ret (T::*)(Args...)>())(
        std::impl::declval<Args>()...));
};
}  // namespace helper

template <typename T, typename... Args>
struct invoke_result : helper::invoke_result_impl<void, T, Args...> {};

template <typename T, typename... Args>
using invoke_result_t = typename invoke_result<T, Args...>::type;

}  // namespace impl
}  // namespace std