#pragma once

#include <bits/comparison_categories.hpp>
#include <bits/compound_traits.hpp>
#include <bits/convertible.hpp>
#include <bits/cvref_traits.hpp>
#include <bits/decay.hpp>
#include <bits/fundamental_traits.hpp>
#include <bits/same.hpp>
#include <bits/type_identity.hpp>

namespace std {
namespace impl {

namespace exposition_only {
template <typename X, typename Y>
using cond_res = decltype(false ? declval<X (&)()>()() : declval<Y (&)()>()());

template <typename FROM, typename TO>
struct copycv_helper {
    using type = TO;
};

template <typename FROM, typename TO>
struct copycv_helper<FROM const, TO> {
    using type = TO const;
};

template <typename FROM, typename TO>
struct copycv_helper<FROM volatile, TO> {
    using type = TO volatile;
};

template <typename FROM, typename TO>
struct copycv_helper<FROM const volatile, TO> {
    using type = TO const volatile;
};

template <typename FROM, typename TO>
using copycv = typename copycv_helper<FROM, TO>::type;

template <typename A, typename B, typename X = remove_reference_t<A>,
          typename Y = remove_reference_t<B>>
struct COMMON_REF_HELPER;

template <typename A, typename B, typename X, typename Y>
    requires(is_lvalue_reference_v<A> && is_lvalue_reference_v<B> &&
             requires { typename cond_res<copycv<X, Y>&, copycv<Y, X>&>; } &&
             is_reference_v<cond_res<copycv<X, Y>&, copycv<Y, X>&>>)
struct COMMON_REF_HELPER<A, B, X, Y> {
    using type = cond_res<copycv<X, Y>&, copycv<Y, X>&>;
};

template <typename A, typename B, typename X, typename Y>
    requires(
        is_rvalue_reference_v<A> && is_rvalue_reference_v<B> &&
        requires {
            typename type_identity<
                remove_reference_t<typename COMMON_REF_HELPER<X, Y>::type>&&>;
        } &&
        is_convertible_v<
            A, remove_reference_t<typename COMMON_REF_HELPER<X, Y>::type> &&> &&
        is_convertible_v<
            B, remove_reference_t<typename COMMON_REF_HELPER<X, Y>::type> &&>)
struct COMMON_REF_HELPER<A, B, X, Y> {
    using type = remove_reference_t<typename COMMON_REF_HELPER<X, Y>::type>&&;
};

template <typename A, typename B, typename X, typename Y>
    requires(
        is_rvalue_reference_v<A> && is_lvalue_reference_v<B> &&
        requires { typename COMMON_REF_HELPER<const X&, Y&>::type; } &&
        is_convertible_v<A, typename COMMON_REF_HELPER<const X&, Y&>::type>)
struct COMMON_REF_HELPER<A, B, X, Y> {
    using type = typename COMMON_REF_HELPER<const X&, Y&>::type;
};

template <typename A, typename B, typename X, typename Y>
    requires(is_lvalue_reference_v<A> && is_rvalue_reference_v<B>)
struct COMMON_REF_HELPER<A, B, X, Y> {
    using type = typename COMMON_REF_HELPER<B, A>::type;
};

template <typename A, typename B>
using common_ref = typename COMMON_REF_HELPER<A, B>::type;

template <typename FROM, typename TO>
struct copyref_helper {
    using type = TO;
};

template <typename FROM, typename TO>
struct copyref_helper<FROM&, TO> {
    using type = TO&;
};

template <typename FROM, typename TO>
struct copyref_helper<FROM&&, TO> {
    using type = TO&&;
};

template <typename FROM, typename TO>
using copyref = typename copyref_helper<FROM, TO>::type;

template <typename FROM>
struct xref {
    template <typename TO>
    using type = copyref<FROM, copycv<FROM, TO>>;
};

template <typename X>
using cref = add_lvalue_reference_t<const remove_reference_t<X>>;
}  // namespace exposition_only

template <typename...>
struct common_type {};

template <typename... T>
using common_type_t = typename common_type<T...>::type;

template <typename T>
struct common_type<T> {
    using type = common_type_t<T, T>;
};

template <typename T1, typename T2>
    requires(!(is_same_v<T1, typename decay<T1>::type> &&
               is_same_v<T2, decay_t<T2>>))
struct common_type<T1, T2> {
    using type = common_type_t<decay_t<T1>, decay_t<T2>>;
};

template <typename T1, typename T2>
    requires(requires {
        typename decay_t<decltype(false ? declval<decay_t<T1>>()
                                        : declval<decay_t<T2>>())>;
    })
struct common_type<T1, T2> {
    using type = decay_t<decltype(false ? declval<decay_t<T1>>()
                                        : declval<decay_t<T2>>())>;
};

template <typename T1, typename T2>
    requires(requires {
        typename exposition_only::cond_res<exposition_only::cref<decay_t<T1>>,
                                           exposition_only::cref<decay_t<T2>>>;
    })
struct common_type<T1, T2> {
    using type =
        decay_t<exposition_only::cond_res<exposition_only::cref<decay_t<T1>>,
                                          exposition_only::cref<decay_t<T2>>>>;
};

template <typename T1, typename T2, typename... P>
struct common_type<T1, T2, P...> {
    using type = common_type_t<common_type_t<T1, T2>, P...>;
};

template <typename T, typename U, template <typename> typename TQual,
          template <typename> typename UQual>
struct basic_common_reference;

template <typename...>
struct common_reference {};

template <typename... Types>
using common_reference_t = typename common_reference<Types...>::type;

template <typename T>
struct common_reference<T> {
    using type = T;
};

template <typename T1, typename T2>
    requires(is_reference_v<T1> && is_reference_v<T2> &&
             requires { typename exposition_only::common_ref<T1, T2>; })
struct common_reference<T1, T2> {
    using type = exposition_only::common_ref<T1, T2>;
};

template <typename T1, typename T2>
    requires(requires {
        typename basic_common_reference<
            remove_cvref_t<T1>, remove_cvref_t<T2>,
            exposition_only::xref<T1>::template type,
            exposition_only::xref<T2>::template type>::type;
    })
struct common_reference<T1, T2> {
    using type = typename basic_common_reference<
        remove_cvref_t<T1>, remove_cvref_t<T2>,
        exposition_only::xref<T1>::template type,
        exposition_only::xref<T2>::template type>::type;
};

template <typename T1, typename T2>
    requires(requires { typename exposition_only::cond_res<T1, T2>; })
struct common_reference<T1, T2> {
    using type = exposition_only::cond_res<T1, T2>;
};

template <typename T1, typename T2>
    requires(requires { typename common_type_t<T1, T2>; })
struct common_reference<T1, T2> {
    using type = common_type_t<T1, T2>;
};

template <typename T1, typename T2, typename... P>
    requires(requires { typename common_reference_t<T1, T2>; })
struct common_reference<T1, T2, P...> {
    using type = common_reference_t<common_reference_t<T1, T2>, P...>;
};

template <typename...>
struct common_comparison_category;

template <typename... Types>
using common_comparison_category_t =
    typename common_comparison_category<Types...>::type;

// 0個の型の場合
template <>
struct common_comparison_category<> {
    using type = strong_ordering;
};

// 1個の型の場合
template <>
struct common_comparison_category<partial_ordering> {
    using type = partial_ordering;
};

template <>
struct common_comparison_category<weak_ordering> {
    using type = weak_ordering;
};

template <>
struct common_comparison_category<strong_ordering> {
    using type = strong_ordering;
};

template <typename T>
struct common_comparison_category<T> {
    using type = void;
};

// 2個の型の場合
template <typename T>
struct common_comparison_category<strong_ordering, T>
    : common_comparison_category<T> {};

template <typename T>
    requires(!same_as<T, strong_ordering>)
struct common_comparison_category<weak_ordering, T>
    : common_comparison_category<T> {};

template <typename T>
    requires(!same_as<T, strong_ordering> && !same_as<T, weak_ordering>)
struct common_comparison_category<partial_ordering, T>
    : common_comparison_category<T> {};

template <typename T>
struct common_comparison_category<void, T> {
    using type = void;
};

template <typename T, typename U>
struct common_comparison_category<T, U>
    : common_comparison_category<common_comparison_category_t<U>,
                                 common_comparison_category_t<T>> {};

// 3個以上の型の場合
template <typename T, typename U, typename... P>
struct common_comparison_category<T, U, P...>
    : common_comparison_category<common_comparison_category_t<T, U>, P...> {};

// concepts
template <typename T, typename U>
concept common_reference_with =
    same_as<common_reference_t<T, U>, common_reference_t<U, T>> &&
    convertible_to<T, common_reference_t<T, U>> &&
    convertible_to<U, common_reference_t<T, U>>;
}  // namespace impl
}  // namespace std
