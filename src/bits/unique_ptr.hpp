#pragma once

#include <bits/array_traits.hpp>
#include <bits/assignable.hpp>
#include <bits/constructible.hpp>
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

   public:
    using pointer = typename get_pointer_type<T, D>::type;
    using deleter_type = D;
    using element_type = T;

   private:
    pointer _ptr;
    deleter_type _deleter;

   public:
    constexpr unique_ptr() noexcept
        requires(is_nothrow_default_constructible_v<deleter_type> &&
                 !is_pointer_v<deleter_type>)
        : _ptr(nullptr), _deleter() {}

    constexpr explicit unique_ptr(pointer p) noexcept
        requires(is_nothrow_default_constructible_v<deleter_type> &&
                 !is_pointer_v<deleter_type>)
        : _ptr(p), _deleter() {}

    constexpr unique_ptr(pointer p, const D& d1) noexcept
        requires is_nothrow_copy_constructible_v<deleter_type>
        : _ptr(p), _deleter(d1) {}

    constexpr unique_ptr(pointer p, D&& d2) noexcept
        requires is_nothrow_move_constructible_v<deleter_type>
        : _ptr(p), _deleter(forward<D>(d2)) {}

    constexpr unique_ptr(unique_ptr&& u) noexcept
        requires is_nothrow_move_constructible_v<deleter_type>
        : _ptr(u.release()), _deleter(forward<D>(u.get_deleter())) {}

    constexpr unique_ptr(nullptr_t) noexcept
        requires(is_nothrow_default_constructible_v<deleter_type> &&
                 !is_pointer_v<deleter_type>)
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

/// @brief
/// リソースへのポインタの所有権を唯一持つスマートポインタ、配列版
/// @tparam T リソースの型
/// @tparam D リソースの解放方法
template <typename T, typename D>
class unique_ptr<T[], D> {
    template <typename Elem, typename Del>
    struct get_pointer_type {
        using type = Elem*;
    };

    template <typename Elem, typename Del>
        requires requires() { typename Del::pointer; }
    struct get_pointer_type<Elem, Del> {
        using type = typename Del::pointer;
    };

   public:
    using pointer = typename get_pointer_type<T, D>::type;
    using deleter_type = D;
    using element_type = T;

    constexpr unique_ptr() noexcept
        requires(is_nothrow_default_constructible_v<deleter_type> &&
                 !is_pointer_v<deleter_type>)
        : _ptr(nullptr), _deleter() {}
    {}

    template <typename U>
        requires(is_nothrow_default_constructible_v<deleter_type> &&
                 !is_pointer_v<deleter_type> &&
                 (is_same_v<U, pointer> ||
                  (is_same_v<pointer, element_type*> && is_pointer_v<U> &&
                   is_convertible_v<remove_pointer_t<U> (*)[],
                                    element_type (*)[]>)))
    constexpr explicit unique_ptr(U p) noexcept : _ptr(p), _deleter() {}

    template <typename U>
        requires(is_nothrow_copy_constructible_v<deleter_type> &&
                 (is_same_v<U, pointer> ||
                  (is_same_v<pointer, element_type*> && is_pointer_v<U> &&
                   is_convertible_v<remove_pointer_t<U> (*)[],
                                    element_type (*)[]>) ||
                  is_same_v<U, nullptr_t>))
    constexpr unique_ptr(U p, const D& d1) noexcept : _ptr(p), _deleter(d1) {}

    template <typename U>
        requires(is_nothrow_move_constructible_v<deleter_type> &&
                 (is_same_v<U, pointer> ||
                  (is_same_v<pointer, element_type*> && is_pointer_v<U> &&
                   is_convertible_v<remove_pointer_t<U> (*)[],
                                    element_type (*)[]>) ||
                  is_same_v<U, nullptr_t>))
    constexpr unique_ptr(U p, D&& d2) noexcept
        : _ptr(p), _deleter(forward<D>(d2)) {}

    constexpr unique_ptr(unique_ptr&& u) noexcept
        requires is_nothrow_move_constructible_v<deleter_type>
        : _ptr(u.release()), _deleter(forward<D>(u.get_deleter())) {}

    constexpr unique_ptr(nullptr_t) noexcept
        requires is_nothrow_default_constructible_v<deleter_type>
        : _ptr(nullptr), _deleter() {}

    template <typename U, typename E>
    constexpr unique_ptr(unique_ptr<U, E>&& u) noexcept
        requires(!is_array_v<U> && is_same_v<pointer, element_type*> &&
                 is_same_v<typename unique_ptr<U, E>::pointer,
                           typename unique_ptr<U, E>::element_type*> &&
                 is_convertible_v<typename unique_ptr<U, D>::element_type (*)[],
                                  element_type (*)[]> &&
                 ((is_same_v<D, E> && is_reference_v<D>) ||
                  (!is_reference_v<D> && is_convertible_v<E, D>)))
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
        requires(is_array_v<U> && is_same_v<pointer, element_type*> &&
                 is_same_v<typename unique_ptr<U, E>::pointer,
                           typename unique_ptr<U, E>::element_type*> &&
                 is_convertible_v<typename unique_ptr<U, D>::element_type (*)[],
                                  element_type (*)[]> &&
                 is_assignable_v<D&, E &&>)
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

    constexpr void reset(nullptr_t = nullptr) noexcept { reset(); }

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

    constexpr T& operator[](size_t i) const { return get()[i]; }
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
