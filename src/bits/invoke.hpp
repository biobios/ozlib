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

namespace helper_invoke {
template <typename F, typename... Args>
struct invoke_helper {
    template <typename Callable>
    inline static constexpr decltype(auto) invoke(Callable&& f,
                                                  Args&&... args) {
        return forward<Callable>(f)(forward<Args>(args)...);
    }
};

template <typename F, typename T1, typename T2, typename... Args>
    requires(is_base_of_v<T1, remove_cvref_t<T2>> && is_function_v<F>)
struct invoke_helper<F T1::*, T2, Args...> {
    inline static constexpr decltype(auto) invoke(F T1::*f, T2&& t2,
                                                  Args&&... args) {
        return (forward<T2>(t2).*f)(forward<Args>(args)...);
    }
};

template <typename F, typename T1, typename T2, typename... Args>
    requires(is_same_v<reference_wrapper<T1>, remove_cvref_t<T2>> &&
             is_function_v<F>)
struct invoke_helper<F T1::*, T2, Args...> {
    inline static constexpr decltype(auto) invoke(F T1::*f, T2&& t2,
                                                  Args&&... args) {
        return (t2.get().*f)(forward<Args>(args)...);
    }
};

template <typename F, typename T1, typename T2, typename... Args>
    requires(is_function_v<F> &&
             !(is_base_of_v<T1, remove_cvref_t<T2>> ||
               is_same_v<reference_wrapper<T1>, remove_cvref_t<T2>>))
struct invoke_helper<F T1::*, T2, Args...> {
    inline static constexpr decltype(auto) invoke(F T1::*f, T2&& t2,
                                                  Args&&... args) {
        return ((*t2).*f)(forward<Args>(args)...);
    }
};

template <typename R, typename T, typename Arg>
    requires(is_base_of_v<T, remove_cvref_t<Arg>> && !is_function_v<R>)
struct invoke_helper<R T::*, Arg> {
    inline static constexpr decltype(auto) invoke(R T::*f, Arg&& t1) {
        return (forward<Arg>(t1).*f);
    }
};

template <typename R, typename T, typename Arg>
    requires(is_same_v<reference_wrapper<T>, remove_cvref_t<Arg>> &&
             !is_function_v<R>)
struct invoke_helper<R T::*, Arg> {
    inline static constexpr decltype(auto) invoke(R T::*f, Arg&& t1) {
        return (t1.get().*f);
    }
};

template <typename R, typename T, typename Arg>
    requires(!is_function_v<R> &&
             !(is_base_of_v<T, remove_cvref_t<Arg>> ||
               is_same_v<reference_wrapper<T>, remove_cvref_t<Arg>>))
struct invoke_helper<R T::*, Arg> {
    inline static constexpr decltype(auto) invoke(R T::*f, Arg&& t1) {
        return ((*t1).*f);
    }
};

}  // namespace helper_invoke

template <typename Callable, typename... Args>
constexpr invoke_result_t<Callable, Args...> invoke(Callable&& f,
                                                    Args&&... args) {
    return helper_invoke::invoke_helper<remove_cvref_t<Callable>,
                                        Args...>::invoke(forward<Callable>(f),
                                                         forward<Args>(
                                                             args)...);
}

}  // namespace impl
}  // namespace std
