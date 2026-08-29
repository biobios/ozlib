#pragma once

#include <bits/namespace.hpp>
#include <bits/atomic_memory_order.hpp>

namespace OZLIB_NAMESPACE {
namespace impl {

inline void atomic_thread_fence(memory_order order) noexcept {
    __atomic_thread_fence(static_cast<int>(order));
}

inline void atomic_signal_fence(memory_order order) noexcept {
    __atomic_signal_fence(static_cast<int>(order));
}

}
} // namespace OZLIB_NAMESPACE
