#pragma once

#include <bits/namespace.hpp>
#include <bits/atomic_memory_order.hpp>

namespace OZLIB_NAMESPACE {
namespace impl {

template <typename T>
struct __atomic_base {
public:
    T _M_val;


public:
    __atomic_base() noexcept = default;
    constexpr __atomic_base(T val) noexcept : _M_val(val) {}
    __atomic_base(const __atomic_base&) = delete;
    __atomic_base& operator=(const __atomic_base&) = delete;
    __atomic_base& operator=(const __atomic_base&) volatile = delete;

    bool is_lock_free() const volatile noexcept {
        return __atomic_is_lock_free(sizeof(T), &_M_val);
    }
    bool is_lock_free() const noexcept {
        return __atomic_is_lock_free(sizeof(T), &_M_val);
    }
    static constexpr bool is_always_lock_free = __atomic_always_lock_free(sizeof(T), nullptr);

    void store(T desired, memory_order order = memory_order_seq_cst) volatile noexcept {
        __atomic_store(&_M_val, &desired, static_cast<int>(order));
    }
    void store(T desired, memory_order order = memory_order_seq_cst) noexcept {
        __atomic_store(&_M_val, &desired, static_cast<int>(order));
    }

    T load(memory_order order = memory_order_seq_cst) const volatile noexcept {
        T ret;
        __atomic_load(&_M_val, &ret, static_cast<int>(order));
        return ret;
    }
    T load(memory_order order = memory_order_seq_cst) const noexcept {
        T ret;
        __atomic_load(&_M_val, &ret, static_cast<int>(order));
        return ret;
    }

    T exchange(T desired, memory_order order = memory_order_seq_cst) volatile noexcept {
        T ret;
        __atomic_exchange(&_M_val, &desired, &ret, static_cast<int>(order));
        return ret;
    }
    T exchange(T desired, memory_order order = memory_order_seq_cst) noexcept {
        T ret;
        __atomic_exchange(&_M_val, &desired, &ret, static_cast<int>(order));
        return ret;
    }

    bool compare_exchange_weak(T& expected, T desired, memory_order success, memory_order failure) volatile noexcept {
        return __atomic_compare_exchange(&_M_val, &expected, &desired, true, static_cast<int>(success), static_cast<int>(failure));
    }
    bool compare_exchange_weak(T& expected, T desired, memory_order success, memory_order failure) noexcept {
        return __atomic_compare_exchange(&_M_val, &expected, &desired, true, static_cast<int>(success), static_cast<int>(failure));
    }
    bool compare_exchange_weak(T& expected, T desired, memory_order order = memory_order_seq_cst) volatile noexcept {
        memory_order failure = order == memory_order_acq_rel ? memory_order_acquire :
                               order == memory_order_release ? memory_order_relaxed : order;
        return compare_exchange_weak(expected, desired, order, failure);
    }
    bool compare_exchange_weak(T& expected, T desired, memory_order order = memory_order_seq_cst) noexcept {
        memory_order failure = order == memory_order_acq_rel ? memory_order_acquire :
                               order == memory_order_release ? memory_order_relaxed : order;
        return compare_exchange_weak(expected, desired, order, failure);
    }

    bool compare_exchange_strong(T& expected, T desired, memory_order success, memory_order failure) volatile noexcept {
        return __atomic_compare_exchange(&_M_val, &expected, &desired, false, static_cast<int>(success), static_cast<int>(failure));
    }
    bool compare_exchange_strong(T& expected, T desired, memory_order success, memory_order failure) noexcept {
        return __atomic_compare_exchange(&_M_val, &expected, &desired, false, static_cast<int>(success), static_cast<int>(failure));
    }
    bool compare_exchange_strong(T& expected, T desired, memory_order order = memory_order_seq_cst) volatile noexcept {
        memory_order failure = order == memory_order_acq_rel ? memory_order_acquire :
                               order == memory_order_release ? memory_order_relaxed : order;
        return compare_exchange_strong(expected, desired, order, failure);
    }
    bool compare_exchange_strong(T& expected, T desired, memory_order order = memory_order_seq_cst) noexcept {
        memory_order failure = order == memory_order_acq_rel ? memory_order_acquire :
                               order == memory_order_release ? memory_order_relaxed : order;
        return compare_exchange_strong(expected, desired, order, failure);
    }
};

} // namespace impl
} // namespace OZLIB_NAMESPACE
