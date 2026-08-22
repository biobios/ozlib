#pragma once

#include <bits/namespace.hpp>

#include <bits/destructible.hpp>

namespace OZLIB_NAMESPACE {
namespace impl {

namespace detail {
template <typename... Types>
union variant_storage;

template <typename T>
union variant_storage<T> {
    T value;
};

template <typename T, typename... Types>
union variant_storage<T, Types...> {
    T value;
    variant_storage<Types...> next;
};
}

template <typename... Types>
    requires(destructible<Types> && ...)
class variant {
public:
private:
    detail::variant_storage<Types...> storage;
};

}
}