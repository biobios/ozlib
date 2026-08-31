#pragma once

#include <bits/namespace.hpp>

#include <bits/pointer_traits.hpp>
#include <bits/signed_traits.hpp>

namespace OZLIB_NAMESPACE {
namespace impl {
namespace allocator_traits_helper {
template <typename Alloc>
struct get_pointer {
    using type = typename Alloc::value_type*;
};

template <typename Alloc>
    requires requires { typename Alloc::pointer; }
struct get_pointer<Alloc> {
    using type = typename Alloc::pointer;
};

template <typename Alloc>
struct get_const_pointer {
    using type = typename pointer_traits<typename get_pointer<Alloc>::type>::template rebind<const typename Alloc::value_type>;
};

template <typename Alloc>
    requires requires { typename Alloc::const_pointer; }
struct get_const_pointer<Alloc> {
    using type = typename Alloc::const_pointer;
};

template <typename Alloc>
struct get_void_pointer {
    using type = typename pointer_traits<typename get_pointer<Alloc>::type>::template rebind<void>;
};

template <typename Alloc>
    requires requires { typename Alloc::void_pointer; }
struct get_void_pointer<Alloc> {
    using type = typename Alloc::void_pointer;
};

template <typename Alloc>
struct get_const_void_pointer {
    using type = typename pointer_traits<typename get_pointer<Alloc>::type>::template rebind<const void>;
};

template <typename Alloc>
    requires requires { typename Alloc::const_void_pointer; }
struct get_const_void_pointer<Alloc> {
    using type = typename Alloc::const_void_pointer;
};

template <typename Alloc>
struct get_difference_type {
    using type = typename pointer_traits<typename get_pointer<Alloc>::type>::difference_type;
};

template <typename Alloc>
    requires requires { typename Alloc::difference_type; }
struct get_difference_type<Alloc> {
    using type = typename Alloc::difference_type;
};

template <typename Alloc>
struct get_size_type {
    using type = make_unsigned_t<typename get_difference_type<Alloc>::type>;
};

template <typename Alloc>
    requires requires { typename Alloc::size_type; }
struct get_size_type<Alloc> {
    using type = typename Alloc::size_type;
};

}
template <typename Alloc>
struct allocator_traits {
    using allocator_type = Alloc;
    using value_type = typename Alloc::value_type;
    using pointer = typename allocator_traits_helper::get_pointer<Alloc>::type;
    using const_pointer = typename allocator_traits_helper::get_const_pointer<Alloc>::type;
    using void_pointer = typename allocator_traits_helper::get_void_pointer<Alloc>::type;
    using const_void_pointer = typename allocator_traits_helper::get_const_void_pointer<Alloc>::type;
    using difference_type = typename allocator_traits_helper::get_difference_type<Alloc>::type;
    using size_type = typename allocator_traits_helper::get_size_type<Alloc>::type;
    // using propagate_on_container_copy_assignment = typename Alloc::propagate_on_container_copy_assignment;
    // using propagate_on_container_move_assignment = typename Alloc::propagate_on_container_move_assignment;
    // using propagate_on_container_swap = typename Alloc::propagate_on_container_swap;
    // using is_always_equal = typename Alloc::is_always_equal;
    // using rebind_alloc = typename Alloc::template rebind_alloc<U>;
    // using rebind_traits = allocator_traits<rebind_alloc>;

    [[nodiscard]] static constexpr pointer allocate(Alloc& a, size_type n) {
        return a.allocate(n);
    }
    [[nodiscard]] static constexpr pointer allocate(Alloc& a, size_type n, const_void_pointer hint) {
        if constexpr (requires { a.allocate(n, hint); }) {
            return a.allocate(n, hint);
        } else {
            return a.allocate(n);
        }
    }
    static constexpr void deallocate(Alloc& a, pointer p, size_type n) {
        a.deallocate(p, n);
    }
    template <typename T, typename... Args>
    static constexpr void construct(Alloc& a, T* p, Args&&... args) {
        if constexpr (requires { a.construct(p, forward<Args>(args)...); }) {
            a.construct(p, forward<Args>(args)...);
        } else {
            construct_at(p, forward<Args>(args)...);
        }
    }
    template <typename T>
    static constexpr void destroy(Alloc& a, T* p) {
        if constexpr (requires { a.destroy(p); }) {
            a.destroy(p);
        } else {
            destroy_at(p);
        }
    }
};
}
}