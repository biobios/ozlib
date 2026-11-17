#pragma once

#include <bits/namespace.hpp>

#include <bits/size_t.hpp>

namespace OZLIB_NAMESPACE {
namespace impl {
template <typename T>
class tuple_size;

template <typename T>
class tuple_size<const T> : public tuple_size<T> {};

template <typename T>
inline constexpr impl::size_t tuple_size_v = tuple_size<T>::value;

template <impl::size_t I, typename T>
class tuple_element;

template <impl::size_t I, typename T>
class tuple_element<I, const T> {
public:
    using type = const typename tuple_element<I, T>::type;
};

template <impl::size_t I, typename T>
using tuple_element_t = typename tuple_element<I, T>::type;
}
}