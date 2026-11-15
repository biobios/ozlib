#pragma once

#include <bits/namespace.hpp>

#include <new> // IWYU pragma: export

#include <bits/assignable.hpp>
#include <bits/byte.hpp>
#include <bits/compound_traits.hpp>
#include <bits/constructible.hpp>
#include <bits/cv_traits.hpp>
#include <bits/exception.hpp>
#include <bits/exception_helper.hpp>
#include <bits/in_place_t.hpp>
#include <bits/invoke.hpp>
#include <bits/invoke_traits.hpp>
#include <bits/move.hpp>
#include <bits/same.hpp>
#include <bits/swap.hpp>
#include <bits/swappable.hpp>
#include <bits/template_specialization_helper.hpp>
#include <bits/temporary_traits.hpp>

namespace OZLIB_NAMESPACE {
namespace impl {
class bad_optional_access : public exception {
public:
    constexpr bad_optional_access() noexcept = default;
    constexpr bad_optional_access(const bad_optional_access&) noexcept = default;
    constexpr bad_optional_access& operator=(const bad_optional_access&) noexcept = default;
    const char* what() const noexcept override {
        return "bad optional access";
    }
};
struct nullopt_t {
public:
    struct _init_tag {};
    constexpr explicit nullopt_t(_init_tag) noexcept {}
    friend constexpr auto operator<=>(nullopt_t, nullopt_t) noexcept = default;
};
inline constexpr nullopt_t nullopt{nullopt_t::_init_tag{}};
template <typename T>
    requires(!OZLIB_NAMESPACE::impl::is_reference_v<T> &&
        !OZLIB_NAMESPACE::impl::same_as<OZLIB_NAMESPACE::impl::remove_cv_t<T>, OZLIB_NAMESPACE::impl::nullopt_t>)
class optional {
public:
    using value_type = T;
    alignas(T) byte _storage[sizeof(T)];
    bool _has_value;
    constexpr optional() noexcept : _has_value(false) {}
    constexpr optional(nullopt_t) noexcept : _has_value(false) {}
    constexpr optional(const optional& rhs) : _has_value(rhs._has_value) {
        if (_has_value) {
            new (_storage) T(*rhs);
        }
    }
    constexpr optional(optional&& rhs) noexcept(is_nothrow_move_constructible_v<T>)
        : _has_value(rhs._has_value) {
        if (_has_value) {
            new (_storage) T(move(*rhs));
        }
    }
    template <typename... Args>
        requires constructible_from<T, Args...>
    constexpr explicit optional(in_place_t, Args&&... args) : _has_value(true) {
        new (_storage) T(forward<Args>(args)...);
    }
    template <typename U = T>
        requires constructible_from<T, U>
    explicit(!is_convertible_v<U&&, T>) constexpr optional(U&& value) 
        : _has_value(true) {
        new (_storage) T(forward<U>(value));
    }
    template <typename U>
        requires constructible_from<T, const U&>
    explicit(!is_convertible_v<U&&, T>) constexpr optional(
        const optional<U>& rhs) : _has_value(rhs._has_value) {
        if (_has_value) {
            new (_storage) T(*rhs);
        }
    }
    template <typename U>
        requires constructible_from<T, U&&>
    explicit(!is_convertible_v<U&&, T>) constexpr optional(
        optional<U>&& rhs) : _has_value(rhs._has_value) {
        if (_has_value) {
            new (_storage) T(move(*rhs));
        }
    }
    constexpr ~optional() {
        if (_has_value) {
            reinterpret_cast<T*>(_storage)->~T();
        }
    }
    constexpr optional& operator=(nullopt_t) noexcept {
        reset();
        return *this;
    }
    constexpr optional& operator=(const optional& rhs) {
        if (_has_value && rhs._has_value) {
            **this = *rhs;
        } else if (_has_value) {
            reinterpret_cast<T*>(_storage)->~T();
            _has_value = false;
        } else if (rhs._has_value) {
            new (_storage) T(*rhs);
            _has_value = true;
        }
        return *this;
    }
    constexpr optional& operator=(optional&& rhs) noexcept(is_nothrow_move_assignable_v<T> &&
        is_nothrow_move_constructible_v<T>) {
        if (_has_value && rhs._has_value) {
            **this = move(*rhs);
        } else if (_has_value) {
            reinterpret_cast<T*>(_storage)->~T();
            _has_value = false;
        } else if (rhs._has_value) {
            new (_storage) T(move(*rhs));
            _has_value = true;
        }
        return *this;
    }
    template <typename U = T>
        requires convertible_to<U, T>
    constexpr optional& operator=(U&& value) {
        if (_has_value) {
            **this = forward<U>(value);
        } else {
            new (_storage) T(forward<U>(value));
            _has_value = true;
        }
        return *this;
    }
    template <typename U>
        requires convertible_to<U, T>
    constexpr optional& operator=(const optional<U>& rhs) {
        if (_has_value && rhs._has_value) {
            **this = *rhs;
        } else if (_has_value) {
            reinterpret_cast<T*>(_storage)->~T();
            _has_value = false;
        } else if (rhs._has_value) {
            new (_storage) T(*rhs);
            _has_value = true;
        }
        return *this;
    }
    template <typename U>
        requires convertible_to<U, T>
    constexpr optional& operator=(optional<U>&& rhs) {
        if (_has_value && rhs._has_value) {
            **this = move(*rhs);
        } else if (_has_value) {
            reinterpret_cast<T*>(_storage)->~T();
            _has_value = false;
        } else if (rhs._has_value) {
            new (_storage) T(move(*rhs));
            _has_value = true;
        }
        return *this;
    }
    template <typename... Args>
        requires is_constructible_v<T, Args&&...>
    constexpr T& emplace(Args&&... args) {
        if (_has_value) {
            reinterpret_cast<T*>(_storage)->~T();
        }
        new (_storage) T(forward<Args>(args)...);
        _has_value = true;
        return **this;
    }
    constexpr void swap(optional& rhs) noexcept(is_nothrow_move_constructible_v<T> &&
        is_nothrow_swappable_v<T>) {
        if (_has_value && rhs._has_value) {
            using OZLIB_NAMESPACE::swap;
            swap(**this, *rhs);
        } else if (_has_value) {
            new (rhs._storage) T(move(**this));
            reinterpret_cast<T*>(_storage)->~T();
            _has_value = false;
            rhs._has_value = true;
        } else if (rhs._has_value) {
            new (_storage) T(move(*rhs));
            reinterpret_cast<T*>(rhs._storage)->~T();
            _has_value = true;
            rhs._has_value = false;
        }
    }
    constexpr void reset() noexcept {
        if (_has_value) {
            reinterpret_cast<T*>(_storage)->~T();
            _has_value = false;
        }
    }
    constexpr T& operator*() & {
        return *reinterpret_cast<T*>(_storage);
    }
    constexpr T&& operator*() && {
        return move(*reinterpret_cast<T*>(_storage));
    }
    constexpr const T& operator*() const& {
        return *reinterpret_cast<const T*>(_storage);
    }
    constexpr const T&& operator*() const&& {
        return move(*reinterpret_cast<const T*>(_storage));
    }
    constexpr const T* operator->() const {
        return reinterpret_cast<const T*>(_storage);
    }
    constexpr T* operator->() {
        return reinterpret_cast<T*>(_storage);
    }
    constexpr explicit operator bool() const noexcept {
        return _has_value;
    }
    constexpr bool has_value() const noexcept {
        return _has_value;
    }
    constexpr const T& value() const& {
        if (!_has_value) {
            ozlib_exception_helper::throw_exception_of_type<bad_optional_access>();
        }
        return **this;
    }
    constexpr T& value() & {
        if (!_has_value) {
            ozlib_exception_helper::throw_exception_of_type<bad_optional_access>();
        }
        return **this;
    }
    constexpr T&& value() && {
        if (!_has_value) {
            ozlib_exception_helper::throw_exception_of_type<bad_optional_access>();
        }
        return move(**this);
    }
    constexpr const T&& value() const&& {
        if (!_has_value) {
            ozlib_exception_helper::throw_exception_of_type<bad_optional_access>();
        }
        return move(**this);
    }
    template <typename U>
        requires is_copy_constructible_v<T>
    constexpr T value_or(U&& v) const& {
        if (_has_value) {
            return **this;
        } else {
            return static_cast<T>(forward<U>(v));
        }
    }
    template <typename U>
        requires is_move_constructible_v<T>
    constexpr T value_or(U&& v) && {
        if (_has_value) {
            return move(**this);
        } else {
            return static_cast<T>(forward<U>(v));
        }
    }
    template <typename F>
    constexpr auto and_then(F&& f) & {
        using U = invoke_result_t<F, decltype(value())>;
        static_assert(detail::is_specialization_of_v<U, optional>, "F must return an optional type");
        if (*this) {
            return invoke(forward<F>(f), value());
        } else {
            return remove_cvref_t<U>();
        }
    }
    template <typename F>
    constexpr auto and_then(F&& f) && {
        using U = invoke_result_t<F, decltype(move(value()))>;
        static_assert(detail::is_specialization_of_v<U, optional>, "F must return an optional type");
        if (*this) {
            return invoke(forward<F>(f), move(value()));
        } else {
            return remove_cvref_t<U>();
        }
    }
    template <typename F>
    constexpr auto and_then(F&& f) const& {
        using U = invoke_result_t<F, decltype(value())>;
        static_assert(detail::is_specialization_of_v<U, optional>, "F must return an optional type");
        if (*this) {
            return invoke(forward<F>(f), value());
        } else {
            return remove_cvref_t<U>();
        }
    }
    template <typename F>
    constexpr auto and_then(F&& f) const&& {
        using U = invoke_result_t<F, decltype(move(value()))>;
        static_assert(detail::is_specialization_of_v<U, optional>, "F must return an optional type");
        if (*this) {
            return invoke(forward<F>(f), move(value()));
        } else {
            return remove_cvref_t<U>();
        }
    }
    template <typename F>
    constexpr auto transform(F&& f) & {
        using U = invoke_result_t<F, decltype(value())>;
        static_assert(!is_same_v<U, in_place_t> && !is_same_v<U, in_place_type_t<T>> && !is_same_v<U, nullopt_t>, "F must not return in_place_t or nullopt_t");
        if (*this) {
            return optional<U>(invoke(forward<F>(f), value()));
        } else {
            return optional<U>();
        }
    }
    template <typename F>
    constexpr auto transform(F&& f) && {
        using U = invoke_result_t<F, decltype(move(value()))>;
        static_assert(!is_same_v<U, in_place_t> && !is_same_v<U, in_place_type_t<T>> && !is_same_v<U, nullopt_t>, "F must not return in_place_t or nullopt_t");
        if (*this) {
            return optional<U>(invoke(forward<F>(f), move(value())));
        } else {
            return optional<U>();
        }
    }
    template <typename F>
    constexpr auto transform(F&& f) const& {
        using U = invoke_result_t<F, decltype(value())>;
        static_assert(!is_same_v<U, in_place_t> && !is_same_v<U, in_place_type_t<T>> && !is_same_v<U, nullopt_t>, "F must not return in_place_t or nullopt_t");
        if (*this) {
            return optional<U>(invoke(forward<F>(f), value()));
        } else {
            return optional<U>();
        }
    }
    template <typename F>
    constexpr auto transform(F&& f) const&& {
        using U = invoke_result_t<F, decltype(move(value()))>;
        static_assert(!is_same_v<U, in_place_t> && !is_same_v<U, in_place_type_t<T>> && !is_same_v<U, nullopt_t>, "F must not return in_place_t or nullopt_t");
        if (*this) {
            return optional<U>(invoke(forward<F>(f), move(value())));
        } else {
            return optional<U>();
        }
    }
    template <typename F>
        requires invocable<F> && copy_constructible<T>
    constexpr optional or_else(F&& f) const& {
        if (*this) {
            return *this;
        } else {
            return forward<F>(f)();
        }
    }
    template <typename F>
        requires invocable<F> && move_constructible<T>
    constexpr optional or_else(F&& f) && {
        if (*this) {
            return move(*this);
        } else {
            return forward<F>(f)();
        }
    }
};

template <typename T>
constexpr optional<decay_t<T>> make_optional(T&& v) {
    return optional<decay_t<T>>(forward<T>(v));
}

template <typename T, typename... Args>
constexpr optional<T> make_optional(Args&&... args) {
    return optional<T>(in_place, forward<Args>(args)...);
}

template <typename T, typename U>
constexpr bool operator==(const optional<T>& x, const optional<U>& y) {
    if (x.has_value() != y.has_value()) {
        return false;
    }
    if (!x.has_value()) {
        return true;
    }
    return *x == *y;
}
template <typename T>
constexpr bool operator==(const optional<T>& x, nullopt_t) noexcept {
    return !x.has_value();
}
template <typename T, typename U>
constexpr bool operator==(const optional<T>& x, const U& y) {
    if (!x.has_value()) {
        return false;
    }
    return *x == y;
}
template <typename T, typename U>
constexpr bool operator==(const T& x, const optional<U>& y) {
    if (!y.has_value()) {
        return false;
    }
    return x == *y;
}
template <typename T, typename U>
constexpr bool operator!=(const optional<T>& x, const optional<U>& y) {
    return !(x == y);
}
template <typename T, typename U>
constexpr bool operator!=(const optional<T>& x, const U& y) {
    return !(x == y);
}
template <typename T, typename U>
constexpr bool operator!=(const U& x, const optional<T>& y) {
    return !(x == y);
}

template <typename T>
optional(T) -> optional<T>;

}
template <typename T>
    requires(impl::is_move_constructible_v<T> && impl::is_swappable_v<T>)
constexpr void swap(impl::optional<T>& x, impl::optional<T>& y) noexcept(noexcept(x.swap(y))) {
    x.swap(y);
}
}