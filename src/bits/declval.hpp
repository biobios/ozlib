#pragma once

#include <bits/add_rvalue_reference.hpp>

namespace std {
namespace impl {
template <class T>
typename add_rvalue_reference<T>::type declval() noexcept;
}
}  // namespace std