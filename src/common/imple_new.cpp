#include <new>

[[nodiscard]] void* operator new(std::size_t size, void* ptr)noexcept{
    return ptr;
}

void operator delete(void* ptr, void*)noexcept{}