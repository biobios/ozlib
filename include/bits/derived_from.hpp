#pragma once

#include <bits/namespace.hpp>

#include <bits/inheritance_traits.hpp>
#include <bits/convertible.hpp>

namespace OZLIB_NAMESPACE {
namespace impl {
template <typename Derived, typename Base>
concept derived_from = is_base_of_v<Base, Derived> && is_convertible_v<const volatile Derived*, const volatile Base*>;
}
}