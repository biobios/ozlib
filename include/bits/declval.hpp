#pragma once

#include <bits/namespace.hpp>

#include <bits/reference_traits.hpp>

namespace OZLIB_NAMESPACE {
namespace impl {
template <class T>
typename add_rvalue_reference<T>::type declval() noexcept;
}
}  // namespace OZLIB_NAMESPACE