#pragma once

#include <bits/namespace.hpp>

#include <bits/size_t.hpp>

namespace OZLIB_NAMESPACE {
namespace impl {

using int_least8_t = char;
using int_least16_t = short;
using int_least32_t = int;
using int_least64_t = long long;
using uint_least8_t = unsigned char;
using uint_least16_t = unsigned short;
using uint_least32_t = unsigned int;
using uint_least64_t = unsigned long long;

static_assert(sizeof(int_least8_t) * 8 >= 8);
static_assert(sizeof(int_least16_t) * 8 >= 16);
static_assert(sizeof(int_least32_t) * 8 >= 32);
static_assert(sizeof(int_least64_t) * 8 >= 64);
static_assert(sizeof(uint_least8_t) * 8 >= 8);
static_assert(sizeof(uint_least16_t) * 8 >= 16);
static_assert(sizeof(uint_least32_t) * 8 >= 32);
static_assert(sizeof(uint_least64_t) * 8 >= 64);

}
}