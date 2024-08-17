#pragma once

#include <bits/declval.hpp>

namespace std {
namespace impl {

using ptrdiff_t = decltype(declval<char*>() - declval<char*>());

}  // namespace impl
}  // namespace std