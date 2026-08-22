#pragma once

#include <bits/namespace.hpp>

#include <compare>

namespace OZLIB_NAMESPACE {
namespace impl {
struct monostate {};

constexpr strong_ordering operator<=>(const monostate&, const monostate&) noexcept {
    return strong_ordering::equal;
}
constexpr bool operator==(const monostate&, const monostate&) noexcept {
    return true;
}
}
}