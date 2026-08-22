#pragma once

#include <bits/namespace.hpp>

#include <compare>

namespace OZLIB_NAMESPACE {
namespace impl {
struct monostate {};

constexpr OZLIB_NAMESPACE::strong_ordering operator<=>(const monostate&, const monostate&) noexcept {
    return OZLIB_NAMESPACE::strong_ordering::equal;
}
}
}