#pragma once

#include <bits/ptrdiff_t.hpp>
#include <bits/template_constants.hpp>
#include <new>

namespace std {
namespace impl {

template <typename T>
class allocator {
   public:
    using value_type = T;
    using propagate_on_container_move_assignment = true_type;
    using difference_type = ptrdiff_t;
    using is_always_equal = true_type;

    constexpr allocator() noexcept = default;
    constexpr allocator(const allocator&) noexcept = default;
    template <typename U>
    constexpr allocator(const allocator<U>&) noexcept {};
    constexpr ~allocator() = default;
    constexpr allocator& operator=(const allocator&) = default;

    [[nodiscard]] constexpr T* allocate(size_t n) {
        align_val_t alignment = align_val_t(alignof(T));
        return static_cast<T*>(::operator new(n * sizeof(T), alignment));
    }

    constexpr void deallocate(T* p, size_t n) {
        ::operator delete(p, n * sizeof(T));
    }
};

template <typename T, typename U>
constexpr bool operator==(const allocator<T>&, const allocator<U>&) noexcept {
    return true;
}

template <typename T, typename U>
constexpr bool operator!=(const allocator<T>&, const allocator<U>&) noexcept {
    return false;
}

}  // namespace impl
}  // namespace std