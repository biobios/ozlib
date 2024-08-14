#pragma once

#include <bits/array_traits.hpp>
#include <bits/assignable.hpp>
#include <bits/default_delete.hpp>
#include <bits/forward.hpp>
#include <bits/move.hpp>
#include <bits/nullptr_t.hpp>
#include <bits/pointer_traits.hpp>
#include <bits/reference_traits.hpp>
#include <bits/same.hpp>
#include <bits/swap.hpp>

// todo: 共通のオーバーロード解決に参加しない条件を追加する

namespace std {
namespace impl {

/// @brief
/// リソースへのポインタの所有権を唯一持つスマートポインタ、単一オブジェクト版
/// @tparam T リソースの型
/// @tparam D リソースの解放方法
template <typename T, typename D = default_delete<T>>
class unique_ptr {
    template <typename Elem, typename Del>
    struct get_pointer_type {
        using type = Elem*;
    };

    template <typename Elem, typename Del>
        requires requires() { typename Del::pointer; }
    struct get_pointer_type<Elem, Del> {
        using type = typename Del::pointer;
    };

    static constexpr bool deleter_default_constructible = requires {
        { D() } noexcept;
    };
    static constexpr bool deleter_copy_constructible = requires(const D& d) {
        { D(d) } noexcept;
    };

    // Dが左辺値参照ならば、deleter_move_constructibleはdeleter_copy_constructibleと同じ
    static constexpr bool deleter_move_constructible = requires(D&& d) {
        { D(forward<D>(d)) } noexcept;
    };

   public:
    using pointer = typename get_pointer_type<T, D>::type;
    using deleter_type = D;
    using element_type = T;

   private:
    pointer _ptr;
    deleter_type _deleter;

   public:
    constexpr unique_ptr() noexcept
        requires(deleter_default_constructible && !is_pointer_v<deleter_type>)
        : _ptr(nullptr), _deleter() {}

    constexpr explicit unique_ptr(pointer p) noexcept
        requires(deleter_default_constructible && !is_pointer_v<deleter_type>)
        : _ptr(p), _deleter() {}

    constexpr unique_ptr(pointer p, const D& d1) noexcept
        requires deleter_copy_constructible
        : _ptr(p), _deleter(d1) {}

    constexpr unique_ptr(pointer p, D&& d2) noexcept
        requires deleter_move_constructible
        : _ptr(p), _deleter(forward<D>(d2)) {}

    constexpr unique_ptr(unique_ptr&& u) noexcept
        requires deleter_move_constructible
        : _ptr(u.release()), _deleter(forward<D>(u.get_deleter())) {}

    constexpr unique_ptr(nullptr_t) noexcept
        requires(deleter_default_constructible && !is_pointer_v<deleter_type>)
        : _ptr(nullptr), _deleter() {}

    template <typename U, typename E>
        requires(is_convertible_v<typename unique_ptr<U, E>::pointer,
                                  pointer> &&
                 !is_array_v<U> &&
                 (is_same_v<D, E> ||
                  (!is_reference_v<D> && is_convertible_v<E, D>)))
    constexpr unique_ptr(unique_ptr<U, E>&& u) noexcept
        : _ptr(u.release()), _deleter(forward<E>(u.get_deleter())) {}

    unique_ptr(const unique_ptr&) = delete;

    constexpr ~unique_ptr() {
        if (get() != nullptr) {
            get_deleter()(get());
        }
    }

    constexpr unique_ptr& operator=(unique_ptr&& u) noexcept {
        reset(u.release());
        _deleter = forward<D>(u.get_deleter());
    }

    template <typename U, typename E>
        requires(
            is_convertible_v<typename unique_ptr<U, E>::pointer, pointer> &&
            !is_array_v<U> && is_assignable_v<D&, E &&>)
    constexpr unique_ptr& operator=(unique_ptr<U, E>&& u) noexcept {
        reset(u.release());
        _deleter = forward<E>(u.get_deleter());
    }

    constexpr unique_ptr& operator=(nullptr_t) noexcept { reset(); }

    unique_ptr& operator=(const unique_ptr&) = delete;

    constexpr pointer release() noexcept {
        pointer p = get();
        _ptr = nullptr;
        return p;
    }

    constexpr void reset(pointer p = pointer()) noexcept {
        pointer old = get();
        _ptr = p;
        if (old != nullptr) {
            get_deleter()(old);
        }
    }

    template <typename U>
        requires(
            is_same_v<U, pointer> ||
            (is_same_v<pointer, element_type*> && is_pointer_v<U> &&
             is_convertible_v<remove_pointer_t<U> (*)[], element_type (*)[]>))
    constexpr void reset(U p) noexcept {
        reset(static_cast<pointer>(p));
    }

    constexpr void swap(unique_ptr& u) noexcept {
        swap(_ptr, u._ptr);
        swap(_deleter, u._deleter);
    }

    constexpr pointer get() const noexcept { return _ptr; }

    constexpr const deleter_type& get_deleter() const noexcept {
        return _deleter;
    }

    constexpr explicit operator bool() const noexcept {
        return get() != nullptr;
    }

    constexpr add_lvalue_reference_t<T> operator*() const { return *get(); }

    constexpr pointer operator->() const noexcept { return get(); }
};


template <typename T1, typename D1, typename T2, typename D2>
constexpr bool operator==(const unique_ptr<T1, D1>& x,
                          const unique_ptr<T2, D2>& y) noexcept {
    return x.get() == y.get();
}

template <typename T, typename D>
constexpr bool operator==(const unique_ptr<T, D>& x, nullptr_t) noexcept {
    return !x;
}

template <typename T, typename D>
constexpr bool operator==(nullptr_t, const unique_ptr<T, D>& x) noexcept {
    return !x;
}

template <typename T1, typename D1, typename T2, typename D2>
constexpr bool operator!=(const unique_ptr<T1, D1>& x,
                          const unique_ptr<T2, D2>& y) noexcept {
    return x.get() != y.get();
}

template <typename T, typename D>
constexpr bool operator!=(const unique_ptr<T, D>& x, nullptr_t) noexcept {
    return static_cast<bool>(x);
}

template <typename T, typename D>
constexpr bool operator!=(nullptr_t, const unique_ptr<T, D>& x) noexcept {
    return static_cast<bool>(x);
}

// todo: 三方演算子の実装

template <typename T, typename... Args>
    requires(!is_array_v<T>)
constexpr unique_ptr<T> make_unique(Args&&... args) {
    return unique_ptr<T>(new T(forward<Args>(args)...));
}

template <typename T>
    requires(is_unbounded_array_v<T>)
constexpr unique_ptr<T> make_unique(size_t n) {
    return unique_ptr<T>(new remove_extent_t<T>[n]());
}

template <typename T, typename... Args>
    requires(is_bounded_array_v<T>)
void make_unique(Args&&...) = delete;

}  // namespace impl
}  // namespace std
