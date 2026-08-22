#pragma once

#include <bits/namespace.hpp>

#include <bits/integer_sequence.hpp>

namespace OZLIB_NAMESPACE {
namespace impl {

template <typename... Args>
class tuple {
    using args_indices = make_index_sequence<sizeof...(Args)>;
    template <size_t... I>
    struct tuple_impl {

    };
};
}
}