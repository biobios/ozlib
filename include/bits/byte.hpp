#pragma once

#include <bits/namespace.hpp>

#include <bits/fundamental_traits.hpp>

namespace OZLIB_NAMESPACE {
namespace impl {
enum class byte : unsigned char {};

template <class IntType>
    requires is_integral_v<IntType>
constexpr byte operator<<(byte b, IntType shift) noexcept {
    return static_cast<byte>(static_cast<unsigned char>(static_cast<unsigned int>(b) << shift));
};

template <class IntType>
    requires is_integral_v<IntType>
constexpr byte& operator<<=(byte& b, IntType shift) noexcept {
    return b = b << shift;
};

template <class IntType>
    requires is_integral_v<IntType>
constexpr byte operator>>(byte b, IntType shift) noexcept {
    return static_cast<byte>(static_cast<unsigned char>(static_cast<unsigned int>(b) >> shift));
};

template <class IntType>
    requires is_integral_v<IntType>
constexpr byte& operator>>=(byte& b, IntType shift) noexcept {
    return b = b >> shift;
};

constexpr byte operator|(byte l, byte r) noexcept {
    return static_cast<byte>(static_cast<unsigned char>(static_cast<unsigned int>(l) | static_cast<unsigned int>(r)));
};

constexpr byte& operator|=(byte& l, byte r) noexcept {
    return l = l | r;
};

constexpr byte operator&(byte l, byte r) noexcept {
    return static_cast<byte>(static_cast<unsigned char>(static_cast<unsigned int>(l) & static_cast<unsigned int>(r)));
};

constexpr byte& operator&=(byte& l, byte r) noexcept {
    return l = l & r;
};

constexpr byte operator^(byte l, byte r) noexcept {
    return static_cast<byte>(static_cast<unsigned char>(static_cast<unsigned int>(l) ^ static_cast<unsigned int>(r)));
};

constexpr byte& operator^=(byte& l, byte r) noexcept {
    return l = l ^ r;
};

constexpr byte operator~(byte b) noexcept {
    return static_cast<byte>(static_cast<unsigned char>(~static_cast<unsigned int>(b)));
};
}
}