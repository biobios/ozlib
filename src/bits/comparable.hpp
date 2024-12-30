#pragma once

#include <bits/common_traits.hpp>
#include <bits/exposition_only.hpp>
#include <bits/reference_traits.hpp>

namespace std {
namespace impl {
namespace exposition_only {

template <typename T, typename U>
concept weakly_equality_comparable_with =
    requires(const remove_reference_t<T>& t, const remove_reference_t<U>& u) {
        { t == u } -> boolean_testable;
        { t != u } -> boolean_testable;
        { u == t } -> boolean_testable;
        { u != t } -> boolean_testable;
    };

template <typename T, typename Cat>
concept compare_as = same_as<common_comparison_category_t<T, Cat>, Cat>;

template <typename T, typename U>
concept partialy_ordered_with =
    requires(const remove_reference_t<T>& t, const remove_reference_t<U>& u) {
        { t < u } -> boolean_testable;
        { t > u } -> boolean_testable;
        { t <= u } -> boolean_testable;
        { t >= u } -> boolean_testable;
        { u < t } -> boolean_testable;
        { u > t } -> boolean_testable;
        { u <= t } -> boolean_testable;
        { u >= t } -> boolean_testable;
    };

}  // namespace exposition_only

template <typename T>
concept equality_comparable =
    exposition_only::weakly_equality_comparable_with<T, T>;

template <typename T, typename U>
concept equality_comparable_with =
    equality_comparable<T> && equality_comparable<U> &&
    common_reference_with<const remove_reference_t<T>&,
                          const remove_reference_t<U>&> &&
    equality_comparable<common_reference_t<const remove_reference_t<T>&,
                                           const remove_reference_t<U>&>> &&
    exposition_only::weakly_equality_comparable_with<T, U>;

template <typename T>
concept totally_ordered =
    equality_comparable<T> && exposition_only::partialy_ordered_with<T, T>;

template <typename T, typename U>
concept totally_ordered_with =
    totally_ordered<T> && totally_ordered<U> &&
    equality_comparable_with<T, U> &&
    totally_ordered<common_reference_t<const remove_reference_t<T>&,
                                       const remove_reference_t<U>&>> &&
    exposition_only::partialy_ordered_with<T, U>;
}  // namespace impl
}  // namespace std