#pragma once

#include <bits/convertible.hpp>
#include <bits/cvref_traits.hpp>
#include <bits/decay.hpp>
#include <bits/forward.hpp>
#include <bits/fundamental_traits.hpp>
#include <bits/invoke.hpp>
#include <bits/move.hpp>
#include <bits/nullptr_t.hpp>
#include <bits/reference_wrapper.hpp>
#include <bits/typeid.hpp>

namespace std {
namespace impl {

namespace helper_function {
template <typename From, typename R>
concept convertible_to_return_type_implicitly = is_convertible_v<From, R>;
}

template <typename>
class function;

/// @class function
/// @brief
/// 関数ポインタ、関数オブジェクト、メンバ関数ポインタ、メンバ変数ポインタを保持するクラス
/// @tparam F 関数型
template <typename R, typename... ArgTypes>
class function<R(ArgTypes...)> {
    struct func_or_val_ptr_wrapper {
        union {
            void* value_ptr;
            void (*func_ptr)(void);
        };

        func_or_val_ptr_wrapper() : value_ptr{nullptr} {}
        func_or_val_ptr_wrapper(void* ptr) : value_ptr{ptr} {}
        template <typename F>
            requires(is_function_v<F>)
        func_or_val_ptr_wrapper(F* ptr)
            : func_ptr{reinterpret_cast<void (*)()>(ptr)} {}

        func_or_val_ptr_wrapper(const func_or_val_ptr_wrapper& other) = default;
        func_or_val_ptr_wrapper(func_or_val_ptr_wrapper&& other) = default;
        func_or_val_ptr_wrapper& operator=(
            const func_or_val_ptr_wrapper& other) = default;
        func_or_val_ptr_wrapper& operator=(func_or_val_ptr_wrapper&& other) =
            default;

        template <typename F>
        F* get() const {
            if constexpr (is_function_v<F>) {
                return reinterpret_cast<F*>(func_ptr);
            } else {
                return static_cast<F*>(value_ptr);
            }
        }

        template <typename F>
        void set(F* ptr) {
            if constexpr (is_function_v<F>) {
                func_ptr = reinterpret_cast<void (*)()>(ptr);
            } else {
                value_ptr = ptr;
            }
        }

        void clear() { value_ptr = nullptr; }
        bool is_null() const {
            // value_ptr == nullptrのとき、
            // 関数ポインタが有効な値を保持していないことを前提とした実装
            return value_ptr == nullptr;
        }
    };

    class management_operand_base {
       public:
        virtual const type_info* type() = 0;
        virtual void* target() = 0;
        virtual void clone_callable(func_or_val_ptr_wrapper*) = 0;
        virtual void destroy_callable() = 0;
    };

    // メンバ変数ポインタ、メンバ関数ポインタ、reference_wrapperを除く関数オブジェクトを対象とするmanagement_operandの実装
    // (動的確保が必要な型に対する実装)
    template <typename F>
    class management_operand_for_dynamic_allocation
        : public management_operand_base {
        F* _target;

       public:
        management_operand_for_dynamic_allocation(F* target)
            : _target{target} {}
        const type_info* type() override {
            return std::helper_typeid::typeid_ptr<F>();
        }
        void* target() override { return _target; }
        void clone_callable(func_or_val_ptr_wrapper* ptr) override {
            ptr->set(new F(*_target));
        }
        void destroy_callable() override { delete _target; }
    };

    // reference_wrapperを対象とするmanagement_operandの実装
    template <typename F>
    class management_operand_for_reference_wrapper
        : public management_operand_base {
        // callable_managerのvalue_ptrを指すポインタ : void**
        // reference_wrapper<F> = F* として扱うことで、
        // 動的にメモリを確保する必要がなくなる
        // 条件: sizeof(reference_wrapper<F>) == sizeof(void*) &&
        //        alignof(reference_wrapper<F>) == alignof(void*)
        // 満たさないときは、他の関数オブジェクトと同様に動的確保する
        // callable_manager_impl<reference_wrapper>のrequire節でチェックする
        reference_wrapper<F>* _target;

       public:
        management_operand_for_reference_wrapper(reference_wrapper<F>* target)
            : _target{target} {}
        const type_info* type() override {
            return std::helper_typeid::typeid_ptr<reference_wrapper<F>>();
        }
        void* target() override { return _target; }
        void clone_callable(func_or_val_ptr_wrapper* ptr) override {
            ptr->set(_target);
        }
        void destroy_callable() override {}
    };

    // 関数ポインタを対象とするmanagement_operandの実装
    template <typename F>
    class management_operand_for_function_pointer;

    template <typename F>
        requires(is_function_v<F>)
    class management_operand_for_function_pointer<F**>
        : public management_operand_base {
        F** _target;

       public:
        management_operand_for_function_pointer(F** target) : _target{target} {}
        const type_info* type() override {
            return std::helper_typeid::typeid_ptr<F*>();
        }
        void* target() override { return _target; }
        void clone_callable(func_or_val_ptr_wrapper* ptr) override {
            ptr->set(_target);
        }
        void destroy_callable() override {}
    };

    class management_operator {
       public:
        virtual void visit(management_operand_base* operand) = 0;

        class get_type_info : public management_operator {
           public:
            const type_info* info;
            void visit(management_operand_base* operand) override {
                info = operand->type();
            }
        };

        template <typename T>
        class get_target : public management_operator {
           public:
            T* target;
            void visit(management_operand_base* operand) override {
                if (*operand->type() != *std::helper_typeid::typeid_ptr<T>()) {
                    target = nullptr;
                    return;
                }

                target = operand->target();
            }
        };

        class clone_callable : public management_operator {
            func_or_val_ptr_wrapper* _ptr_wrapper;

           public:
            clone_callable(func_or_val_ptr_wrapper* ptr_wrapper)
                : _ptr_wrapper{ptr_wrapper} {}
            void visit(management_operand_base* operand) override {
                operand->clone_callable(_ptr_wrapper);
            }
        };

        class destroy_callable : public management_operator {
           public:
            void visit(management_operand_base* operand) override {
                operand->destroy_callable();
            }
        };
    };

    class callable_manager {
        func_or_val_ptr_wrapper _data;

        using invoker_type = R (*)(func_or_val_ptr_wrapper const*,
                                   ArgTypes&&...);
        using manage_func_type = void (*)(func_or_val_ptr_wrapper*,
                                          management_operator*);

        invoker_type _invoker;
        manage_func_type _manage_func;

       public:
        callable_manager() noexcept
            : _data{}, _invoker{nullptr}, _manage_func{nullptr} {}

        callable_manager(callable_manager&& other) noexcept
            : _data{other._data},
              _invoker{other._invoker},
              _manage_func{other._manage_func} {
            other._data.clear();
            other._invoker = nullptr;
            other._manage_func = nullptr;
        }

        callable_manager(const callable_manager& other) noexcept {
            if (other._data.is_null()) {
                _invoker = nullptr;
                _manage_func = nullptr;
                return;
            }

            typename management_operator::clone_callable ope{&_data};
            other.operation(&ope);
            _invoker = other._invoker;
            _manage_func = other._manage_func;
        }

        template <typename F>
        callable_manager(F&& f) {
            using instanced_callable_manager_impl =
                callable_manager_impl<decay_t<F>>;
            instanced_callable_manager_impl::init(&this->_data, forward<F>(f));
            _invoker = instanced_callable_manager_impl::invoke;
            _manage_func = instanced_callable_manager_impl::operation;
        }

        callable_manager& operator=(callable_manager&& other) {
            this->reset();

            _data = other._data;
            _invoker = other._invoker;
            _manage_func = other._manage_func;
            other._data.value_ptr = nullptr;
            other._invoker = nullptr;
            other._manage_func = nullptr;
            return *this;
        }

        callable_manager& operator=(const callable_manager& other) {
            this->reset();

            typename management_operator::clone_callable ope{&_data};
            other.operation(&ope);
            _invoker = other._invoker;
            _manage_func = other._manage_func;
            return *this;
        }

        void reset() {
            if (_data.is_null()) return;
            typename management_operator::destroy_callable ope;
            operation(&ope);
            _data.clear();
            _invoker = nullptr;
            _manage_func = nullptr;
        }

        ~callable_manager() { reset(); }

        static void swap(callable_manager& lhs, callable_manager& rhs) {
            callable_manager tmp{std::impl::move(lhs)};
            lhs = std::impl::move(rhs);
            rhs = std::impl::move(tmp);
        }

        R invoke(ArgTypes&&... args) const {
            // if (!has_callable()) throw bad_function_call{};
            return _invoker(&_data, forward<ArgTypes>(args)...);
        }

        void operation(management_operator* ope) const {
            // management_operatorと、management_operand間は型情報が失われているため、
            // どこかでconst修飾をはずす必要がある
            // const用のstructや関数を用意すれば、const_castを使わずに済むが、
            // ほぼ同じコードを書くことになるため、const_castを使う
            // もし、const_castを使いたくない場合は、
            // operatorとoperandをconstと非constで分ける
            // operatorはget_target_not_constのみ非const用のoperator_baseを継承する
            // operandはget_target_not_constのみ非const用のoperand_baseを継承する
            // operation関数を2種類用意する

            if (!has_callable()) return;
            _manage_func(const_cast<func_or_val_ptr_wrapper*>(&_data), ope);
        }

        bool has_callable() const { return !_data.is_null(); }

       private:
        // メンバ変数ポインタ、メンバ関数ポインタ、reference_wrapperを除く関数オブジェクトを対象とするcallable_managerの実装
        template <typename FD>
        class callable_manager_impl {
           public:
            template <typename F>
            static void init(func_or_val_ptr_wrapper* data, F&& f) {
                data->set(new FD(forward<F>(f)));
            }

            static R invoke(func_or_val_ptr_wrapper const* data,
                            ArgTypes&&... args) {
                return std::impl::invoke(*data->template get<FD>(),
                                         forward<ArgTypes>(args)...);
            }

            static void operation(func_or_val_ptr_wrapper* data,
                                  management_operator* ope) {
                management_operand_for_dynamic_allocation<FD> operand{
                    data->template get<FD>()};
                ope->visit(&operand);
            }
        };

        // reference_wrapperを対象とするcallable_managerの実装
        template <typename F>
            requires(sizeof(reference_wrapper<F>) == sizeof(void*) &&
                     alignof(reference_wrapper<F>) == alignof(void*))
        class callable_manager_impl<reference_wrapper<F>> {
           public:
            static void init(func_or_val_ptr_wrapper* data,
                             reference_wrapper<F>& f) {
                data->set(&f.get());
            }

            static R invoke(func_or_val_ptr_wrapper const* data,
                            ArgTypes&&... args) {
                return std::impl::invoke(
                    reference_wrapper<F>{*data->template get<F>()},
                    forward<ArgTypes>(args)...);
            }

            static void operation(func_or_val_ptr_wrapper* data,
                                  management_operator* ope) {
                management_operand_for_reference_wrapper<F> operand{
                    reinterpret_cast<reference_wrapper<F>*>(&data->value_ptr)};
                ope->visit(&operand);
            }
        };

        // 関数ポインタを対象とするcallable_managerの実装
        template <typename F>
            requires(is_function_v<F>)
        class callable_manager_impl<F*> {
           public:
            static void init(func_or_val_ptr_wrapper* data, F* f) {
                data->set(f);
            }

            static R invoke(func_or_val_ptr_wrapper const* data,
                            ArgTypes&&... args) {
                return std::impl::invoke(data->template get<F>(),
                                         forward<ArgTypes>(args)...);
            }

            static void operation(func_or_val_ptr_wrapper* data,
                                  management_operator* ope) {
                management_operand_for_function_pointer<F**> operand{
                    reinterpret_cast<F**>(&data->func_ptr)};
                ope->visit(&operand);
            }
        };
    };

    callable_manager _manager;

   public:
    using result_type = R;
    function() noexcept : _manager{} {}
    function(nullptr_t) noexcept : _manager{} {}
    function(const function& f) : _manager{f._manager} {}
    function(function&& f) noexcept : _manager{std::impl::move(f._manager)} {}

    /// @brief 関数ポインタ、関数オブジェクト、メンバポインタで初期化する
    /// @par テンプレートパラメータ制約
    /// - Fはfunction型でない
    /// - FはArgTypes...型の引数を用いて、関数呼び出し可能であり、
    ///   戻り値がR型である
    /// @tparam F
    /// 関数ポインタ、関数オブジェクト、メンバ関数ポインタ、メンバ変数ポインタ型
    /// @param f
    /// 関数ポインタ、関数オブジェクト、メンバ関数ポインタ、メンバ変数ポインタ
    template <typename F>
        requires(
            !is_same_v<remove_cvref_t<F>, function> &&
            requires(decay_t<F> f, ArgTypes... args) {
                {
                    std::impl::invoke(f, forward<ArgTypes>(args)...)
                } -> helper_function::convertible_to_return_type_implicitly<R>;
            })
    function(F&& f) : _manager{forward<F>(f)} {}
    function& operator=(const function& f) { function(f).swap(*this); }
    function& operator=(function&& f) {
        if (this == &f) return *this;

        this->_manager = std::impl::move(f._manager);
        return *this;
    }
    function& operator=(nullptr_t) noexcept { this->_manager.reset(); }
    template <typename F>
        requires(requires(decay_t<F> f, ArgTypes... args) {
            {
                std::impl::invoke(f, forward<ArgTypes>(args)...)
            } -> helper_function::convertible_to_return_type_implicitly<R>;
        })
    function& operator=(F&& f) {
        function(forward<F>(f)).swap(*this);
        return *this;
    }
    template <typename F>
    function& operator=(reference_wrapper<F> f) noexcept {
        function(f).swap(*this);
        return *this;
    }

    ~function() noexcept = default;

    void swap(function& other) noexcept {
        if (this == &other) return;
        callable_manager::swap(this->_manager, other._manager);
    }
    explicit operator bool() const noexcept {
        return this->_manager.has_callable();
    }
    R operator()(ArgTypes... args) const {
        return this->_manager.invoke(forward<ArgTypes>(args)...);
    }
#if defined(RTTI_ENABLED)
    const type_info& target_type() const noexcept {
        if (!this->_manager.has_callable()) return typeid(void);

        typename management_operator::get_type_info ope;
        this->_manager.operation(&ope);
        return *ope.info;
    }
#endif
    template <typename T>
    T* target() noexcept {
        if (!this->_manager.has_callable()) return nullptr;

        typename management_operator::template get_target<T> ope;
        this->_manager.operation(&ope);
        return ope.target;
    }
    template <typename T>
    const T* target() const noexcept {
        if (!this->_manager.has_callable()) return nullptr;

        typename management_operator::template get_target<T> ope;
        this->_manager.operation(&ope);
        return ope.target;
    }
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
