#pragma once

#include <bits/cvref_traits.hpp>
#include <bits/forward.hpp>
#include <bits/fundamental_traits.hpp>
#include <bits/inheritance_traits.hpp>
#include <bits/invoke_traits.hpp>
#include <bits/template_logics.hpp>

namespace std {
namespace impl {

template <typename T>
class reference_wrapper;

namespace helper {

struct invoke_other {};
struct invoke_memobj_ptr {};
struct invoke_memobj_refwrap {};
struct invoke_memobj_ref {};
struct invoke_memfun_ptr {};
struct invoke_memfun_refwrap {};
struct invoke_memfun_ref {};

template <typename VOID, typename T, typename... Args>
struct invoke_type {
    using type = invoke_other;
};

template <typename Ret, typename T>
struct invoke_type<void, Ret(T::*), T*> {
    using type = invoke_memobj_ptr;
};

template <typename Ret, typename T, typename Arg>
struct invoke_type<
    enable_if_t<is_same_v<reference_wrapper<T>, remove_cvref_t<Arg>> &&
                !is_function_v<Ret>>,
    Ret(T::*), Arg> {
    using type = invoke_memobj_refwrap;
};

template <typename Ret, typename T, typename Arg>
struct invoke_type<
    enable_if_t<is_base_of_v<T, remove_cvref_t<Arg>> && !is_function_v<Ret>>,
    Ret(T::*), Arg> {
    using type = invoke_memobj_ref;
};

template <typename Ret, typename T, typename T1, typename... Args>
struct invoke_type<
    enable_if_t<!is_same_v<reference_wrapper<T>, remove_cvref_t<T1>> &&
                !is_base_of_v<T, remove_cvref_t<T1>>>,
    Ret (T::*)(Args...), T1, Args...> {
    using type = invoke_memfun_ptr;
};

template <typename Ret, typename T, typename T1, typename... Args>
struct invoke_type<
    enable_if_t<is_same_v<reference_wrapper<T>, remove_cvref_t<T1>>>,
    Ret (T::*)(Args...), T1, Args...> {
    using type = invoke_memfun_refwrap;
};

template <typename Ret, typename T, typename T1, typename... Args>
struct invoke_type<enable_if_t<is_base_of_v<T, remove_cvref_t<T1>>>,
                   Ret (T::*)(Args...), T1, Args...> {
    using type = invoke_memfun_ref;
};
}  // namespace helper

namespace helper {

template <typename Res, typename Fn, typename... Args>
constexpr Res invoke_impl(helper::invoke_other, Fn&& f, Args&&... args) {
    return forward<Fn>(f)(forward<Args>(args)...);
}

template <typename Res, typename MemObjPtr, typename T>
constexpr Res invoke_impl(helper::invoke_memobj_ptr, MemObjPtr&& f, T&& t) {
    return (*forward<T>(t).*f);
}

template <typename Res, typename MemObjPtr, typename RefWrap>
constexpr Res invoke_impl(helper::invoke_memobj_refwrap, MemObjPtr&& f,
                          RefWrap&& t) {
    return (t.get().*f);
}

template <typename Res, typename MemObjPtr, typename T>
constexpr Res invoke_impl(helper::invoke_memobj_ref, MemObjPtr&& f, T&& t) {
    return (forward<T>(t).*f);
}

template <typename Res, typename MemFunPtr, typename T, typename... Args>
constexpr Res invoke_impl(helper::invoke_memfun_ptr, MemFunPtr&& f, T&& t,
                          Args&&... args) {
    return (*forward<T>(t).*f)(forward<Args>(args)...);
}

template <typename Res, typename MemFunPtr, typename RefWrap, typename... Args>
constexpr Res invoke_impl(helper::invoke_memfun_refwrap, MemFunPtr&& f,
                          RefWrap&& t, Args&&... args) {
    return (t.get().*f)(forward<Args>(args)...);
}

template <typename Res, typename MemFunPtr, typename T, typename... Args>
constexpr Res invoke_impl(helper::invoke_memfun_ref, MemFunPtr&& f, T&& t,
                          Args&&... args) {
    return (forward<T>(t).*f)(forward<Args>(args)...);
}

template <typename Callable, typename... Args>
constexpr invoke_result_t<Callable, Args...> invoke(Callable&& f,
                                                    Args&&... args) {
    return helper::invoke_impl<invoke_result_t<Callable, Args...>>(
        typename helper::invoke_type<void, Callable, Args...>::type{},
        forward<Callable>(f), forward<Args>(args)...);
}
}  // namespace helper

template <typename Callable, typename... Args>
inline constexpr invoke_result_t<Callable, Args...> invoke(Callable&& f,
                                                           Args&&... args) {
    return helper::invoke(forward<Callable>(f), forward<Args>(args)...);
}

}  // namespace impl
}  // namespace std
