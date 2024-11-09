#pragma once

// check if RTTI is enabled
#include <bits/has_rtti.hpp>
#include <typeinfo>

namespace std {
namespace helper_typeid {

/**
 * @brief Get the type_info pointer for the given type T.
 *
 * @tparam T The type for which to get the type_info.
 * @return const type_info* Pointer to the type_info of T, or nullptr if RTTI is
 * disabled.
 */
template <typename T>
const type_info* typeid_ptr() {
#if defined(RTTI_ENABLED)
    return &typeid(T);
#else
    return nullptr;
#endif
}

}  // namespace helper_typeid
}  // namespace std