#pragma once

#include <bits/namespace.hpp>
#include <bits/atomic_memory_order.hpp>

namespace OZLIB_NAMESPACE {
namespace impl {

struct atomic_flag {
    unsigned char _M_val = 0;

    atomic_flag() noexcept = default;
    constexpr atomic_flag(bool val) noexcept : _M_val(val ? 1 : 0) {}

    atomic_flag(const atomic_flag&) = delete;
    atomic_flag& operator=(const atomic_flag&) = delete;
    atomic_flag& operator=(const atomic_flag&) volatile = delete;

    bool test_and_set(memory_order order = memory_order_seq_cst) volatile noexcept {
        return __atomic_test_and_set(&_M_val, static_cast<int>(order));
    }

    bool test_and_set(memory_order order = memory_order_seq_cst) noexcept {
        return __atomic_test_and_set(&_M_val, static_cast<int>(order));
    }

    void clear(memory_order order = memory_order_seq_cst) volatile noexcept {
        __atomic_clear(&_M_val, static_cast<int>(order));
    }

    void clear(memory_order order = memory_order_seq_cst) noexcept {
        __atomic_clear(&_M_val, static_cast<int>(order));
    }

    bool test(memory_order order = memory_order_seq_cst) const volatile noexcept {
        unsigned char ret;
        __atomic_load(&_M_val, &ret, static_cast<int>(order));
        return ret != 0;
    }

    bool test(memory_order order = memory_order_seq_cst) const noexcept {
        unsigned char ret;
        __atomic_load(&_M_val, &ret, static_cast<int>(order));
        return ret != 0;
    }
};

} // namespace impl
} // namespace OZLIB_NAMESPACE
