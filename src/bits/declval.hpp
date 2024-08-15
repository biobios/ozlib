#pragma once

#include <bits/reference_traits.hpp>

namespace std {
namespace impl {
template <class T>
typename add_rvalue_reference<T>::type declval() noexcept;
}
}  // namespace std