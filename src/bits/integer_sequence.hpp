#pragma once

#include <bits/size_t.hpp>

namespace std {
namespace impl {
template <typename T, T... I>
struct integer_sequence {
    using value_type = T;
    static constexpr size_t size() noexcept { return sizeof...(I); }
};

template <size_t... I>
using index_sequence = integer_sequence<size_t, I...>;

namespace detail {

template <typename T, typename Left, typename Right>
struct make_integer_sequence_cat;

template <typename T, T... Left, T... Right>
struct make_integer_sequence_cat<T, integer_sequence<T, Left...>, integer_sequence<T, Right...>> {
    using type = integer_sequence<T, Left..., Right...>;
};

template <typename T, T begin, T end>
struct make_integer_sequence_range {
    using left = typename make_integer_sequence_range<T, begin, (begin + end) / 2>::type;
    using right = typename make_integer_sequence_range<T, (begin + end) / 2, end>::type;
    using type = typename make_integer_sequence_cat<T, left, right>::type;
};

template <typename T, T begin_end>
struct make_integer_sequence_range<T, begin_end, begin_end> {
    using type = integer_sequence<T>;
};

template <typename T, T begin, T end>
    requires (begin + 1 == end)
struct make_integer_sequence_range<T, begin, end> {
    using type = integer_sequence<T, begin>;
};

}

template <typename T, T N>
using make_integer_sequence = typename detail::make_integer_sequence_range<T, 0, N>::type;

template <size_t N>
using make_index_sequence = make_integer_sequence<size_t, N>;

template <typename... Types>
using index_sequence_for = make_index_sequence<sizeof...(Types)>;

}
}