#pragma once

#include <bits/fundamental_traits.hpp>
#include <bits/constructible.hpp>

#include <bits/memory_order.hpp>

namespace std {
namespace impl {
    
template <typename T> struct atomic;
template <std::impl::integral T> struct atomic<T> {
    using value_type = T;
    static constexpr bool is_always_lock_free = true;
    constexpr atomic() noexcept(std::impl::is_nothrow_default_constructible_v<T>);
    constexpr atomic(value_type desired) noexcept;
    ~atomic() = default;
    value_type operator=(value_type desired) volatile noexcept;
    constexpr value_type operator=(value_type desired) noexcept;
    bool is_lock_free() const volatile noexcept;
    bool is_lock_free() const noexcept;
    void store(value_type desired, std::impl::memory_order order = memory_order_seq_cst) volatile noexcept;
    constexpr void store(value_type desired, std::impl::memory_order order = memory_order_seq_cst) noexcept;
    value_type load(memory_order order = memory_order_seq_cst) const volatile noexcept;
    constexpr value_type load(memory_order order = memory_order_seq_cst) const noexcept;
    operator value_type() const volatile noexcept
        requires(is_always_lock_free)
    {}
    constexpr operator value_type() const noexcept;
    value_type exchange(value_type desired, memory_order order = memory_order_seq_cst) volatile noexcept
        requires(is_always_lock_free)
    {}
    constexpr value_type exchange(value_type desired, memory_order order = memory_order_seq_cst) noexcept;
    bool compare_exchange_weak(
        value_type& expected, value_type desired,
        memory_order success,
        memory_order failure) volatile noexcept
        requires(is_always_lock_free)
    {}
    constexpr bool compare_exchange_weak(
        value_type& expected, value_type desired,
        memory_order success,
        memory_order failure) noexcept;
    bool compare_exchange_weak(
        value_type& expected, value_type desired,
        memory_order order = memory_order_seq_cst) volatile noexcept
        requires(is_always_lock_free)
    {}
    constexpr bool compare_exchange_weak(
        value_type& expected, value_type desired,
        memory_order order = memory_order_seq_cst) noexcept;    
    bool compare_exchange_strong(
        value_type& expected, value_type desired,
        memory_order success,
        memory_order failure) volatile noexcept
        requires(is_always_lock_free)
    {}
    constexpr bool compare_exchange_strong(
        value_type& expected, value_type desired,
        memory_order success,
        memory_order failure) noexcept;
    bool compare_exchange_strong(
        value_type& expected, value_type desired,
        memory_order order = memory_order_seq_cst) volatile noexcept
        requires(is_always_lock_free)
    {}
    constexpr bool compare_exchange_strong(
        value_type& expected, value_type desired,
        memory_order order = memory_order_seq_cst) noexcept;
    void wait(value_type old, memory_order order = memory_order_seq_cst) const volatile noexcept
        requires(is_always_lock_free)
    {}
    constexpr void wait(value_type old, memory_order order = memory_order_seq_cst) const noexcept;
    void notify_one() volatile noexcept
        requires(is_always_lock_free)
    {}
    constexpr void notify_one() noexcept;
    void notify_all() volatile noexcept
        requires(is_always_lock_free)
    {}
    constexpr void notify_all() noexcept;

    atomic(const atomic&) = delete;
    atomic(atomic&&) = delete;
    atomic& operator=(const atomic&) = delete;
    atomic& operator=(const atomic&) volatile = delete;
};

template <std::impl::floating_point T> struct atomic<T> {
    using value_type = T;
    static constexpr bool is_always_lock_free = true;
    constexpr atomic() noexcept(std::impl::is_nothrow_default_constructible_v<T>);
    constexpr atomic(value_type desired) noexcept;
    ~atomic() = default;
    value_type operator=(value_type desired) volatile noexcept;
    constexpr value_type operator=(value_type desired) noexcept;
    bool is_lock_free() const volatile noexcept;
    bool is_lock_free() const noexcept;
    void store(value_type desired, std::impl::memory_order order = memory_order_seq_cst) volatile noexcept;
    constexpr void store(value_type desired, std::impl::memory_order order = memory_order_seq_cst) noexcept;
    value_type load(memory_order order = memory_order_seq_cst) const volatile noexcept;
    constexpr value_type load(memory_order order = memory_order_seq_cst) const noexcept;
    operator value_type() const volatile noexcept
        requires(is_always_lock_free)
    {}
    constexpr operator value_type() const noexcept;
    value_type exchange(value_type desired, memory_order order = memory_order_seq_cst) volatile noexcept
        requires(is_always_lock_free)
    {}
    constexpr value_type exchange(value_type desired, memory_order order = memory_order_seq_cst) noexcept;
    bool compare_exchange_weak(
        value_type& expected, value_type desired,
        memory_order success,
        memory_order failure) volatile noexcept
        requires(is_always_lock_free)
    {}
    constexpr bool compare_exchange_weak(
        value_type& expected, value_type desired,
        memory_order success,
        memory_order failure) noexcept;
    bool compare_exchange_weak(
        value_type& expected, value_type desired,
        memory_order order = memory_order_seq_cst) volatile noexcept
        requires(is_always_lock_free)
    {}
    constexpr bool compare_exchange_weak(
        value_type& expected, value_type desired,
        memory_order order = memory_order_seq_cst) noexcept;    
    bool compare_exchange_strong(
        value_type& expected, value_type desired,
        memory_order success,
        memory_order failure) volatile noexcept
        requires(is_always_lock_free)
    {}
    constexpr bool compare_exchange_strong(
        value_type& expected, value_type desired,
        memory_order success,
        memory_order failure) noexcept;
    bool compare_exchange_strong(
        value_type& expected, value_type desired,
        memory_order order = memory_order_seq_cst) volatile noexcept
        requires(is_always_lock_free)
    {}
    constexpr bool compare_exchange_strong(
        value_type& expected, value_type desired,
        memory_order order = memory_order_seq_cst) noexcept;
    void wait(value_type old, memory_order order = memory_order_seq_cst) const volatile noexcept
        requires(is_always_lock_free)
    {}
    constexpr void wait(value_type old, memory_order order = memory_order_seq_cst) const noexcept;
    void notify_one() volatile noexcept
        requires(is_always_lock_free)
    {}
    constexpr void notify_one() noexcept;
    void notify_all() volatile noexcept
        requires(is_always_lock_free)
    {}
    constexpr void notify_all() noexcept;

    atomic(const atomic&) = delete;
    atomic(atomic&&) = delete;
    atomic& operator=(const atomic&) = delete;
    atomic& operator=(const atomic&) volatile = delete;
};

template <typename T> struct atomic<T*> {
    using value_type = T*;
    static constexpr bool is_always_lock_free = true;
    constexpr atomic() noexcept(std::impl::is_nothrow_default_constructible_v<T>);
    constexpr atomic(value_type desired) noexcept;
    ~atomic() = default;
    value_type operator=(value_type desired) volatile noexcept;
    constexpr value_type operator=(value_type desired) noexcept;
    bool is_lock_free() const volatile noexcept;
    bool is_lock_free() const noexcept;
    void store(value_type desired, std::impl::memory_order order = memory_order_seq_cst) volatile noexcept;
    constexpr void store(value_type desired, std::impl::memory_order order = memory_order_seq_cst) noexcept;
    value_type load(memory_order order = memory_order_seq_cst) const volatile noexcept;
    constexpr value_type load(memory_order order = memory_order_seq_cst) const noexcept;
    operator value_type() const volatile noexcept
        requires(is_always_lock_free)
    {}
    constexpr operator value_type() const noexcept;
    value_type exchange(value_type desired, memory_order order = memory_order_seq_cst) volatile noexcept
        requires(is_always_lock_free)
    {}
    constexpr value_type exchange(value_type desired, memory_order order = memory_order_seq_cst) noexcept;
    bool compare_exchange_weak(
        value_type& expected, value_type desired,
        memory_order success,
        memory_order failure) volatile noexcept
        requires(is_always_lock_free)
    {}
    constexpr bool compare_exchange_weak(
        value_type& expected, value_type desired,
        memory_order success,
        memory_order failure) noexcept;
    bool compare_exchange_weak(
        value_type& expected, value_type desired,
        memory_order order = memory_order_seq_cst) volatile noexcept
        requires(is_always_lock_free)
    {}
    constexpr bool compare_exchange_weak(
        value_type& expected, value_type desired,
        memory_order order = memory_order_seq_cst) noexcept;    
    bool compare_exchange_strong(
        value_type& expected, value_type desired,
        memory_order success,
        memory_order failure) volatile noexcept
        requires(is_always_lock_free)
    {}
    constexpr bool compare_exchange_strong(
        value_type& expected, value_type desired,
        memory_order success,
        memory_order failure) noexcept;
    bool compare_exchange_strong(
        value_type& expected, value_type desired,
        memory_order order = memory_order_seq_cst) volatile noexcept
        requires(is_always_lock_free)
    {}
    constexpr bool compare_exchange_strong(
        value_type& expected, value_type desired,
        memory_order order = memory_order_seq_cst) noexcept;
    void wait(value_type old, memory_order order = memory_order_seq_cst) const volatile noexcept
        requires(is_always_lock_free)
    {}
    constexpr void wait(value_type old, memory_order order = memory_order_seq_cst) const noexcept;
    void notify_one() volatile noexcept
        requires(is_always_lock_free)
    {}
    constexpr void notify_one() noexcept;
    void notify_all() volatile noexcept
        requires(is_always_lock_free)
    {}
    constexpr void notify_all() noexcept;


    atomic(const atomic&) = delete;
    atomic(atomic&&) = delete;
    atomic& operator=(const atomic&) = delete;
    atomic& operator=(const atomic&) volatile = delete;
};

}
}