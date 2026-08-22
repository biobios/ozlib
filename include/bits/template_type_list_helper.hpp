#pragma once

#include <bits/namespace.hpp>

#include <bits/declval.hpp>
#include <bits/integer_sequence.hpp>
#include <bits/size_t.hpp>

namespace OZLIB_NAMESPACE {
namespace impl {
namespace type_list_helper {

template <typename... Types>
struct type_list {};

template <typename Left, typename Right>
struct type_list_cat;

template <typename... Left, typename... Right>
struct type_list_cat<type_list<Left...>, type_list<Right...>> {
    using type = type_list<Left..., Right...>;
};

template <typename Left, typename Right>
using type_list_cat_t = typename type_list_cat<Left, Right>::type;

template <typename = void>
struct dummy {
    template <typename U>
    dummy(U&&);
};

template <typename T, size_t N>
class get_type_at_index;

template <typename... Types, size_t N>
class get_type_at_index<type_list<Types...>, N> {
#if defined(__GNUC__) || !defined(__clang__)
    using type = __type_pack_element<N, Types...>;
#else
    using indices = make_index_sequence<N>;
    
    template <typename T>
    struct get_type_at_index_impl;
    
    template <size_t... I>
    struct get_type_at_index_impl<index_sequence<I...>> {
        template <typename T, typename... Rest>
        static T get_type_at_index_helper(decltype(dummy{I})..., dummy<T>, Rest...);

        using type = decltype(get_type_at_index_helper(dummy<Types>{}...));
    };
public:
    using type = typename get_type_at_index_impl<indices>::type;
#endif
};

}
}
}