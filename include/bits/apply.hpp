#pragma once

#include <bits/namespace.hpp>

#include <bits/integer_sequence.hpp>
#include <bits/invoke.hpp>
#include <bits/tuple_like.hpp>

namespace OZLIB_NAMESPACE {
namespace impl {
namespace apply_traits_helper {
template <typename F, typename Tuple, typename Indices>
struct apply_traits {
    inline static constexpr bool is_applicable = false;
    inline static constexpr bool is_nothrow_applicable = false;
};

template <typename F, typename Tuple, size_t... Is>
    requires requires {
        invoke(declval<F>(), get<Is>(declval<Tuple>())...);
    }
struct apply_traits<F, Tuple, index_sequence<Is...>> {
    using result_type = decltype(invoke(declval<F>(), get<Is>(declval<Tuple>())...));
    inline static constexpr bool is_applicable = true;
    inline static constexpr bool is_nothrow_applicable =
        noexcept(invoke(declval<F>(), get<Is>(declval<Tuple>())...));
    static constexpr result_type apply(F&& f, Tuple&& t) noexcept(is_nothrow_applicable) {
        return invoke(forward<F>(f), get<Is>(forward<Tuple>(t))...);
    }
};

template <typename F, typename Tuple>
using apply_result_t = typename apply_traits<F, Tuple, make_index_sequence<tuple_size_v<remove_cvref_t<Tuple>>>>::result_type;
template <typename F, typename Tuple>
inline static constexpr bool is_applicable_v = apply_traits<F, Tuple, make_index_sequence<tuple_size_v<remove_cvref_t<Tuple>>>>::is_applicable;
template <typename F, typename Tuple>
inline static constexpr bool is_nothrow_applicable_v = apply_traits<F, Tuple, make_index_sequence<tuple_size_v<remove_cvref_t<Tuple>>>>::is_nothrow_applicable;
}

template <typename F, typename Tuple>
struct apply_result {};

template <typename F, typename Tuple>
    requires requires {
        typename apply_traits_helper::apply_result_t<F, Tuple>;
    }
struct apply_result<F, Tuple> {
    using type = apply_traits_helper::apply_result_t<F, Tuple>;
};

template <typename F, typename Tuple>
using apply_result_t = typename apply_result<F, Tuple>::type;

template <typename F, typename Tuple>
struct is_applicable : bool_constant<apply_traits_helper::is_applicable_v<F, Tuple>> {};

template <typename F, typename Tuple>
inline constexpr bool is_applicable_v = is_applicable<F, Tuple>::value;

template <typename F, typename Tuple>
struct is_nothrow_applicable : bool_constant<apply_traits_helper::is_nothrow_applicable_v<F, Tuple>> {};

template <typename F, typename Tuple>
inline constexpr bool is_nothrow_applicable_v = is_nothrow_applicable<F, Tuple>::value;

template <typename F, typename Tuple>
constexpr apply_result_t<F, Tuple> apply(F&& f, Tuple&& t) noexcept(is_nothrow_applicable_v<F, Tuple>) {
    return apply_traits_helper::apply_traits<F, Tuple, make_index_sequence<tuple_size_v<remove_cvref_t<Tuple>>>>::apply(forward<F>(f), forward<Tuple>(t));
}

}
}