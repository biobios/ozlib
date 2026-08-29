#pragma once

#include <bits/namespace.hpp>
#include <bits/atomic_base.hpp>
#include <bits/ptrdiff_t.hpp>

namespace OZLIB_NAMESPACE {
namespace impl {

template <typename T>
struct atomic : public __atomic_base<T> {
    atomic() noexcept = default;
    constexpr atomic(T val) noexcept : __atomic_base<T>(val) {}

    T operator=(T desired) volatile noexcept {
        this->store(desired);
        return desired;
    }
    T operator=(T desired) noexcept {
        this->store(desired);
        return desired;
    }

    operator T() const volatile noexcept {
        return this->load();
    }
    operator T() const noexcept {
        return this->load();
    }

    atomic(const atomic&) = delete;
    atomic& operator=(const atomic&) = delete;
    atomic& operator=(const atomic&) volatile = delete;
};

// Macro for integral specializations
#define _OZLIB_ATOMIC_INTEGRAL_SPECIALIZATION(IType) \
template <> \
struct atomic<IType> : public __atomic_base<IType> { \
    using __base_type = __atomic_base<IType>; \
    atomic() noexcept = default; \
    constexpr atomic(IType val) noexcept : __base_type(val) {} \
    atomic(const atomic&) = delete; \
    atomic& operator=(const atomic&) = delete; \
    atomic& operator=(const atomic&) volatile = delete; \
    IType operator=(IType desired) volatile noexcept { this->store(desired); return desired; } \
    IType operator=(IType desired) noexcept { this->store(desired); return desired; } \
    operator IType() const volatile noexcept { return this->load(); } \
    operator IType() const noexcept { return this->load(); } \
    IType fetch_add(IType arg, memory_order order = memory_order_seq_cst) volatile noexcept { \
        return __atomic_fetch_add(&this->_M_val, arg, static_cast<int>(order)); \
    } \
    IType fetch_add(IType arg, memory_order order = memory_order_seq_cst) noexcept { \
        return __atomic_fetch_add(&this->_M_val, arg, static_cast<int>(order)); \
    } \
    IType fetch_sub(IType arg, memory_order order = memory_order_seq_cst) volatile noexcept { \
        return __atomic_fetch_sub(&this->_M_val, arg, static_cast<int>(order)); \
    } \
    IType fetch_sub(IType arg, memory_order order = memory_order_seq_cst) noexcept { \
        return __atomic_fetch_sub(&this->_M_val, arg, static_cast<int>(order)); \
    } \
    IType fetch_and(IType arg, memory_order order = memory_order_seq_cst) volatile noexcept { \
        return __atomic_fetch_and(&this->_M_val, arg, static_cast<int>(order)); \
    } \
    IType fetch_and(IType arg, memory_order order = memory_order_seq_cst) noexcept { \
        return __atomic_fetch_and(&this->_M_val, arg, static_cast<int>(order)); \
    } \
    IType fetch_or(IType arg, memory_order order = memory_order_seq_cst) volatile noexcept { \
        return __atomic_fetch_or(&this->_M_val, arg, static_cast<int>(order)); \
    } \
    IType fetch_or(IType arg, memory_order order = memory_order_seq_cst) noexcept { \
        return __atomic_fetch_or(&this->_M_val, arg, static_cast<int>(order)); \
    } \
    IType fetch_xor(IType arg, memory_order order = memory_order_seq_cst) volatile noexcept { \
        return __atomic_fetch_xor(&this->_M_val, arg, static_cast<int>(order)); \
    } \
    IType fetch_xor(IType arg, memory_order order = memory_order_seq_cst) noexcept { \
        return __atomic_fetch_xor(&this->_M_val, arg, static_cast<int>(order)); \
    } \
    IType operator++(int) volatile noexcept { return fetch_add(1); } \
    IType operator++(int) noexcept { return fetch_add(1); } \
    IType operator--(int) volatile noexcept { return fetch_sub(1); } \
    IType operator--(int) noexcept { return fetch_sub(1); } \
    IType operator++() volatile noexcept { return fetch_add(1) + 1; } \
    IType operator++() noexcept { return fetch_add(1) + 1; } \
    IType operator--() volatile noexcept { return fetch_sub(1) - 1; } \
    IType operator--() noexcept { return fetch_sub(1) - 1; } \
    IType operator+=(IType arg) volatile noexcept { return fetch_add(arg) + arg; } \
    IType operator+=(IType arg) noexcept { return fetch_add(arg) + arg; } \
    IType operator-=(IType arg) volatile noexcept { return fetch_sub(arg) - arg; } \
    IType operator-=(IType arg) noexcept { return fetch_sub(arg) - arg; } \
    IType operator&=(IType arg) volatile noexcept { return fetch_and(arg) & arg; } \
    IType operator&=(IType arg) noexcept { return fetch_and(arg) & arg; } \
    IType operator|=(IType arg) volatile noexcept { return fetch_or(arg) | arg; } \
    IType operator|=(IType arg) noexcept { return fetch_or(arg) | arg; } \
    IType operator^=(IType arg) volatile noexcept { return fetch_xor(arg) ^ arg; } \
    IType operator^=(IType arg) noexcept { return fetch_xor(arg) ^ arg; } \
};

_OZLIB_ATOMIC_INTEGRAL_SPECIALIZATION(char)
_OZLIB_ATOMIC_INTEGRAL_SPECIALIZATION(signed char)
_OZLIB_ATOMIC_INTEGRAL_SPECIALIZATION(unsigned char)
_OZLIB_ATOMIC_INTEGRAL_SPECIALIZATION(short)
_OZLIB_ATOMIC_INTEGRAL_SPECIALIZATION(unsigned short)
_OZLIB_ATOMIC_INTEGRAL_SPECIALIZATION(int)
_OZLIB_ATOMIC_INTEGRAL_SPECIALIZATION(unsigned int)
_OZLIB_ATOMIC_INTEGRAL_SPECIALIZATION(long)
_OZLIB_ATOMIC_INTEGRAL_SPECIALIZATION(unsigned long)
_OZLIB_ATOMIC_INTEGRAL_SPECIALIZATION(long long)
_OZLIB_ATOMIC_INTEGRAL_SPECIALIZATION(unsigned long long)
_OZLIB_ATOMIC_INTEGRAL_SPECIALIZATION(char8_t)
_OZLIB_ATOMIC_INTEGRAL_SPECIALIZATION(char16_t)
_OZLIB_ATOMIC_INTEGRAL_SPECIALIZATION(char32_t)
_OZLIB_ATOMIC_INTEGRAL_SPECIALIZATION(wchar_t)

#undef _OZLIB_ATOMIC_INTEGRAL_SPECIALIZATION

template <typename T>
struct atomic<T*> : public __atomic_base<T*> {
    using __base_type = __atomic_base<T*>;
    
    atomic() noexcept = default;
    constexpr atomic(T* val) noexcept : __base_type(val) {}
    atomic(const atomic&) = delete;
    atomic& operator=(const atomic&) = delete;
    atomic& operator=(const atomic&) volatile = delete;

    T* operator=(T* desired) volatile noexcept {
        this->store(desired);
        return desired;
    }
    T* operator=(T* desired) noexcept {
        this->store(desired);
        return desired;
    }

    operator T*() const volatile noexcept { return this->load(); }
    operator T*() const noexcept { return this->load(); }

    T* fetch_add(ptrdiff_t arg, memory_order order = memory_order_seq_cst) volatile noexcept {
        return __atomic_fetch_add(&this->_M_val, arg * sizeof(T), static_cast<int>(order));
    }
    T* fetch_add(ptrdiff_t arg, memory_order order = memory_order_seq_cst) noexcept {
        return __atomic_fetch_add(&this->_M_val, arg * sizeof(T), static_cast<int>(order));
    }

    T* fetch_sub(ptrdiff_t arg, memory_order order = memory_order_seq_cst) volatile noexcept {
        return __atomic_fetch_sub(&this->_M_val, arg * sizeof(T), static_cast<int>(order));
    }
    T* fetch_sub(ptrdiff_t arg, memory_order order = memory_order_seq_cst) noexcept {
        return __atomic_fetch_sub(&this->_M_val, arg * sizeof(T), static_cast<int>(order));
    }

    T* operator++(int) volatile noexcept { return fetch_add(1); }
    T* operator++(int) noexcept { return fetch_add(1); }
    T* operator--(int) volatile noexcept { return fetch_sub(1); }
    T* operator--(int) noexcept { return fetch_sub(1); }
    T* operator++() volatile noexcept { return fetch_add(1) + 1; }
    T* operator++() noexcept { return fetch_add(1) + 1; }
    T* operator--() volatile noexcept { return fetch_sub(1) - 1; }
    T* operator--() noexcept { return fetch_sub(1) - 1; }

    T* operator+=(ptrdiff_t arg) volatile noexcept { return fetch_add(arg) + arg; }
    T* operator+=(ptrdiff_t arg) noexcept { return fetch_add(arg) + arg; }
    T* operator-=(ptrdiff_t arg) volatile noexcept { return fetch_sub(arg) - arg; }
    T* operator-=(ptrdiff_t arg) noexcept { return fetch_sub(arg) - arg; }
};

} // namespace impl
} // namespace OZLIB_NAMESPACE
