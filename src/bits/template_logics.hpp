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

}  // namespace impl
}  // namespace std
