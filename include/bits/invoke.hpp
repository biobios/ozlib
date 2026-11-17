#pragma once

#include <bits/namespace.hpp>

#include <bits/cvref_traits.hpp>
#include <bits/forward.hpp>
#include <bits/fundamental_traits.hpp>
#include <bits/inheritance_traits.hpp>
#include <bits/invoke_traits.hpp>
#include <bits/template_logics.hpp>

namespace OZLIB_NAMESPACE {
namespace impl {

template <typename T>
class reference_wrapper;

namespace helper_invoke {
template <typename F, typename... Args>
struct invoke_helper {
    template <typename Callable>
    inline static constexpr decltype(auto) invoke(Callable&& f,
                                                  Args&&... args) noexcept(noexcept(forward<Callable>(f)(forward<Args>(args)...))) {
        return forward<Callable>(f)(forward<Args>(args)...);
    }
};

template <typename F, typename T1, typename T2, typename... Args>
    requires(is_base_of_v<T1, remove_cvref_t<T2>> && is_function_v<F>)
struct invoke_helper<F T1::*, T2, Args...> {
    inline static constexpr decltype(auto) invoke(F T1::*f, T2&& t2,
                                                  Args&&... args) noexcept(noexcept((forward<T2>(t2).*f)(forward<Args>(args)...))) {
        return (forward<T2>(t2).*f)(forward<Args>(args)...);
    }
};

template <typename F, typename T1, typename T2, typename... Args>
    requires(is_same_v<reference_wrapper<T1>, remove_cvref_t<T2>> &&
             is_function_v<F>)
struct invoke_helper<F T1::*, T2, Args...> {
    inline static constexpr decltype(auto) invoke(F T1::*f, T2&& t2,
                                                  Args&&... args) noexcept(noexcept((t2.get().*f)(forward<Args>(args)...))) {
        return (t2.get().*f)(forward<Args>(args)...);
    }
};

template <typename F, typename T1, typename T2, typename... Args>
    requires(is_function_v<F> &&
             !(is_base_of_v<T1, remove_cvref_t<T2>> ||
               is_same_v<reference_wrapper<T1>, remove_cvref_t<T2>>))
struct invoke_helper<F T1::*, T2, Args...> {
    inline static constexpr decltype(auto) invoke(F T1::*f, T2&& t2,
                                                  Args&&... args) noexcept(noexcept(((*t2).*f)(forward<Args>(args)...))) {
        return ((*t2).*f)(forward<Args>(args)...);
    }
};

template <typename R, typename T, typename Arg>
    requires(is_base_of_v<T, remove_cvref_t<Arg>> && !is_function_v<R>)
struct invoke_helper<R T::*, Arg> {
    inline static constexpr decltype(auto) invoke(R T::*f, Arg&& t1) noexcept(noexcept(forward<Arg>(t1).*f)) {
        return (forward<Arg>(t1).*f);
    }
};

template <typename R, typename T, typename Arg>
    requires(is_same_v<reference_wrapper<T>, remove_cvref_t<Arg>> &&
             !is_function_v<R>)
struct invoke_helper<R T::*, Arg> {
    inline static constexpr decltype(auto) invoke(R T::*f, Arg&& t1) noexcept(noexcept(t1.get().*f)) {
        return (t1.get().*f);
    }
};

template <typename R, typename T, typename Arg>
    requires(!is_function_v<R> &&
             !(is_base_of_v<T, remove_cvref_t<Arg>> ||
               is_same_v<reference_wrapper<T>, remove_cvref_t<Arg>>))
struct invoke_helper<R T::*, Arg> {
    inline static constexpr decltype(auto) invoke(R T::*f, Arg&& t1) noexcept(noexcept((*t1).*f)) {
        return ((*t1).*f);
    }
};

}  // namespace helper_invoke

template <typename Callable, typename... Args>
struct is_nothrow_invocable : false_type {};

template <typename Callable, typename... Args>
    requires requires(Callable&& f, Args&&... args) {
        { helper_invoke::invoke_helper<remove_cvref_t<Callable>, Args...>::invoke(forward<Callable>(f), forward<Args>(args)...) } noexcept;
    }
struct is_nothrow_invocable<Callable, Args...> : true_type {};

template <typename Callable, typename... Args>
inline constexpr bool is_nothrow_invocable_v = is_nothrow_invocable<Callable, Args...>::value;

template <typename Callable, typename... Args>
constexpr invoke_result_t<Callable, Args...> invoke(Callable&& f,
                                                    Args&&... args) noexcept(is_nothrow_invocable_v<Callable, Args...>) {
    return helper_invoke::invoke_helper<remove_cvref_t<Callable>,
                                        Args...>::invoke(forward<Callable>(f),
                                                         forward<Args>(
                                                             args)...);
}

template <typename F, typename... Args>
concept invocable = requires(F&& f, Args&&... args) {
    invoke(forward<F>(f), forward<Args>(args)...);
};

template <typename F, typename... Args>
concept regular_invocable = invocable<F, Args...>;

}  // namespace impl
}  // namespace OZLIB_NAMESPACE
