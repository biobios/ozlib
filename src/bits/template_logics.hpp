#pragma once

namespace std {
namespace impl {

template <bool Condition, class T = void>
struct enable_if {};

template <typename T>
struct enable_if<true, T> {
    using type = T;
};

template <bool Condition, class T = void>
using enable_if_t = typename enable_if<Condition, T>::type;

template <bool Condition, class T, class F>
struct conditional {
    using type = F;
};

template <class T, class F>
struct conditional<true, T, F> {
    using type = T;
};

template <bool Condition, class T, class F>
using conditional_t = typename conditional<Condition, T, F>::type;

}  // namespace impl
}  // namespace std
