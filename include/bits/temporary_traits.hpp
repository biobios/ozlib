#pragma once

#include <bits/namespace.hpp>

#include <bits/template_constants.hpp>

namespace OZLIB_NAMESPACE {
namespace impl {

#if defined(__has_builtin) && __has_builtin(__reference_constructs_from_temporary)
template <typename T, typename U>
struct reference_constructs_from_temporary
    : public bool_constant<__reference_constructs_from_temporary(T, U)> {};
#else
template <typename T, typename U>
struct reference_constructs_from_temporary : public false_type {};
#endif

template <typename T, typename U>
inline constexpr bool reference_constructs_from_temporary_v =
    reference_constructs_from_temporary<T, U>::value;

#if defined(__has_builtin) && __has_builtin(__reference_converts_from_temporary)
template <typename T, typename U>
struct reference_converts_from_temporary
    : public bool_constant<__reference_converts_from_temporary(T, U)> {};
#else
template <typename T, typename U>
struct reference_converts_from_temporary : public reference_constructs_from_temporary<T, U> {};
#endif

template <typename T, typename U>
inline constexpr bool reference_converts_from_temporary_v =
    reference_converts_from_temporary<T, U>::value;

}
}