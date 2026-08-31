#pragma once

#include <bits/namespace.hpp>

#include <bits/compound_traits.hpp>
#include <bits/launder.hpp>

namespace OZLIB_NAMESPACE {
namespace impl {
namespace exposition_only {
template <typename T>
constexpr void* voidify(T& ptr) noexcept {
    return addressof(ptr);
}
}

template <typename T, typename... Args>
constexpr T* construct_at(T* location, Args&&... args) {
    auto ptr = ::new (exposition_only::voidify(*location)) T(forward<Args>(args)...);
    if constexpr (is_array_v<T>) {
        return launder(ptr);
    } else {
        return ptr;
    }
}
}
}