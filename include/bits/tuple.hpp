#pragma once

#include <bits/namespace.hpp>

#include <bits/allocator_arg_t.hpp>
#include <bits/assignable.hpp>
#include <bits/constructible.hpp>
#include <bits/cvref_traits.hpp>
#include <bits/integer_sequence.hpp>
#include <bits/pair.hpp>
#include <bits/swappable.hpp>
#include <bits/template_constants.hpp>
#include <bits/template_logics.hpp>
#include <bits/template_specialization_helper.hpp>
#include <bits/template_type_list_helper.hpp>
#include <bits/temporary_traits.hpp>
#include <bits/tuple_like.hpp>
#include <bits/unwrap_ref_decay.hpp>

namespace OZLIB_NAMESPACE {


template <class T, impl::size_t N>
struct array;
namespace impl {
namespace detail {
template <typename T>
struct is_specialization_of_array : false_type {};
template <typename T, impl::size_t N>
struct is_specialization_of_array<array<T, N>> : true_type {};
}
template <typename... Args>
class tuple;
}

template <impl::size_t I, typename... Types>
constexpr tuple_element_t<I, impl::tuple<Types...>>& get(impl::tuple<Types...>&) noexcept requires(I < sizeof...(Types));
template <impl::size_t I, typename... Types>
constexpr tuple_element_t<I, impl::tuple<Types...>>&& get(impl::tuple<Types...>&&) noexcept requires(I < sizeof...(Types));
template <impl::size_t I, typename... Types>
constexpr const tuple_element_t<I, impl::tuple<Types...>>& get(const impl::tuple<Types...>&) noexcept requires(I < sizeof...(Types));
template <impl::size_t I, typename... Types>
constexpr const tuple_element_t<I, impl::tuple<Types...>>&& get(const impl::tuple<Types...>&&) noexcept requires(I < sizeof...(Types));

namespace impl{

namespace exposition_only {
template <typename T>
concept tuple_like =
    (impl::detail::is_specialization_of_array<T>::value ||
     impl::detail::is_specialization_of_v<remove_cvref_t<T>, pair> ||
//     detail::is_specialization_of_v<remove_cvref_t<T>, ranges::subrange> ||
     impl::detail::is_specialization_of_v<remove_cvref_t<T>, tuple>);
template <typename T, typename U>
concept different_from = !same_as<remove_cvref_t<T>, remove_cvref_t<U>>;
}

template <typename... Args>
class tuple {
    using args_indices = make_index_sequence<sizeof...(Args)>;
    using args_type_list = detail::type_list<Args...>;
    template <impl::size_t I>
    class element {
    public:
        using type = args_type_list::template get_type_at_index<I>;
        type value;
        constexpr element() requires(is_default_constructible_v<type>) : value() {}
        constexpr element(const element&) requires(is_copy_constructible_v<type>) = default;
        constexpr element(element&&) requires(is_move_constructible_v<type>) = default;
        template <typename U>
            requires(is_constructible_v<type, U>)
        constexpr element(U&& u) : value(forward<U>(u)) {}
        constexpr element& operator=(const element& other) requires(!is_reference_v<type>) = default;
        constexpr const element& operator=(const element& other) const requires(is_reference_v<type>) {
            value = other.value;
            return *this;
        }
        constexpr element& operator=(element&& other) requires(!is_reference_v<type>) = default;
        constexpr const element& operator=(element&& other) const requires(is_reference_v<type>) {
            value = move(other.value);
            return *this;
        }
        template <typename U>
        constexpr element& operator=(const U& other) requires(!is_reference_v<type>) {
            value = other;
            return *this;
        }
        template <typename U>
        constexpr const element& operator=(const U& other) const requires(is_reference_v<type>) {
            value = other;
            return *this;
        }
        template <typename U>
        constexpr element& operator=(U&& other) requires(!is_reference_v<type>) {
            value = move(other);
            return *this;
        }
        template <typename U>
        constexpr const element& operator=(U&& other) const requires(is_reference_v<type>) {
            value = move(other);
            return *this;
        }
        constexpr void swap(element& rhs) noexcept(is_nothrow_swappable_v<type>) {
            using OZLIB_NAMESPACE::swap;
            swap(value, rhs.value);
        }
        constexpr void swap(const element& rhs) const noexcept(is_nothrow_swappable_v<const type>) {
            using OZLIB_NAMESPACE::swap;
            swap(value, rhs.value);
        }
    };
    struct construct_tuple_like_tag {};
    template <impl::size_t... Is>
    class tuple_impl : public element<Is>... {
    public:
        constexpr tuple_impl() = default;
        constexpr tuple_impl(const tuple_impl&) = default;
        constexpr tuple_impl(tuple_impl&&) = default;
        constexpr tuple_impl(const Args&... args) requires(sizeof...(Args) != 0) : element<Is>{args}... {}
        template <typename... UArgs>
            requires(sizeof...(Args) != 0)
        constexpr tuple_impl(UArgs&&... args) : element<Is>{forward<UArgs>(args)}... {}
        template <typename... UArgs>
        constexpr tuple_impl(tuple<UArgs...>& other) : element<Is>{get<Is>(static_cast<decltype(other)>(other))}... {}
        template <typename... UArgs>
        constexpr tuple_impl(const tuple<UArgs...>& other) : element<Is>{get<Is>(static_cast<decltype(other)>(other))}... {}
        template <typename... UArgs>
        constexpr tuple_impl(tuple<UArgs...>&& other) : element<Is>{get<Is>(static_cast<decltype(other)>(other))}... {}
        template <typename... UArgs>
        constexpr tuple_impl(const tuple<UArgs...>&& other) : element<Is>{get<Is>(static_cast<decltype(other)>(other))}... {}
        template <typename U1, typename U2>
        constexpr tuple_impl(pair<U1, U2>& other) : element<0>{get<0>(static_cast<decltype(other)>(other))}, element<1>{get<1>(static_cast<decltype(other)>(other))} {}
        template <typename U1, typename U2>
        constexpr tuple_impl(const pair<U1, U2>& other) : element<0>{get<0>(static_cast<decltype(other)>(other))}, element<1>{get<1>(static_cast<decltype(other)>(other))} {}
        template <typename U1, typename U2>
        constexpr tuple_impl(pair<U1, U2>&& other) : element<0>{get<0>(static_cast<decltype(other)>(other))}, element<1>{get<1>(static_cast<decltype(other)>(other))} {}
        template <typename U1, typename U2>
        constexpr tuple_impl(const pair<U1, U2>&& other) : element<0>{get<0>(static_cast<decltype(other)>(other))}, element<1>{get<1>(static_cast<decltype(other)>(other))} {}
        template <exposition_only::tuple_like UTuple>
        constexpr tuple_impl(construct_tuple_like_tag, UTuple&& other) : element<Is>{get<Is>(static_cast<decltype(other)>(other))}... {}
        constexpr tuple_impl& operator=(const tuple_impl& other) = default;
        constexpr const tuple_impl& operator=(const tuple_impl& other) const {
            ((static_cast<const element<Is>&>(*this) = static_cast<const element<Is>&>(other)), ...);
            return *this;
        }
        constexpr tuple_impl& operator=(tuple_impl&& other) = default;
        constexpr const tuple_impl& operator=(tuple_impl&& other) const {
            ((static_cast<const element<Is>&>(*this) = static_cast<element<Is>&&>(other)), ...);
            return *this;
        }
        template <typename... UArgs>
        constexpr tuple_impl& operator=(const tuple<UArgs...>::template tuple_impl<Is...>& other) {
            ((static_cast<element<Is>&>(*this) = static_cast<const typename tuple<UArgs...>::template element<Is>&>(other)), ...);
            return *this;
        }
        template <typename... UArgs>
        constexpr const tuple_impl& operator=(const tuple<UArgs...>::template tuple_impl<Is...>& other) const {
            ((static_cast<const element<Is>&>(*this) = static_cast<const typename tuple<UArgs...>::template element<Is>&>(other)), ...);
            return *this;
        }
        template <typename... UArgs>
        constexpr tuple_impl& operator=(tuple<UArgs...>::template tuple_impl<Is...>&& other) {
            ((static_cast<element<Is>&>(*this) = static_cast<typename tuple<UArgs...>::template element<Is>&&>(other)), ...);
            return *this;
        }
        template <typename... UArgs>
        constexpr const tuple_impl& operator=(tuple<UArgs...>::template tuple_impl<Is...>&& other) const {
            ((static_cast<const element<Is>&>(*this) = static_cast<typename tuple<UArgs...>::template element<Is>&&>(other)), ...);
            return *this;
        }
        template <typename U1, typename U2>
        constexpr tuple_impl& operator=(const pair<U1, U2>& other) {
            static_cast<element<0>&>(*this) = get<0>(other);
            static_cast<element<1>&>(*this) = get<1>(other);
            return *this;
        }
        template <typename U1, typename U2>
        constexpr const tuple_impl& operator=(const pair<U1, U2>& other) const {
            static_cast<const element<0>&>(*this) = get<0>(other);
            static_cast<const element<1>&>(*this) = get<1>(other);
            return *this;
        }
        template <typename U1, typename U2>
        constexpr tuple_impl& operator=(pair<U1, U2>&& other) {
            static_cast<element<0>&>(*this) = get<0>(other);
            static_cast<element<1>&>(*this) = get<1>(other);
            return *this;
        }
        template <typename U1, typename U2>
        constexpr const tuple_impl& operator=(pair<U1, U2>&& other) const {
            static_cast<const element<0>&>(*this) = get<0>(other);
            static_cast<const element<1>&>(*this) = get<1>(other);
            return *this;
        }
        template <exposition_only::tuple_like UTuple>
        constexpr tuple_impl& operator=(UTuple&& other) {
            ((static_cast<element<Is>&>(*this) = get<Is>(static_cast<decltype(other)>(other))), ...);
            return *this;
        }
        template <exposition_only::tuple_like UTuple>
        constexpr const tuple_impl& operator=(UTuple&& other) const {
            ((static_cast<const element<Is>&>(*this) = get<Is>(static_cast<decltype(other)>(other))), ...);
            return *this;
        }
        constexpr void swap(tuple_impl& rhs) noexcept((is_nothrow_swappable_v<Args> && ...)) {
            ((static_cast<element<Is>&>(*this).swap(static_cast<element<Is>&>(rhs))), ...);
        }
        constexpr void swap(const tuple_impl& rhs) const noexcept((is_nothrow_swappable_v<const Args> && ...)) {
            ((static_cast<const element<Is>&>(*this).swap(static_cast<const element<Is>&>(rhs))), ...);
        }
    };
    template <typename T>
    struct get_tuple_impl;
    template <impl::size_t... Is>
    struct get_tuple_impl<index_sequence<Is...>> {
        using type = tuple_impl<Is...>;
    };
    using impl_type = typename get_tuple_impl<args_indices>::type;
    impl_type _impl;
    template <typename... UArgs>
    struct get_disambiguating_constraint {
        using type = true_type;
    };
    template <typename UArg>
    struct get_disambiguating_constraint<UArg> {
        using type = negation<is_same<remove_cvref_t<UArg>, tuple>>;
    };
    template <typename UArg, typename... UArgs>
        requires(sizeof...(Args) == 2 || sizeof...(Args) == 3)
    struct get_disambiguating_constraint<UArg, UArgs...> {
        using type = bool_constant<!is_same_v<remove_cvref_t<UArg>, allocator_arg_t> || is_same_v<remove_cvref_t<typename args_type_list::template get_type_at_index<0>>, allocator_arg_t>>;
    };
    template <typename T, typename U>
    struct delete_convert_tuple;
    template <impl::size_t... Is, typename U>
    struct delete_convert_tuple<index_sequence<Is...>, U> {
        static constexpr bool value = (reference_constructs_from_temporary_v<Args, decltype(get<Is>(declval<U>()))> || ...);
    };
    template <typename T, typename U, typename... UArgs>
    struct convertible_tuple;
    template <impl::size_t... Is, typename U, typename... UArgs>
    struct convertible_tuple<index_sequence<Is...>, U, UArgs...> {
        using T0 = typename args_type_list::template get_type_at_index<0>;
        using U0 = typename detail::type_list<UArgs...>::template get_type_at_index<0>;
        static constexpr bool value = sizeof...(Args) == sizeof...(UArgs) &&
            (is_constructible_v<Args, decltype(get<Is>(declval<U>()))> && ...) &&
            (sizeof...(Args) != 1 || (!is_convertible_v<U, T0> && !is_constructible_v<T0, U> && !is_same_v<T0, U0>));
    };
    template <typename T, typename U>
    struct explicit_convert_tuple;
    template <impl::size_t... Is, typename U>
    struct explicit_convert_tuple<index_sequence<Is...>, U> {
        static constexpr bool value = !(is_convertible_v<decltype(get<Is>(declval<U>())), Args> && ...);
    };
    template <typename U>
    struct delete_convert_pair {
        using T0 = typename args_type_list::template get_type_at_index<0>;
        using T1 = typename args_type_list::template get_type_at_index<1>;
        static constexpr bool value = 
            (reference_constructs_from_temporary_v<T0, decltype(get<0>(declval<U>()))> ||
            reference_constructs_from_temporary_v<T1, decltype(get<1>(declval<U>()))>);
    };
    template <typename U>
    struct convertible_pair {
        using T0 = typename args_type_list::template get_type_at_index<0>;
        using T1 = typename args_type_list::template get_type_at_index<1>;
        static constexpr bool value = sizeof...(Args) == 2 &&
            is_constructible_v<T0, decltype(get<0>(declval<U>()))> &&
            is_constructible_v<T1, decltype(get<1>(declval<U>()))>;
    };
    template <typename U>
    struct explicit_convert_pair {
        using T0 = typename args_type_list::template get_type_at_index<0>;
        using T1 = typename args_type_list::template get_type_at_index<1>;
        static constexpr bool value = !is_convertible_v<decltype(get<0>(declval<U>())), T0> ||
            !is_convertible_v<decltype(get<1>(declval<U>())), T1>;
    };
    template <typename T, typename U>
    struct convertible_tuple_like;
    template <impl::size_t... Is, typename U>
    struct convertible_tuple_like<index_sequence<Is...>, U> {
        static constexpr bool value =
            (is_constructible_v<Args, decltype(get<Is>(declval<U>()))> && ...) &&
            (sizeof...(Args) != 1 ||
             (!is_convertible_v<U, typename args_type_list::template get_type_at_index<0>> &&
              !is_constructible_v<typename args_type_list::template get_type_at_index<0>, U>));
    };
    template <typename T, typename U>
    using explicit_convert_tuple_like = explicit_convert_tuple<T, U>;
    template <typename T, typename U>
    using delete_convert_tuple_like = delete_convert_tuple<T, U>;
    template <typename T, typename U>
    struct assignable_tuple_like;
    template <impl::size_t... Is, typename U>
    struct assignable_tuple_like<index_sequence<Is...>, U> {
        static constexpr bool value = (is_assignable_v<Args&, decltype(get<Is>(declval<U>()))> && ...);
    };
    template <typename T, typename U>
    struct proxy_assignable_tuple_like;
    template <impl::size_t... Is, typename U>
    struct proxy_assignable_tuple_like<index_sequence<Is...>, U> {
        static constexpr bool value = (is_assignable_v<const Args&, decltype(get<Is>(declval<U>()))> && ...);
    };
public:
    explicit((detail::explicitly_default_constructible<Args> && ...))
        constexpr tuple() requires(is_default_constructible_v<Args> && ...) = default;
    explicit((conjunction_v<is_convertible<const Args&, Args>...>))
        constexpr tuple(const Args&... args) requires(sizeof...(Args) != 0 && (is_copy_constructible_v<Args> && ...))
        : _impl{args...} {}
    template <typename... UArgs>
        requires(sizeof...(UArgs) == sizeof...(Args) &&
            conjunction_v<typename get_disambiguating_constraint<UArgs...>::type, is_constructible<Args, UArgs>...> &&
            !(reference_constructs_from_temporary_v<Args, UArgs&&> || ...) )
    explicit((!conjunction_v<is_convertible<UArgs, Args>...>))
        constexpr tuple(UArgs&&... uargs) : _impl{forward<UArgs>(uargs)...} {}
    template <typename... UArgs>
        requires(sizeof...(UArgs) == sizeof...(Args) &&
            conjunction_v<typename get_disambiguating_constraint<UArgs...>::type, is_constructible<Args, UArgs>...> &&
            (reference_constructs_from_temporary_v<Args, UArgs&&> || ...) )
    explicit((!conjunction_v<is_convertible<UArgs, Args>...>))
        constexpr tuple(UArgs&&... uargs) = delete;
    tuple(const tuple&) = default;
    tuple(tuple&&) = default;
    template <typename... UArgs>
        requires(convertible_tuple<args_indices, tuple<UArgs...>&, UArgs...>::value)
    explicit(explicit_convert_tuple<args_indices, tuple<UArgs...>&>::value)
        constexpr tuple(tuple<UArgs...>& other) : _impl{static_cast<decltype(other)>(other)} {}
    template <typename... UArgs>
        requires(delete_convert_tuple<args_indices, tuple<UArgs...>&>::value)
    explicit(explicit_convert_tuple<args_indices, tuple<UArgs...>&>::value)
        constexpr tuple(tuple<UArgs...>& other) = delete;
    template <typename... UArgs>
        requires(convertible_tuple<args_indices, const tuple<UArgs...>&, UArgs...>::value)
    explicit(explicit_convert_tuple<args_indices, const tuple<UArgs...>&>::value)
        constexpr tuple(const tuple<UArgs...>& other) : _impl{static_cast<decltype(other)>(other)} {}
    template <typename... UArgs>
        requires(delete_convert_tuple<args_indices, const tuple<UArgs...>&>::value)
    explicit(explicit_convert_tuple<args_indices, const tuple<UArgs...>&>::value)
        constexpr tuple(const tuple<UArgs...>& other) = delete;
    template <typename... UArgs>
        requires(convertible_tuple<args_indices, tuple<UArgs...>&&, UArgs...>::value)
    explicit(explicit_convert_tuple<args_indices, tuple<UArgs...>&&>::value)
        constexpr tuple(tuple<UArgs...>&& other) : _impl{static_cast<decltype(other)>(other)} {}
    template <typename... UArgs>
        requires(delete_convert_tuple<args_indices, tuple<UArgs...>&&>::value)
    explicit(explicit_convert_tuple<args_indices, tuple<UArgs...>&&>::value)
        constexpr tuple(tuple<UArgs...>&& other) = delete;
    template <typename... UArgs>
        requires(convertible_tuple<args_indices, const tuple<UArgs...>&&, UArgs...>::value)
    explicit(explicit_convert_tuple<args_indices, const tuple<UArgs...>&&>::value)
        constexpr tuple(const tuple<UArgs...>&& other) : _impl{static_cast<decltype(other)>(other)} {}
    template <typename... UArgs>
        requires(delete_convert_tuple<args_indices, const tuple<UArgs...>&&>::value)
    explicit(explicit_convert_tuple<args_indices, const tuple<UArgs...>&&>::value)
        constexpr tuple(const tuple<UArgs...>&& other) = delete;
    template <typename U1, typename U2>
        requires(convertible_pair<tuple<U1, U2>&>::value)
    explicit(explicit_convert_pair<tuple<U1, U2>&>::value)
        constexpr tuple(pair<U1, U2>& other) : _impl{static_cast<decltype(other)>(other)} {}
    template <typename U1, typename U2>
        requires(delete_convert_pair<tuple<U1, U2>&>::value)
    explicit(explicit_convert_pair<tuple<U1, U2>&>::value)
        constexpr tuple(pair<U1, U2>& other) = delete;
    template <typename U1, typename U2>
        requires(convertible_pair<const tuple<U1, U2>&>::value)
    explicit(explicit_convert_pair<const tuple<U1, U2>&>::value)
        constexpr tuple(const pair<U1, U2>& other) : _impl{static_cast<decltype(other)>(other)} {}
    template <typename U1, typename U2>
        requires(delete_convert_pair<const tuple<U1, U2>&>::value)
    explicit(explicit_convert_pair<const tuple<U1, U2>&>::value)
        constexpr tuple(const pair<U1, U2>& other) = delete;
    template <typename U1, typename U2>
        requires(convertible_pair<tuple<U1, U2>&&>::value)
    explicit(explicit_convert_pair<tuple<U1, U2>&&>::value)
        constexpr tuple(pair<U1, U2>&& other) : _impl{static_cast<decltype(other)>(other)} {}
    template <typename U1, typename U2>
        requires(delete_convert_pair<tuple<U1, U2>&&>::value)
    explicit(explicit_convert_pair<tuple<U1, U2>&&>::value)
        constexpr tuple(pair<U1, U2>&& other) = delete;
    template <typename U1, typename U2>
        requires(convertible_pair<const tuple<U1, U2>&&>::value)
    explicit(explicit_convert_pair<const tuple<U1, U2>&&>::value)
        constexpr tuple(const pair<U1, U2>&& other) : _impl{static_cast<decltype(other)>(other)} {}
    template <typename U1, typename U2>
        requires(delete_convert_pair<const tuple<U1, U2>&&>::value)
    explicit(explicit_convert_pair<const tuple<U1, U2>&&>::value)
        constexpr tuple(const pair<U1, U2>&& other) = delete;
    template <exposition_only::tuple_like UTuple>
        requires(exposition_only::different_from<UTuple, tuple> &&
            // !detail::is_specialization_of_v<remove_cvref_t<UTuple>, ranges::subrange> &&
            sizeof...(Args) == tuple_size_v<remove_cvref_t<UTuple>> &&
            convertible_tuple_like<args_indices, UTuple>::value)
    explicit(explicit_convert_tuple_like<args_indices, UTuple>::value)
        constexpr tuple(UTuple&& other) : _impl{construct_tuple_like_tag{}, static_cast<decltype(other)>(other)} {}
    template <exposition_only::tuple_like UTuple>
        requires(delete_convert_tuple_like<args_indices, UTuple>::value)
    explicit(explicit_convert_tuple_like<args_indices, UTuple>::value)
        constexpr tuple(UTuple&& other) = delete;
    
    constexpr tuple& operator=(const tuple&) = default;
    constexpr tuple& operator=(const tuple&) requires(!(is_copy_assignable_v<Args> && ...)) = delete;
    constexpr const tuple& operator=(const tuple& other) const requires(is_copy_assignable_v<const Args> && ...) {
        _impl = static_cast<const tuple&>(other)._impl;
        return *this;
    }
    constexpr tuple& operator=(tuple&&) noexcept((is_nothrow_move_assignable_v<Args> && ...)) requires(is_move_assignable_v<Args> && ...) = default;
    constexpr const tuple& operator=(tuple&& other) const {
        _impl = static_cast<tuple&&>(other)._impl;
        return *this;
    }
    template <typename... UArgs>
        requires(sizeof...(UArgs) == sizeof...(Args) &&
            (is_assignable_v<Args&, const UArgs&> && ...))
    constexpr tuple& operator=(const tuple<UArgs...>& other) {
        _impl.template operator=<UArgs...>(static_cast<const tuple<UArgs...>&>(other)._impl);
        return *this;
    }
    template <typename... UArgs>
        requires(sizeof...(UArgs) == sizeof...(Args) &&
            (is_assignable_v<const Args&, const UArgs&> && ...))
    constexpr const tuple& operator=(const tuple<UArgs...>& other) const {
        _impl.template operator=<UArgs...>(static_cast<const tuple<UArgs...>&>(other)._impl);
        return *this;
    }
    template <typename... UArgs>
        requires(sizeof...(UArgs) == sizeof...(Args) &&
            (is_assignable_v<Args&, UArgs> && ...))
    constexpr tuple& operator=(tuple<UArgs...>&& other) {
        _impl.template operator=<UArgs...>(static_cast<tuple<UArgs...>&&>(other)._impl);
        return *this;
    }
    template <typename... UArgs>
        requires(sizeof...(UArgs) == sizeof...(Args) &&
            (is_assignable_v<const Args&, UArgs> && ...))
    constexpr const tuple& operator=(tuple<UArgs...>&& other) const {
        _impl.template operator=<UArgs...>(static_cast<tuple<UArgs...>&&>(other)._impl);
        return *this;
    }
    template <typename U1, typename U2>
        requires(sizeof...(Args) == 2 &&
            is_assignable_v<typename args_type_list::template get_type_at_index<0>&, const U1&> &&
            is_assignable_v<typename args_type_list::template get_type_at_index<1>&, const U2&>)
    constexpr tuple& operator=(const pair<U1, U2>& other) {
        _impl = static_cast<const pair<U1, U2>&>(other);
        return *this;
    }
    template <typename U1, typename U2>
        requires(sizeof...(Args) == 2 &&
            is_assignable_v<const typename args_type_list::template get_type_at_index<0>&, const U1&> &&
            is_assignable_v<const typename args_type_list::template get_type_at_index<1>&, const U2&>)
    constexpr const tuple& operator=(const pair<U1, U2>& other) const {
        _impl = static_cast<const pair<U1, U2>&>(other);
        return *this;
    }
    template <typename U1, typename U2>
        requires(sizeof...(Args) == 2 &&
            is_assignable_v<typename args_type_list::template get_type_at_index<0>&, U1> &&
            is_assignable_v<typename args_type_list::template get_type_at_index<1>&, U2>)
    constexpr tuple& operator=(pair<U1, U2>&& other) {
        _impl = static_cast<pair<U1, U2>&&>(other);
        return *this;
    }
    template <typename U1, typename U2>
        requires(sizeof...(Args) == 2 &&
            is_assignable_v<const typename args_type_list::template get_type_at_index<0>&, U1> &&
            is_assignable_v<const typename args_type_list::template get_type_at_index<1>&, U2>)
    constexpr const tuple& operator=(pair<U1, U2>&& other) const {
        _impl = static_cast<pair<U1, U2>&&>(other);
        return *this;
    }
    template <exposition_only::tuple_like UTuple>
        requires(tuple_size_v<remove_cvref_t<UTuple>> == sizeof...(Args) &&
                exposition_only::different_from<UTuple, tuple> &&
                // !detail::is_specialization_of_v<remove_cvref_t<UTuple>, ranges::subrange> &&
                assignable_tuple_like<args_indices, UTuple>::value)
    constexpr tuple& operator=(UTuple&& other) {
        _impl = static_cast<decltype(other)>(other);
        return *this;
    }
    template <exposition_only::tuple_like UTuple>
        requires(tuple_size_v<remove_cvref_t<UTuple>> == sizeof...(Args) &&
                exposition_only::different_from<UTuple, tuple> &&
                // !detail::is_specialization_of_v<remove_cvref_t<UTuple>, ranges::subrange> &&
                proxy_assignable_tuple_like<args_indices, UTuple>::value)
    constexpr const tuple& operator=(UTuple&& other) const {
        _impl = static_cast<decltype(other)>(other);
        return *this;
    }
    constexpr void swap(tuple& rhs) noexcept((is_nothrow_swappable_v<Args> && ...)) requires(is_swappable_v<Args> && ...) {
        _impl.swap(static_cast<tuple&>(rhs)._impl);
    }
    constexpr void swap(const tuple& rhs) const noexcept((is_nothrow_swappable_v<const Args> && ...)) requires(is_swappable_v<const Args> && ...) {
        _impl.swap(static_cast<const tuple&>(rhs)._impl);
    }
    
    template <size_t I, typename... Types>
    friend constexpr tuple_element_t<I, tuple<Types...>>& OZLIB_NAMESPACE::get(tuple<Types...>&) noexcept requires(I < sizeof...(Types));
    template <size_t I, typename... Types>
    friend constexpr tuple_element_t<I, tuple<Types...>>&& OZLIB_NAMESPACE::get(tuple<Types...>&&) noexcept requires(I < sizeof...(Types));
    template <size_t I, typename... Types>
    friend constexpr const tuple_element_t<I, tuple<Types...>>& OZLIB_NAMESPACE::get(const tuple<Types...>&) noexcept requires(I < sizeof...(Types));
    template <size_t I, typename... Types>
    friend constexpr const tuple_element_t<I, tuple<Types...>>&& OZLIB_NAMESPACE::get(const tuple<Types...>&&) noexcept requires(I < sizeof...(Types));
    // template <typename T, typename... Types>
    // friend constexpr T& get(tuple<Types...>&) noexcept;
    // template <typename T, typename...Types>
    // friend constexpr T&& get(tuple<Types...>&&) noexcept;
    // template <typename T, typename... Types>
    // friend constexpr const T& get(const tuple<Types...>&) noexcept;
    // template <typename T, typename... Types>
    // friend constexpr const T&& get(const tuple<Types...>&&) noexcept;
};

template <typename... Types>
constexpr tuple<unwrap_ref_decay_t<Types>...> make_tuple(Types&&... args) {
    return tuple<unwrap_ref_decay_t<Types>...>(forward<Types>(args)...);
}

template <typename... Types>
constexpr tuple<Types&&...> forward_as_tuple(Types&&... args) noexcept {
    return tuple<Types&&...>(forward<Types>(args)...);
}

template <typename... Types>
constexpr tuple<Types&...> tie(Types&... args) noexcept {
    return tuple<Types&...>(args...);
}

namespace detail {
template <exposition_only::tuple_like T>
struct tuple_traits {
    template <typename IndexSequence>
    struct get_tuple_types;
    template <impl::size_t... Is>
    struct get_tuple_types<index_sequence<Is...>> {
        using types = detail::type_list<tuple_element_t<Is, remove_cvref_t<T>>...>;
    };

    using types = typename get_tuple_types<make_index_sequence<tuple_size_v<remove_cvref_t<T>>>>::types;
};
template <exposition_only::tuple_like... T>
struct tuple_cat_helper {
    using types = typename detail::type_list_cat_multiple_t<typename tuple_traits<T>::types...>;
    static constexpr size_t size = types::size();
    using result = types::template apply<tuple>;
    template <typename... Types1, size_t... I1, size_t... I2, exposition_only::tuple_like Tuple>
    static constexpr tuple<Types1..., tuple_element_t<I2, remove_cvref_t<Tuple>>...> forward_merge(tuple<Types1...>&& t1, Tuple&& t2, index_sequence<I1...>, index_sequence<I2...>) {
        return forward_as_tuple(forward<Types1>(get<I1>(forward<tuple<Types1...>>(t1)))..., forward<tuple_element_t<I2, remove_cvref_t<Tuple>>>(get<I2>(forward<Tuple>(t2)))...);
    }

    template <typename... Types1, exposition_only::tuple_like Tuple, exposition_only::tuple_like... Tuples>
    static constexpr auto forward_cat(tuple<Types1...>&& t1, Tuple&& t2, Tuples&&... tuples) {
        if constexpr (sizeof...(Tuples) == 0) {
            return forward_merge(move(t1), forward<Tuple>(t2), make_index_sequence<sizeof...(Types1)>(), make_index_sequence<tuple_size_v<remove_cvref_t<Tuple>>>());
        } else {
            return forward_cat(forward_merge(move(t1), forward<Tuple>(t2), make_index_sequence<sizeof...(Types1)>(), make_index_sequence<tuple_size_v<remove_cvref_t<Tuple>>>()), forward<Tuples>(tuples)...);
        }
    }
};
}

template <exposition_only::tuple_like... Tuples>
constexpr detail::tuple_cat_helper<Tuples...>::result tuple_cat(Tuples&&... tuples) {
    return typename detail::tuple_cat_helper<Tuples...>::result{detail::tuple_cat_helper<Tuples...>::forward_cat(tuple<>{}, forward<Tuples>(tuples)...)};
}


namespace exposition_only {
struct ignore_type {
    constexpr const ignore_type& operator=(const auto&) const noexcept { return *this; }
};
}
inline constexpr exposition_only::ignore_type ignore{};
}

template <impl::size_t I, typename... Types>
constexpr tuple_element_t<I, impl::tuple<Types...>>& get(impl::tuple<Types...>& t) noexcept requires(I < sizeof...(Types)) {
    return static_cast<impl::tuple<Types...>::template element<I>&>(t._impl).value;
}
template <impl::size_t I, typename... Types>
constexpr tuple_element_t<I, impl::tuple<Types...>>&& get(impl::tuple<Types...>&& t) noexcept requires(I < sizeof...(Types)) {
    return static_cast<tuple_element_t<I, impl::tuple<Types...>>&&>(static_cast<impl::tuple<Types...>::template element<I>&&>(t._impl).value);
}
template <impl::size_t I, typename... Types>
constexpr const tuple_element_t<I, impl::tuple<Types...>>& get(const impl::tuple<Types...>& t) noexcept requires(I < sizeof...(Types)) {
    return static_cast<const impl::tuple<Types...>::template element<I>&>(t._impl).value;
}
template <impl::size_t I, typename... Types>
constexpr const tuple_element_t<I, impl::tuple<Types...>>&& get(const impl::tuple<Types...>&& t) noexcept requires(I < sizeof...(Types)) {
    return static_cast<const tuple_element_t<I, impl::tuple<Types...>>&&>(static_cast<const impl::tuple<Types...>::template element<I>&&>(t._impl).value);
}

template <typename... Types>
class tuple_size<impl::tuple<Types...>> : public impl::integral_constant<impl::size_t, sizeof...(Types)> {};

template <impl::size_t I, typename... Types>
class tuple_element<I, impl::tuple<Types...>> {
public:
    using type = typename impl::detail::type_list<Types...>::template get_type_at_index<I>;
};

template <typename... Types>
constexpr void swap(impl::tuple<Types...>& x, impl::tuple<Types...>& y) noexcept(noexcept(x.swap(y))) {
    x.swap(y);
}

template <typename... Types>
constexpr void swap(const impl::tuple<Types...>& x, const impl::tuple<Types...>& y) noexcept(noexcept(x.swap(y))) {
    x.swap(y);
}

}