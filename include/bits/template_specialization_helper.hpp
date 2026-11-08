#pragma once

#include <bits/namespace.hpp>

#include <bits/template_constants.hpp>

namespace OZLIB_NAMESPACE {
namespace impl {
namespace detail {
template <typename T, template <typename...> typename Template>
struct is_specialization_of : false_type {};

template <typename... Args, template <typename...> typename Template>
struct is_specialization_of<Template<Args...>, Template> : true_type {};

template <typename T, template <typename...> typename Template>
inline constexpr bool is_specialization_of_v = is_specialization_of<T, Template>::value;

template <typename T, template <auto...> typename Template>
struct is_specialization_of_value : false_type {};

template <auto... Args, template <auto...> typename Template>
struct is_specialization_of_value<Template<Args...>, Template> : true_type {};

template <typename T, template <auto...> typename Template>
inline constexpr bool is_specialization_of_value_v = is_specialization_of_value<T, Template>::value;

}
}
}