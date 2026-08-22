#pragma once

#include <bits/namespace.hpp>

#include <bits/declval.hpp>

namespace OZLIB_NAMESPACE {
namespace impl {

using ptrdiff_t = decltype(declval<char*>() - declval<char*>());

}  // namespace impl
}  // namespace OZLIB_NAMESPACE