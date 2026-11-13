#pragma once

#include <bits/namespace.hpp>

#include <bits/exposition_only.hpp>

namespace OZLIB_NAMESPACE {
namespace impl {
template <exposition_only::dereferenceable I>
using iter_reference_t = decltype(*declval<I&>());

template <typename Out, typename T>
concept indirectly_writable =
    requires(Out&& o, T&& t) {
        *o = forward<T>(t);
        *forward<Out>(o) = forward<T>(t);
        const_cast<const iter_reference_t<Out>&&>(*o) = forward<T>(t);
        const_cast<const iter_reference_t<Out>&&>(*forward<Out>(o)) = forward<T>(t);
    };
}
}