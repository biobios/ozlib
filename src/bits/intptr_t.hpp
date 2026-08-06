#pragma once

namespace std {
namespace impl {

using intptr_t = long;
using uintptr_t = unsigned long;

static_assert(sizeof(intptr_t) >= sizeof(void*));
static_assert(sizeof(uintptr_t) >= sizeof(void*));

}
}