#pragma once

#include <bits/namespace.hpp>

#include <bits/size_t.hpp>

namespace OZLIB_NAMESPACE {
namespace impl {

struct in_place_t {
    explicit in_place_t() = default;
};

template <typename T>
struct in_place_type_t {
    explicit in_place_type_t() = default;
};

template <size_t I>
struct in_place_index_t {
    explicit in_place_index_t() = default;
};

inline constexpr in_place_t in_place{};

template <typename T>
inline constexpr in_place_type_t<T> in_place_type{};

template <size_t I>
inline constexpr in_place_index_t<I> in_place_index{};
}
}