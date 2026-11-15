#pragma once

#include <bits/namespace.hpp>

#include <bits/declval.hpp>
#include <bits/integer_sequence.hpp>
#include <bits/size_t.hpp>

namespace OZLIB_NAMESPACE {
namespace impl {
namespace detail {

template <typename T, size_t N>
class get_type_at_index;

template <typename... Types>
struct type_list {
    template <template <typename...> typename Template>
    using apply = Template<Types...>;
    
    template <size_t N>
    using get_type_at_index = typename detail::get_type_at_index<type_list, N>::type;
    
    static constexpr size_t size() noexcept {
        return sizeof...(Types);
    }
};

template <typename Left, typename Right>
struct type_list_cat;

template <typename... Left, typename... Right>
struct type_list_cat<type_list<Left...>, type_list<Right...>> {
    using type = type_list<Left..., Right...>;
};

template <typename Left, typename Right>
using type_list_cat_t = typename type_list_cat<Left, Right>::type;

template <typename... Lists>
struct type_list_cat_multiple;

template <typename List>
struct type_list_cat_multiple<List> {
    using type = List;
};

template <typename List1, typename List2, typename... Rest>
struct type_list_cat_multiple<List1, List2, Rest...> {
    using type = type_list_cat_t<List1, typename type_list_cat_multiple<List2, Rest...>::type>;
};

template <typename... Lists>
using type_list_cat_multiple_t = typename type_list_cat_multiple<Lists...>::type;

template <typename = void>
struct dummy {
    dummy();
    template <typename U>
    dummy(U&&);
};

template <typename... Types, size_t N>
class get_type_at_index<type_list<Types...>, N> {
#if defined(__clang__) || (defined(__GNUC__) && __GNUC__ >= 14)
public:
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

template <typename T, size_t N>
using get_type_at_index_t = typename get_type_at_index<T, N>::type;

/**
 * @brief index_sequenceを受け取って、型リストから指定されたインデックスの型リストを取得する
 * @tparam T 型リスト
 * @tparam U index_sequence
 */
template <typename T, typename U>
struct get_type_list_at_indices;

template <typename... Types, size_t... I>
struct get_type_list_at_indices<type_list<Types...>, index_sequence<I...>> {
    using type = type_list<get_type_at_index_t<type_list<Types...>, I>...>;
};

template <typename T, typename U>
using get_type_list_at_indices_t = typename get_type_list_at_indices<T, U>::type;

}
}
}