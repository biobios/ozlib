#pragma once

#include <bits/integer_sequence.hpp>

namespace std {
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