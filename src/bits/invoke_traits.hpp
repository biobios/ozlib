#pragma once

#include <bits/cvref_traits.hpp>
#include <bits/decay.hpp>
#include <bits/declval.hpp>
#include <bits/fundamental_traits.hpp>
#include <bits/inheritance_traits.hpp>
#include <bits/template_logics.hpp>

namespace std {
namespace impl {

template <typename T>
class reference_wrapper;

namespace helper_invoke_result {

template <typename...>
struct invoke_result_helper {};

template <typename F, typename T1, typename T2, typename... Args>
    requires(is_base_of_v<T1, remove_cvref_t<T2>> && is_function_v<F> &&
             requires(F T1::*f, T2 t2, Args... args) { (t2.*f)(args...); })
struct invoke_result_helper<F T1::*, T2, Args...> {
    using type =
        decltype((declval<T2>().*declval<F T1::*>())(declval<Args>()...));
};

template <typename F, typename T1, typename T2, typename... Args>
    requires(
        is_same_v<reference_wrapper<T1>, remove_cvref_t<T2>> &&
        is_function_v<F> &&
        requires(F T1::*f, T2 t2, Args... args) { (t2.get().*f)(args...); })
struct invoke_result_helper<F T1::*, T2, Args...> {
    using type =
        decltype((declval<T2>().get().*declval<F T1::*>())(declval<Args>()...));
};

template <typename F, typename T1, typename T2, typename... Args>
    requires(requires(F T1::*f, T2 t2, Args... args) { ((*t2).*f)(args...); })
struct invoke_result_helper<F T1::*, T2, Args...> {
    using type =
        decltype(((*declval<T2>()).*declval<F T1::*>())(declval<Args>()...));
};

template <typename R, typename T, typename Arg>
    requires(!is_function_v<R> && is_base_of_v<T, remove_cvref_t<Arg>> &&
             requires(R T::*f, Arg t1) { (t1.*f); })
struct invoke_result_helper<R T::*, Arg> {
    using type = decltype(declval<Arg>().*declval<R T::*>());
};

template <typename R, typename T, typename Arg>
    requires(!is_function_v<R> &&
             is_same_v<reference_wrapper<T>, remove_cvref_t<Arg>> &&
             requires(R T::*f, Arg t1) { (t1.get().*f); })
struct invoke_result_helper<R T::*, Arg> {
    using type = decltype(declval<Arg>().get().*declval<R T::*>());
};

template <typename R, typename T, typename Arg>
    requires(!is_function_v<R> && requires(R T::*f, Arg t1) { ((*t1).*f); })
struct invoke_result_helper<R T::*, Arg> {
    using type = decltype((*declval<Arg>()).*declval<R T::*>());
};

template <typename F, typename... Args>
    requires(requires(F f, Args... args) { f(args...); })
struct invoke_result_helper<F, Args...> {
    using type = decltype(declval<F>()(declval<Args>()...));
};

}  // namespace helper_invoke_result

template <typename F, typename... Args>
struct invoke_result : helper_invoke_result::invoke_result_helper<
                           conditional_t<is_class_v<remove_cvref_t<F>> ||
                                             is_union_v<remove_cvref_t<F>>,
                                         F, remove_cvref_t<F>>,
                           Args...> {
    // remove_cvref_t<F>が
    // メンバ変数ポインタ、メンバ関数ポインタ、関数ポインタであれば、remove_cvref_t<F>
    // オブジェクト(class or union)の時は、
    // Fをそのままinvoker_result_helperに渡す

    // オブジェクトの場合は、それのconst、volatile、右辺値、左辺値どれであるかによって
    // 呼び出される関数が変わることがあるから
};

template <typename F, typename... Args>
using invoke_result_t = typename invoke_result<F, Args...>::type;

}  // namespace impl
}  // namespace std