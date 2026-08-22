#pragma once

#include <bits/namespace.hpp>

namespace OZLIB_NAMESPACE {
namespace impl {

template <typename T>
struct type_identity {
    using type = T;
};

}  // namespace impl
}  // namespace OZLIB_NAMESPACE