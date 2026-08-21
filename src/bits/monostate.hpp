#pragma once

#include <compare>

namespace std {
namespace impl {
struct monostate {};

constexpr std::strong_ordering operator<=>(const monostate&, const monostate&) noexcept {
    return std::strong_ordering::equal;
}
}
}