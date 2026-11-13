#pragma once

#include <bits/namespace.hpp>

#include <bits/decay.hpp>
#include <bits/reference_wrapper.hpp>

namespace OZLIB_NAMESPACE {
namespace impl {
template <typename T>
struct unwrap_ref_decay : unwrap_reference<decay_t<T>> {};

template <typename T>
using unwrap_ref_decay_t = typename unwrap_ref_decay<T>::type;
}
}