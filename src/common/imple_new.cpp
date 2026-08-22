#include <new>
#include <cstddef>
#include <bits/namespace.hpp>

[[nodiscard]] void* operator new(OZLIB_NAMESPACE::size_t size, void* ptr) noexcept {
    return ptr;
}

void operator delete(void* ptr, void*) noexcept {}