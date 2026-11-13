#pragma once

#include <bits/namespace.hpp>

namespace OZLIB_NAMESPACE {
namespace impl {
struct allocator_arg_t {
    explicit allocator_arg_t() = default;
};
inline constexpr allocator_arg_t allocator_arg{};
}
}