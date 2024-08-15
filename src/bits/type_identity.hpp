#pragma once

namespace std {
namespace impl {

template <typename T>
struct type_identity {
    using type = T;
};

}  // namespace impl
}  // namespace std