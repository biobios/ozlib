#pragma once

#include <bits/namespace.hpp>

#include <bits/compound_traits.hpp>
#include <bits/iterator_utils.hpp>

namespace OZLIB_NAMESPACE {
namespace impl {
template <typename ForwardIterator>
constexpr void destroy(ForwardIterator first, ForwardIterator last) {
    for (; first != last; ++first) {
        destroy_at(addressof(*first));
    }
};

template <typename T>
constexpr void destroy_at(T* location) {
    if constexpr (is_array_v<T>) {
        destroy(begin(*location), end(*location));
    }
}
}
}