#pragma once

namespace std {
namespace impl {

namespace exposition_only {
enum class ord {
    equal = 0,
    equivalent = equal,
    less = -1,
    greater = 1,
};

enum class ncmp {
    unordered = -127,
};
}  // namespace exposition_only

namespace helper_comparison_categories {

struct unspecified {
    constexpr unspecified(unspecified*) noexcept {}
};

}  // namespace helper_comparison_categories

class partial_ordering {
    using unspecified = helper_comparison_categories::unspecified;

    int _value;
    bool _is_ordered;

    constexpr explicit partial_ordering(exposition_only::ord v) noexcept
        : _value(static_cast<int>(v)), _is_ordered(true) {}
    constexpr explicit partial_ordering(exposition_only::ncmp v) noexcept
        : _value(static_cast<int>(v)), _is_ordered(false) {}

   public:
    static const partial_ordering less;
    static const partial_ordering equivalent;
    static const partial_ordering greater;
    static const partial_ordering unordered;

    friend constexpr bool operator==(partial_ordering v, unspecified) noexcept {
        return v._is_ordered && v._value == 0;
    }
    friend constexpr bool operator==(partial_ordering v,
                                     partial_ordering w) noexcept = default;
    friend constexpr bool operator<(partial_ordering v, unspecified) noexcept {
        return v._is_ordered && v._value < 0;
    }
    friend constexpr bool operator>(partial_ordering v, unspecified) noexcept {
        return v._is_ordered && v._value > 0;
    }
    friend constexpr bool operator<=(partial_ordering v, unspecified) noexcept {
        return v._is_ordered && v._value <= 0;
    }
    friend constexpr bool operator>=(partial_ordering v, unspecified) noexcept {
        return v._is_ordered && v._value >= 0;
    }
    friend constexpr bool operator<(unspecified, partial_ordering v) noexcept {
        return v._is_ordered && 0 < v._value;
    }
    friend constexpr bool operator>(unspecified, partial_ordering v) noexcept {
        return v._is_ordered && 0 > v._value;
    }
    friend constexpr bool operator<=(unspecified, partial_ordering v) noexcept {
        return v._is_ordered && 0 <= v._value;
    }
    friend constexpr bool operator>=(unspecified, partial_ordering v) noexcept {
        return v._is_ordered && 0 >= v._value;
    }
    friend constexpr partial_ordering operator<=>(partial_ordering v,
                                                  unspecified) noexcept {
        return v;
    }
    friend constexpr partial_ordering operator<=>(unspecified,
                                                  partial_ordering v) noexcept {
        return v < 0   ? partial_ordering::greater
               : v > 0 ? partial_ordering::less
                       : v;
    }
};

inline constexpr partial_ordering partial_ordering::less =
    partial_ordering(exposition_only::ord::less);
inline constexpr partial_ordering partial_ordering::equivalent =
    partial_ordering(exposition_only::ord::equivalent);
inline constexpr partial_ordering partial_ordering::greater =
    partial_ordering(exposition_only::ord::greater);
inline constexpr partial_ordering partial_ordering::unordered =
    partial_ordering(exposition_only::ncmp::unordered);

class weak_ordering {
    using unspecified = helper_comparison_categories::unspecified;

    int _value;
    constexpr explicit weak_ordering(exposition_only::ord v) noexcept
        : _value(static_cast<int>(v)) {}

   public:
    static const weak_ordering less;
    static const weak_ordering equivalent;
    static const weak_ordering greater;

    constexpr operator partial_ordering() const noexcept {
        return _value == 0  ? partial_ordering::equivalent
               : _value < 0 ? partial_ordering::less
                            : partial_ordering::greater;
    }

    friend constexpr bool operator==(weak_ordering v, unspecified) noexcept {
        return v._value == 0;
    }
    friend constexpr bool operator==(weak_ordering v,
                                     weak_ordering w) noexcept = default;
    friend constexpr bool operator<(weak_ordering v, unspecified) noexcept {
        return v._value < 0;
    }
    friend constexpr bool operator>(weak_ordering v, unspecified) noexcept {
        return v._value > 0;
    }
    friend constexpr bool operator<=(weak_ordering v, unspecified) noexcept {
        return v._value <= 0;
    }
    friend constexpr bool operator>=(weak_ordering v, unspecified) noexcept {
        return v._value >= 0;
    }
    friend constexpr bool operator<(unspecified, weak_ordering v) noexcept {
        return 0 < v._value;
    }
    friend constexpr bool operator>(unspecified, weak_ordering v) noexcept {
        return 0 > v._value;
    }
    friend constexpr bool operator<=(unspecified, weak_ordering v) noexcept {
        return 0 <= v._value;
    }
    friend constexpr bool operator>=(unspecified, weak_ordering v) noexcept {
        return 0 >= v._value;
    }
    friend constexpr weak_ordering operator<=>(weak_ordering v,
                                               unspecified) noexcept {
        return v;
    }
    friend constexpr weak_ordering operator<=>(unspecified,
                                               weak_ordering v) noexcept {
        return v < 0 ? weak_ordering::greater : v > 0 ? weak_ordering::less : v;
    }
};

inline constexpr weak_ordering weak_ordering::less =
    weak_ordering(exposition_only::ord::less);
inline constexpr weak_ordering weak_ordering::equivalent =
    weak_ordering(exposition_only::ord::equivalent);
inline constexpr weak_ordering weak_ordering::greater =
    weak_ordering(exposition_only::ord::greater);

class strong_ordering {
    using unspecified = helper_comparison_categories::unspecified;

    int _value;
    constexpr explicit strong_ordering(exposition_only::ord v) noexcept
        : _value(static_cast<int>(v)) {}

   public:
    static const strong_ordering less;
    static const strong_ordering equal;
    static const strong_ordering equivalent;
    static const strong_ordering greater;

    constexpr operator weak_ordering() const noexcept {
        return _value == 0  ? weak_ordering::equivalent
               : _value < 0 ? weak_ordering::less
                            : weak_ordering::greater;
    }
    constexpr operator partial_ordering() const noexcept {
        return _value == 0  ? partial_ordering::equivalent
               : _value < 0 ? partial_ordering::less
                            : partial_ordering::greater;
    }

    friend constexpr bool operator==(strong_ordering v, unspecified) noexcept {
        return v._value == 0;
    }
    friend constexpr bool operator==(strong_ordering v,
                                     strong_ordering w) noexcept = default;
    friend constexpr bool operator<(strong_ordering v, unspecified) noexcept {
        return v._value < 0;
    }
    friend constexpr bool operator>(strong_ordering v, unspecified) noexcept {
        return v._value > 0;
    }
    friend constexpr bool operator<=(strong_ordering v, unspecified) noexcept {
        return v._value <= 0;
    }
    friend constexpr bool operator>=(strong_ordering v, unspecified) noexcept {
        return v._value >= 0;
    }
    friend constexpr bool operator<(unspecified, strong_ordering v) noexcept {
        return 0 < v._value;
    }
    friend constexpr bool operator>(unspecified, strong_ordering v) noexcept {
        return 0 > v._value;
    }
    friend constexpr bool operator<=(unspecified, strong_ordering v) noexcept {
        return 0 <= v._value;
    }
    friend constexpr bool operator>=(unspecified, strong_ordering v) noexcept {
        return 0 >= v._value;
    }
    friend constexpr strong_ordering operator<=>(strong_ordering v,
                                                 unspecified) noexcept {
        return v;
    }
    friend constexpr strong_ordering operator<=>(unspecified,
                                                 strong_ordering v) noexcept {
        return v < 0   ? strong_ordering::greater
               : v > 0 ? strong_ordering::less
                       : v;
    }
};

inline constexpr strong_ordering strong_ordering::less =
    strong_ordering(exposition_only::ord::less);
inline constexpr strong_ordering strong_ordering::equal =
    strong_ordering(exposition_only::ord::equal);
inline constexpr strong_ordering strong_ordering::equivalent =
    strong_ordering(exposition_only::ord::equivalent);
}  // namespace impl
}  // namespace std
