#pragma once

#include <bits/namespace.hpp>

#include <bits/template_constants.hpp>

namespace OZLIB_NAMESPACE {
namespace impl {
template <typename T, typename U>
struct reference_constructs_from_temporary
    : public bool_constant<__reference_constructs_from_temporary(T, U)> {};

template <typename T, typename U>
inline constexpr bool reference_constructs_from_temporary_v =
    reference_constructs_from_temporary<T, U>::value;

template <typename T, typename U>
struct reference_converts_from_temporary
    : public bool_constant<__reference_converts_from_temporary(T, U)> {};

template <typename T, typename U>
inline constexpr bool reference_converts_from_temporary_v =
    reference_converts_from_temporary<T, U>::value;

}
}