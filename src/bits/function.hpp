#pragma once

#include <bits/convertible.hpp>
#include <bits/decay.hpp>
#include <bits/declval.hpp>
#include <bits/forward.hpp>
#include <bits/nullptr_t.hpp>
#include <bits/reference_traits.hpp>

namespace std {
namespace impl {

/// @brief
/// 関数ポインタ、関数オブジェクト、メンバ関数ポインタ、メンバ変数ポインタを保持するクラス
/// @tparam R 関数の戻り値の型
/// @tparam ArgTypes 関数の引数の型
template <typename>
class function;

template <typename R, typename... ArgTypes>
class function<R(ArgTypes...)> {
    union func_ptr_or_value_ptr {
        void* value_ptr;
        void (*func_ptr)(void);
    };

    using invoker_type = R (*)(func_ptr_or_value_ptr, ArgTypes...);
    using destructor_type = void (*)(func_ptr_or_value_ptr);
    using copy_constructor_type = void (*)(func_ptr_or_value_ptr*,
                                           func_ptr_or_value_ptr);

    invoker_type _invoker;
    destructor_type _destructor;
    copy_constructor_type _copy_constructor;
    func_ptr_or_value_ptr _func;

    template <typename...>
    struct invoker_member_function;

    // メンバ関数ポインタを呼び出す関数(第一引数がポインタではない)
    template <typename FuncType, typename FirstArg, typename... RestArgs>
        requires(
            requires(FuncType func, FirstArg first_arg, RestArgs... rest_args) {
                {
                    (first_arg.*func)(forward<RestArgs>(rest_args)...)
                } -> same_as<R>;
            })
    struct invoker_member_function<FuncType, FirstArg, RestArgs...> {
       private:
        FuncType _func;
        invoker_member_function(FuncType func) : _func{func} {}

       public:
        static void* create(FuncType func) {
            return new invoker_member_function{func};
        }
        static R invoke(func_ptr_or_value_ptr func, FirstArg first_arg,
                        RestArgs... rest_args) {
            auto self = static_cast<invoker_member_function*>(func.value_ptr);
            return (first_arg.*self->_func)(forward<RestArgs>(rest_args)...);
        }
        static void destroy(func_ptr_or_value_ptr func) {
            delete static_cast<invoker_member_function*>(func.value_ptr);
        }
        static void copy_constructor(func_ptr_or_value_ptr* dest,
                                     func_ptr_or_value_ptr src) {
            dest->value_ptr = new invoker_member_function(
                *static_cast<invoker_member_function*>(src.value_ptr));
        }
    };

    // メンバ関数ポインタを呼び出す関数(第一引数がポインタ)
    template <typename FuncType, typename FirstArg, typename... RestArgs>
        requires(
            requires(FuncType func, FirstArg first_arg, RestArgs... rest_args) {
                {
                    (first_arg->*func)(forward<RestArgs>(rest_args)...)
                } -> same_as<R>;
            })
    struct invoker_member_function<FuncType, FirstArg, RestArgs...> {
       private:
        FuncType _func;
        invoker_member_function(FuncType func) : _func{func} {}

       public:
        static void* create(FuncType func) {
            return new invoker_member_function{func};
        }
        static R invoke(func_ptr_or_value_ptr func, FirstArg first_arg,
                        RestArgs... rest_args) {
            auto self = static_cast<invoker_member_function*>(func.value_ptr);
            return (first_arg->*self->_func)(forward<RestArgs>(rest_args)...);
        }
        static void destroy(func_ptr_or_value_ptr func) {
            delete static_cast<invoker_member_function*>(func.value_ptr);
        }
        static void copy_constructor(func_ptr_or_value_ptr* dest,
                                     func_ptr_or_value_ptr src) {
            dest->value_ptr = new invoker_member_function(
                *static_cast<invoker_member_function*>(src.value_ptr));
        }
    };

    template <typename FuncType, typename FirstArg>
    struct invoker_member_variable {
        static_assert(!is_lvalue_reference_v<R> || is_reference_v<FirstArg>,
                      "argument type must be reference type if return type is "
                      "lvalue reference type");
        static_assert(!is_lvalue_reference_v<R> ||
                          !is_rvalue_reference_v<FirstArg>,
                      "rvalue cannot be assigned to lvalue reference");
        static_assert(!is_rvalue_reference_v<R> ||
                          !is_lvalue_reference_v<FirstArg>,
                      "lvalue cannot be assigned to rvalue reference");
        static void* create(FuncType);
        static R invoke(func_ptr_or_value_ptr, FirstArg);
        static void destroy(func_ptr_or_value_ptr);
        static void copy_constructor(func_ptr_or_value_ptr*,
                                     func_ptr_or_value_ptr);
    };

    // メンバ変数ポインタを呼び出す関数（第一引数がポインタではない）
    template <typename FuncType, typename FirstArg>
        requires(is_convertible_v<decltype(std::impl::declval<FirstArg>().*
                                           std::impl::declval<FuncType>()),
                                  R>)
    struct invoker_member_variable<FuncType, FirstArg> {
       private:
        FuncType _func;
        invoker_member_variable(FuncType func) : _func{func} {}

       public:
        static void* create(FuncType func) {
            return new invoker_member_variable{func};
        }
        static R invoke(func_ptr_or_value_ptr func, FirstArg first_arg) {
            auto self = static_cast<invoker_member_variable*>(func.value_ptr);
            return forward<FirstArg>(first_arg).*self->_func;
        }
        static void destroy(func_ptr_or_value_ptr func) {
            delete static_cast<invoker_member_variable*>(func.value_ptr);
        }
        static void copy_constructor(func_ptr_or_value_ptr* dest,
                                     func_ptr_or_value_ptr src) {
            dest->value_ptr = new invoker_member_variable(
                *static_cast<invoker_member_variable*>(src.value_ptr));
        }
    };

    // メンバ変数ポインタを呼び出す関数（第一引数がポインタ）
    template <typename FuncType, typename FirstArg>
        requires(is_convertible_v<
                 decltype(declval<FirstArg>()->*declval<FuncType>()), R>)
    struct invoker_member_variable<FuncType, FirstArg> {
       private:
        FuncType _func;
        invoker_member_variable(FuncType func) : _func{func} {}

       public:
        static void* create(FuncType func) {
            return new invoker_member_variable{func};
        }
        static R invoke(func_ptr_or_value_ptr func, FirstArg first_arg) {
            auto self = static_cast<invoker_member_variable*>(func.value_ptr);
            return first_arg->*self->_func;
        }
        static void destroy(func_ptr_or_value_ptr func) {
            delete static_cast<invoker_member_variable*>(func.value_ptr);
        }
        static void copy_constructor(func_ptr_or_value_ptr* dest,
                                     func_ptr_or_value_ptr src) {
            dest->value_ptr = new invoker_member_variable(
                *static_cast<invoker_member_variable*>(src.value_ptr));
        }
    };

    // 関数オブジェクトで初期化する
    template <typename F>
    void init(F&& f) {
        // 関数オブジェクトはサイズが一定ではないため、動的確保する
        _func.value_ptr = new F(forward<F>(f));
        _invoker = [](func_ptr_or_value_ptr func, ArgTypes... args) -> R {
            return (*static_cast<F*>(func.value_ptr))(
                forward<ArgTypes>(args)...);
        };
        _destructor = [](func_ptr_or_value_ptr func) {
            delete static_cast<F*>(func.value_ptr);
        };
        _copy_constructor = [](func_ptr_or_value_ptr* dest,
                               func_ptr_or_value_ptr src) {
            dest->value_ptr = new F(*static_cast<const F*>(src.value_ptr));
        };
    }

    // 関数ポインタで初期化する
    void init(R (*f)(ArgTypes...)) {
        _func.func_ptr = reinterpret_cast<void (*)()>(f);
        _invoker = [](func_ptr_or_value_ptr func, ArgTypes... args) -> R {
            return reinterpret_cast<R (*)(ArgTypes...)>(func.func_ptr)(
                forward<ArgTypes>(args)...);
        };
        _destructor = [](func_ptr_or_value_ptr) {};
        _copy_constructor = [](func_ptr_or_value_ptr* dest,
                               func_ptr_or_value_ptr src) {
            dest->func_ptr = src.func_ptr;
        };
    }

    // メンバ関数ポインタで初期化する
    template <typename T, typename... RestArgs>
    void init(R (T::*f)(RestArgs...)) {
        // メンバ関数ポインタはサイズが一定ではないため、動的確保する
        _func.value_ptr =
            invoker_member_function<decltype(f), ArgTypes...>::create(f);
        _invoker = invoker_member_function<decltype(f), ArgTypes...>::invoke;
        _destructor =
            invoker_member_function<decltype(f), ArgTypes...>::destroy;
        _copy_constructor =
            invoker_member_function<decltype(f), ArgTypes...>::copy_constructor;
    }

    // メンバ変数ポインタで初期化する
    template <typename RetType, typename T>
    void init(RetType T::*f) {
        // メンバ変数ポインタはサイズが一定ではないので、動的確保する
        _func.value_ptr =
            invoker_member_variable<decltype(f), ArgTypes...>::create(f);
        _invoker = invoker_member_variable<decltype(f), ArgTypes...>::invoke;
        _destructor =
            invoker_member_variable<decltype(f), ArgTypes...>::destroy;
        _copy_constructor =
            invoker_member_variable<decltype(f), ArgTypes...>::copy_constructor;
    }

   public:
    using result_type = R;
    function() noexcept {
        _invoker = nullptr;
        _destructor = nullptr;
        _copy_constructor = nullptr;
        _func = nullptr;
    }
    function(nullptr_t) noexcept {
        _invoker = nullptr;
        _destructor = nullptr;
        _copy_constructor = nullptr;
        _func = nullptr;
    }
    function(const function& f) {
        _invoker = f._invoker;
        _destructor = f._destructor;
        _copy_constructor = f._copy_constructor;
        _func = nullptr;
        if (f._func != nullptr) {
            _copy_constructor(&_func, f._func);
        }
    }
    function(function&& f) noexcept {
        _invoker = f._invoker;
        _destructor = f._destructor;
        _copy_constructor = f._copy_constructor;
        _func = f._func;
        f._invoker = nullptr;
        f._func = nullptr;
        f._destructor = nullptr;
        f._copy_constructor = nullptr;
    }
    template <typename F>
    function(F f) {
        static_assert(
            requires(const F& f) {
                { F(f) } noexcept;
            }, "F must be copy constructible without throwing exceptions");
        static_assert(
            requires(F* f) {
                { delete f } noexcept;
            }, "F must be destructible without throwing exceptions");
        init(forward<F>(f));
    }
    ~function() {
        if (_func.value_ptr != nullptr) {
            _destructor(_func);
        }
    }
    function& operator=(const function& f) {
        function(f).swap(*this);
        return *this;
    }
    function& operator=(function&& f) {
        if (_func.value_ptr != nullptr) {
            _destructor(_func);
        }
        _invoker = f._invoker;
        _destructor = f._destructor;
        _copy_constructor = f._copy_constructor;
        _func = f._func;
        f._invoker = nullptr;
        f._func = nullptr;
        f._destructor = nullptr;
        f._copy_constructor = nullptr;
        return *this;
    }
    function& operator=(nullptr_t) {
        if (_func.value_ptr != nullptr) {
            _destructor(_func);
        }
        _invoker = nullptr;
        _destructor = nullptr;
        _copy_constructor = nullptr;
        _func = nullptr;
        return *this;
    }
    template <typename F>
        requires(requires(decay<F> f, ArgTypes... args) {
            { f(forward<ArgTypes>(args)...) } -> same_as<R>;
        })
    function& operator=(F&& f) {
        function(forward<F>(f)).swap(*this);
        return *this;
    }
    // todo: 実装する
    // template <typename F>
    // function& operator=(reference_wrapper<F> f) noexcept;
    void swap(function& other) noexcept {
        auto tmp_invoker = _invoker;
        auto tmp_destructor = _destructor;
        auto tmp_copy_constructor = _copy_constructor;
        auto tmp_func = _func;
        _invoker = other._invoker;
        _destructor = other._destructor;
        _copy_constructor = other._copy_constructor;
        _func = other._func;
        other._invoker = tmp_invoker;
        other._destructor = tmp_destructor;
        other._copy_constructor = tmp_copy_constructor;
        other._func = tmp_func;
    }
    explicit operator bool() const noexcept { return _func != nullptr; }
    R operator()(ArgTypes... args) const {
        return _invoker(_func, forward<ArgTypes>(args)...);
    }
    // todo: type_infoを実装後に実装する
    // const type_info& target_type() const noexcept;
    // template <typename T>
    //     requires(requires(T t, ArgTypes... args) {
    //         { t(forward<ArgTypes>(args)...) } -> same_as<R>;
    //     })
    // T* target() noexcept;
    // template <typename T>
    //     requires(requires(T t, ArgTypes... args) {
    //         { t(forward<ArgTypes>(args)...) } -> same_as<R>;
    //     })
    // T* target() noexcept;
};

template <typename R, typename... ArgTypes>
function(R (*)(ArgTypes...)) -> function<R(ArgTypes...)>;

template <typename R, typename... ArgTypes>
bool operator==(const function<R(ArgTypes...)>& f, nullptr_t) noexcept {
    return !f;
};

template <typename R, typename... ArgTypes>
bool operator==(nullptr_t, const function<R(ArgTypes...)>& f) noexcept {
    return !f;
};

template <typename R, typename... ArgTypes>
bool operator!=(const function<R(ArgTypes...)>& f, nullptr_t) noexcept {
    return static_cast<bool>(f);
};

template <typename R, typename... ArgTypes>
bool operator!=(nullptr_t, const function<R(ArgTypes...)>& f) noexcept {
    return static_cast<bool>(f);
};

template <typename R, typename... ArgTypes>
void swap(function<R(ArgTypes...)>& f1, function<R(ArgTypes...)>& f2) noexcept {
    f1.swap(f2);
};

}  // namespace impl
}  // namespace std
