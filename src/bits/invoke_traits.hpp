#pragma once

#include <bits/cvref_traits.hpp>
#include <bits/fundamental_traits.hpp>
#include <bits/inheritance_traits.hpp>
#include <bits/template_logics.hpp>

namespace std {
namespace impl {

template <typename T>
class reference_wrapper;

template <typename...>
struct invoke_result {};

template <typename R, typename T1, typename... Args1, typename T2,
          typename... Args2>
    requires(is_base_of_v<T1, remove_cvref_t<T2>> &&
             requires(R (T1::*f)(Args1...), T2 t2, Args2... args2) {
                 (t2.*f)(args2...);
             })
struct invoke_result<R (T1::*)(Args1...), T2, Args2...> {
    using type = R;
};

template <typename R, typename T1, typename... Args1, typename T2,
          typename... Args2>
    requires(is_same_v<reference_wrapper<T1>, remove_cvref_t<T2>> &&
             requires(R (T1::*f)(Args1...), T2 t2, Args2... args2) {
                 (t2.get().*f)(args2...);
             })
struct invoke_result<R (T1::*)(Args1...), T2, Args2...> {
    using type = R;
};

template <typename R, typename T1, typename... Args1, typename T2,
          typename... Args2>
    requires(requires(R (T1::*f)(Args1...), T2 t2, Args2... args2) {
        ((*t2).*f)(args2...);
    })
struct invoke_result<R (T1::*)(Args1...), T2, Args2...> {
    using type = R;
};

template <typename R, typename T, typename Arg>
    requires(is_base_of_v<T, remove_cvref_t<Arg>> &&
             requires(R T::*f, Arg t1) { (t1.*f); })
struct invoke_result<R T::*, Arg> {
    using type = R;
};

template <typename R, typename T, typename Arg>
    requires(is_same_v<reference_wrapper<T>, remove_cvref_t<Arg>> &&
             requires(R T::*f, Arg t1) { (t1.get().*f); })
struct invoke_result<R T::*, Arg> {
    using type = R;
};

template <typename R, typename T, typename Arg>
    requires(requires(R T::*f, Arg t1) { ((*t1).*f); })
struct invoke_result<R T::*, Arg> {
    using type = R;
};

template <typename F, typename... Args>
    requires(requires(F f, Args... args) { f(args...); })
struct invoke_result<F, Args...> {
    using type = decltype(declval<F>()(declval<Args>()...));
};

template <typename T, typename... Args>
using invoke_result_t = typename invoke_result<T, Args...>::type;

}  // namespace impl
}  // namespace std