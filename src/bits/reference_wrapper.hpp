#pragma once

#include <bits/compound_traits.hpp>
#include <bits/declval.hpp>
#include <bits/invoke.hpp>
#include <bits/invoke_traits.hpp>
#include <bits/pointer_utils.hpp>
#include <bits/template_logics.hpp>

namespace std {
namespace impl {

template <class T>
class reference_wrapper {
    // トリビアルコピー可能である
    T* _ptr;

    static constexpr T* s_fun(T& t) noexcept { return addressof(t); }

    static void s_fun(T&&) = delete;

   public:
    using type = T;

    template <
        class U,
        class = enable_if<!is_same_v<reference_wrapper, remove_cvref_t<U>>>,
        class = decltype(reference_wrapper::s_fun(declval<U>()))>
    constexpr reference_wrapper(U&& u) noexcept(
        noexcept(reference_wrapper::s_fun(declval<U>())))
        : _ptr(reference_wrapper::s_fun(forward<U>(u))) {}

    constexpr reference_wrapper(const reference_wrapper& x) noexcept = default;
    ~reference_wrapper() noexcept = default;

    constexpr reference_wrapper& operator=(
        const reference_wrapper& x) noexcept = default;
    constexpr T& get() const noexcept { return *_ptr; }

    constexpr operator T&() const noexcept { return *_ptr; }

    template <class... ArgTypes>
    constexpr invoke_result_t<T&, ArgTypes&&...> operator()(
        ArgTypes&&... args) const {
        if constexpr (is_object_v<T>)
            static_assert(sizeof(T), "type must be complete");
        return invoke(get(), forward<ArgTypes>(args)...);
    }
};

}  // namespace impl
}  // namespace std