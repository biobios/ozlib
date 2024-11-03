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
    inline static constexpr auto invoke(Callable&& f, Args&&... args) {
        return forward<Callable>(f)(forward<Args>(args)...);
    }
};

template <typename R, typename T1, typename... Args1, typename T2,
          typename... Args2>
    requires(is_base_of_v<T1, remove_cvref_t<T2>>)
struct invoke_helper<R (T1::*)(Args1...), T2, Args2...> {
    inline static constexpr R invoke(R (T1::*f)(Args1...), T2&& t2,
                                     Args2&&... args2) {
        return (forward<T2>(t2).*f)(forward<Args2>(args2)...);
    }
};

template <typename R, typename T1, typename... Args1, typename T2,
          typename... Args2>
    requires(is_same_v<reference_wrapper<T1>, remove_cvref_t<T2>>)
struct invoke_helper<R (T1::*)(Args1...), T2, Args2...> {
    inline static constexpr R invoke(R (T1::*f)(Args1...), T2 t2,
                                     Args2&&... args2) {
        return (t2.get().*f)(forward<Args2>(args2)...);
    }
};

template <typename R, typename T1, typename... Args1, typename T2,
          typename... Args2>
struct invoke_helper<R (T1::*)(Args1...), T2, Args2...> {
    inline static constexpr R invoke(R (T1::*f)(Args1...), T2 t2,
                                     Args2&&... args2) {
        return ((*t2).*f)(forward<Args2>(args2)...);
    }
};

template <typename R, typename T, typename Arg>
    requires(is_base_of_v<T, remove_cvref_t<Arg>>)
struct invoke_helper<R T::*, Arg> {
    inline static constexpr R invoke(R T::*f, Arg&& t1) { return (t1.*f); }
};

template <typename R, typename T, typename Arg>
    requires(is_same_v<reference_wrapper<T>, remove_cvref_t<Arg>>)
struct invoke_helper<R T::*, Arg> {
    inline static constexpr R invoke(R T::*f, Arg&& t1) {
        return (t1.get().*f);
    }
};

template <typename R, typename T, typename Arg>
struct invoke_helper<R T::*, Arg> {
    inline static constexpr R invoke(R T::*f, Arg&& t1) { return ((*t1).*f); }
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
