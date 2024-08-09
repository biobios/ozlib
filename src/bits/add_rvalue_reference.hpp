#pragma once

namespace std {
namespace impl {
namespace {
namespace helper_add_rvalue_reference {

template <typename T>
using ref = T&;

template <typename T>
concept is_referenceable = requires { typename ref<T>; };
}  // namespace helper_add_rvalue_reference
}  // namespace

template <class T>
struct add_rvalue_reference {
    using type = T;
};

template <helper_add_rvalue_reference::is_referenceable T>
struct add_rvalue_reference<T> {
    using type = T&&;
};

template <class T>
using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

namespace helper_add_rvalue_reference {}

}  // namespace impl
}  // namespace std