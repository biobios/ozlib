#include <optional>
#include <type_traits>
#include <utility>
#include <bios/ut.hpp>

using namespace bios::ut::literal;

namespace {

struct Point {
    int x;
    int y;

    constexpr Point(int x_, int y_) : x(x_), y(y_) {}
    constexpr bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

struct Tracker {
    static inline int default_constructs = 0;
    static inline int value_constructs = 0;
    static inline int copy_constructs = 0;
    static inline int move_constructs = 0;
    static inline int copy_assigns = 0;
    static inline int move_assigns = 0;
    static inline int destructs = 0;

    static void reset_counts() {
        default_constructs = 0;
        value_constructs = 0;
        copy_constructs = 0;
        move_constructs = 0;
        copy_assigns = 0;
        move_assigns = 0;
        destructs = 0;
    }

    int value;

    Tracker() : value(0) { ++default_constructs; }
    explicit Tracker(int v) : value(v) { ++value_constructs; }
    Tracker(const Tracker& other) : value(other.value) { ++copy_constructs; }
    Tracker(Tracker&& other) noexcept : value(other.value) {
        other.value = -1;
        ++move_constructs;
    }
    Tracker& operator=(const Tracker& other) {
        value = other.value;
        ++copy_assigns;
        return *this;
    }
    Tracker& operator=(Tracker&& other) noexcept {
        value = other.value;
        other.value = -1;
        ++move_assigns;
        return *this;
    }
    ~Tracker() { ++destructs; }

    bool operator==(const Tracker& other) const { return value == other.value; }
};

} // namespace

bios::ut::suite test_optional{[]() {
    "nullopt"_test = [] {
        std::nullopt_t null = std::nullopt;
        bios::ut::expect(static_cast<int>(null == std::nullopt) == 1_i);
    };

    "default_constructor"_test = [] {
        std::optional<int> opt;
        bios::ut::expect(static_cast<int>(opt.has_value()) == 0_i);
        bios::ut::expect(static_cast<int>(static_cast<bool>(opt)) == 0_i);

        std::optional<int> opt_null(std::nullopt);
        bios::ut::expect(static_cast<int>(opt_null.has_value()) == 0_i);
        bios::ut::expect(static_cast<int>(static_cast<bool>(opt_null)) == 0_i);
    };

    "value_constructor"_test = [] {
        std::optional<int> opt(42);
        bios::ut::expect(static_cast<int>(opt.has_value()) == 1_i);
        bios::ut::expect(*opt == 42_i);

        // Converting constructor
        std::optional<long> opt_l(10);
        bios::ut::expect(static_cast<int>(opt_l.has_value()) == 1_i);
        bios::ut::expect(*opt_l == 10_l);
    };

    "in_place_constructor"_test = [] {
        std::optional<Point> opt(std::in_place, 3, 7);
        bios::ut::expect(static_cast<int>(opt.has_value()) == 1_i);
        bios::ut::expect(opt->x == 3_i);
        bios::ut::expect(opt->y == 7_i);
    };

    "copy_move_constructor"_test = [] {
        Tracker::reset_counts();
        {
            std::optional<Tracker> o1(Tracker(10));
            bios::ut::expect(Tracker::move_constructs == 1_i);

            // Copy engaged
            std::optional<Tracker> o2(o1);
            bios::ut::expect(Tracker::copy_constructs == 1_i);
            bios::ut::expect(static_cast<int>(o2.has_value()) == 1_i);
            bios::ut::expect(o2->value == 10_i);

            // Move engaged
            std::optional<Tracker> o3(std::move(o1));
            bios::ut::expect(Tracker::move_constructs == 2_i);
            bios::ut::expect(static_cast<int>(o3.has_value()) == 1_i);
            bios::ut::expect(o3->value == 10_i);
        }

        // Disengaged copy/move
        {
            Tracker::reset_counts();
            std::optional<Tracker> empty1;
            std::optional<Tracker> empty2(empty1);
            std::optional<Tracker> empty3(std::move(empty1));
            bios::ut::expect(static_cast<int>(empty2.has_value()) == 0_i);
            bios::ut::expect(static_cast<int>(empty3.has_value()) == 0_i);
            bios::ut::expect(Tracker::copy_constructs == 0_i);
            bios::ut::expect(Tracker::move_constructs == 0_i);
        }
    };

    "converting_copy_move_constructor"_test = [] {
        std::optional<int> o1(42);
        std::optional<long> o2(o1);
        bios::ut::expect(static_cast<int>(o2.has_value()) == 1_i);
        bios::ut::expect(*o2 == 42_l);

        std::optional<long> o3(std::move(o1));
        bios::ut::expect(static_cast<int>(o3.has_value()) == 1_i);
        bios::ut::expect(*o3 == 42_l);

        std::optional<int> empty;
        std::optional<long> empty_conv(empty);
        bios::ut::expect(static_cast<int>(empty_conv.has_value()) == 0_i);

        std::optional<long> empty_conv_move(std::move(empty));
        bios::ut::expect(static_cast<int>(empty_conv_move.has_value()) == 0_i);
    };

    "deduction_guide"_test = [] {
        std::optional opt(100);
        bios::ut::expect(static_cast<int>(std::is_same_v<decltype(opt), std::optional<int>>) == 1_i);
        bios::ut::expect(*opt == 100_i);
    };

    "destructor"_test = [] {
        Tracker::reset_counts();
        {
            std::optional<Tracker> opt(Tracker(5));
            bios::ut::expect(Tracker::destructs == 1_i); // temporary destroyed
        }
        bios::ut::expect(Tracker::destructs == 2_i); // opt destroyed

        Tracker::reset_counts();
        {
            std::optional<Tracker> empty;
        }
        bios::ut::expect(Tracker::destructs == 0_i); // nothing destroyed
    };

    "assignment_nullopt"_test = [] {
        Tracker::reset_counts();
        std::optional<Tracker> opt(Tracker(5));
        int dest_before = Tracker::destructs;
        opt = std::nullopt;
        bios::ut::expect(static_cast<int>(opt.has_value()) == 0_i);
        bios::ut::expect(Tracker::destructs - dest_before == 1_i);

        // Assigning nullopt to already empty optional
        opt = std::nullopt;
        bios::ut::expect(static_cast<int>(opt.has_value()) == 0_i);
    };

    "assignment_value"_test = [] {
        Tracker::reset_counts();
        std::optional<Tracker> opt;
        // Assign to empty optional (constructs)
        opt = Tracker(10);
        bios::ut::expect(static_cast<int>(opt.has_value()) == 1_i);
        bios::ut::expect(opt->value == 10_i);

        // Assign to engaged optional (move assigns)
        opt = Tracker(20);
        bios::ut::expect(static_cast<int>(opt.has_value()) == 1_i);
        bios::ut::expect(opt->value == 20_i);
        bios::ut::expect(Tracker::move_assigns == 1_i);
    };

    "copy_move_assignment"_test = [] {
        // Both engaged
        {
            Tracker::reset_counts();
            std::optional<Tracker> o1(Tracker(1));
            std::optional<Tracker> o2(Tracker(2));
            o2 = o1;
            bios::ut::expect(Tracker::copy_assigns == 1_i);
            bios::ut::expect(o2->value == 1_i);

            o2 = std::move(o1);
            bios::ut::expect(Tracker::move_assigns == 1_i);
            bios::ut::expect(o2->value == 1_i);
        }

        // Left engaged, right empty
        {
            Tracker::reset_counts();
            std::optional<Tracker> o1(Tracker(1));
            std::optional<Tracker> empty;
            int dest_before = Tracker::destructs;
            o1 = empty;
            bios::ut::expect(static_cast<int>(o1.has_value()) == 0_i);
            bios::ut::expect(Tracker::destructs - dest_before == 1_i);
        }

        // Left empty, right engaged
        {
            Tracker::reset_counts();
            std::optional<Tracker> empty;
            std::optional<Tracker> o2(Tracker(2));
            empty = o2;
            bios::ut::expect(static_cast<int>(empty.has_value()) == 1_i);
            bios::ut::expect(Tracker::copy_constructs == 1_i);
            bios::ut::expect(empty->value == 2_i);
        }

        // Both empty
        {
            Tracker::reset_counts();
            std::optional<Tracker> e1;
            std::optional<Tracker> e2;
            e1 = e2;
            bios::ut::expect(static_cast<int>(e1.has_value()) == 0_i);
            e1 = std::move(e2);
            bios::ut::expect(static_cast<int>(e1.has_value()) == 0_i);
        }
    };

    "converting_assignment"_test = [] {
        std::optional<long> opt_l;
        std::optional<int> opt_i(42);
        opt_l = opt_i;
        bios::ut::expect(static_cast<int>(opt_l.has_value()) == 1_i);
        bios::ut::expect(*opt_l == 42_l);

        opt_l.reset();
        opt_l = std::move(opt_i);
        bios::ut::expect(static_cast<int>(opt_l.has_value()) == 1_i);
        bios::ut::expect(*opt_l == 42_l);
    };

    "emplace"_test = [] {
        Tracker::reset_counts();
        std::optional<Tracker> opt;
        Tracker& ref = opt.emplace(10);
        bios::ut::expect(static_cast<int>(opt.has_value()) == 1_i);
        bios::ut::expect(ref.value == 10_i);
        bios::ut::expect(opt->value == 10_i);
        bios::ut::expect(Tracker::value_constructs == 1_i);

        // Emplace on engaged destroys old
        int dest_before = Tracker::destructs;
        Tracker& ref2 = opt.emplace(20);
        bios::ut::expect(Tracker::destructs - dest_before == 1_i);
        bios::ut::expect(ref2.value == 20_i);
        bios::ut::expect(opt->value == 20_i);
    };

    "reset"_test = [] {
        Tracker::reset_counts();
        std::optional<Tracker> opt(Tracker(10));
        int dest_before = Tracker::destructs;
        opt.reset();
        bios::ut::expect(static_cast<int>(opt.has_value()) == 0_i);
        bios::ut::expect(Tracker::destructs - dest_before == 1_i);

        // Reset on empty is safe
        opt.reset();
        bios::ut::expect(static_cast<int>(opt.has_value()) == 0_i);
    };

    "swap"_test = [] {
        // Both engaged
        {
            std::optional<int> o1(10);
            std::optional<int> o2(20);
            o1.swap(o2);
            bios::ut::expect(*o1 == 20_i);
            bios::ut::expect(*o2 == 10_i);

            using OZLIB_NAMESPACE::swap;
            swap(o1, o2);
            bios::ut::expect(*o1 == 10_i);
            bios::ut::expect(*o2 == 20_i);
        }

        // Left engaged, right empty
        {
            std::optional<int> o1(10);
            std::optional<int> o2;
            o1.swap(o2);
            bios::ut::expect(static_cast<int>(o1.has_value()) == 0_i);
            bios::ut::expect(static_cast<int>(o2.has_value()) == 1_i);
            bios::ut::expect(*o2 == 10_i);
        }

        // Left empty, right engaged
        {
            std::optional<int> o1;
            std::optional<int> o2(20);
            o1.swap(o2);
            bios::ut::expect(static_cast<int>(o1.has_value()) == 1_i);
            bios::ut::expect(*o1 == 20_i);
            bios::ut::expect(static_cast<int>(o2.has_value()) == 0_i);
        }

        // Both empty
        {
            std::optional<int> o1;
            std::optional<int> o2;
            o1.swap(o2);
            bios::ut::expect(static_cast<int>(o1.has_value()) == 0_i);
            bios::ut::expect(static_cast<int>(o2.has_value()) == 0_i);
        }
    };

    "observers"_test = [] {
        std::optional<Point> opt(std::in_place, 10, 20);
        bios::ut::expect(opt->x == 10_i);
        bios::ut::expect(opt->y == 20_i);

        bios::ut::expect((*opt).x == 10_i);
        bios::ut::expect((*opt).y == 20_i);

        const auto& copt = opt;
        bios::ut::expect(copt->x == 10_i);
        bios::ut::expect((*copt).y == 20_i);

        bios::ut::expect(opt.value().x == 10_i);
        bios::ut::expect(copt.value().y == 20_i);

        // Move observer
        Point p = std::move(opt).value();
        bios::ut::expect(p.x == 10_i);
        bios::ut::expect(p.y == 20_i);
    };

    "value_or"_test = [] {
        std::optional<int> opt(42);
        std::optional<int> empty;

        bios::ut::expect(opt.value_or(10) == 42_i);
        bios::ut::expect(empty.value_or(10) == 10_i);

        // rvalue
        bios::ut::expect(std::optional<int>(99).value_or(0) == 99_i);
        bios::ut::expect(std::optional<int>().value_or(77) == 77_i);
    };

    "monadic_and_then"_test = [] {
        auto square_root = [](int x) -> std::optional<int> {
            if (x == 4) return std::optional<int>(2);
            if (x == 9) return std::optional<int>(3);
            return std::nullopt;
        };

        std::optional<int> opt(4);
        auto r1 = opt.and_then(square_root);
        bios::ut::expect(static_cast<int>(r1.has_value()) == 1_i);
        bios::ut::expect(*r1 == 2_i);

        std::optional<int> opt_invalid(5);
        auto r2 = opt_invalid.and_then(square_root);
        bios::ut::expect(static_cast<int>(r2.has_value()) == 0_i);

        std::optional<int> empty;
        auto r3 = empty.and_then(square_root);
        bios::ut::expect(static_cast<int>(r3.has_value()) == 0_i);
    };

    "monadic_transform"_test = [] {
        auto double_val = [](int x) { return x * 2; };

        std::optional<int> opt(21);
        auto r1 = opt.transform(double_val);
        bios::ut::expect(static_cast<int>(r1.has_value()) == 1_i);
        bios::ut::expect(*r1 == 42_i);

        std::optional<int> empty;
        auto r2 = empty.transform(double_val);
        bios::ut::expect(static_cast<int>(r2.has_value()) == 0_i);
    };

    "monadic_or_else"_test = [] {
        auto fallback = []() -> std::optional<int> {
            return std::optional<int>(999);
        };

        std::optional<int> opt(42);
        auto r1 = opt.or_else(fallback);
        bios::ut::expect(static_cast<int>(r1.has_value()) == 1_i);
        bios::ut::expect(*r1 == 42_i);

        std::optional<int> empty;
        auto r2 = empty.or_else(fallback);
        bios::ut::expect(static_cast<int>(r2.has_value()) == 1_i);
        bios::ut::expect(*r2 == 999_i);
    };

    "make_optional"_test = [] {
        auto opt1 = std::make_optional(123);
        bios::ut::expect(static_cast<int>(std::is_same_v<decltype(opt1), std::optional<int>>) == 1_i);
        bios::ut::expect(*opt1 == 123_i);

        auto opt2 = std::make_optional<Point>(10, 20);
        bios::ut::expect(opt2->x == 10_i);
        bios::ut::expect(opt2->y == 20_i);
    };

    "comparisons"_test = [] {
        std::optional<int> o1(10);
        std::optional<int> o2(10);
        std::optional<int> o3(20);
        std::optional<int> empty1;
        std::optional<int> empty2;

        // optional == optional
        bios::ut::expect(static_cast<int>(o1 == o2) == 1_i);
        bios::ut::expect(static_cast<int>(o1 != o3) == 1_i);
        bios::ut::expect(static_cast<int>(o1 == empty1) == 0_i);
        bios::ut::expect(static_cast<int>(o1 != empty1) == 1_i);
        bios::ut::expect(static_cast<int>(empty1 == empty2) == 1_i);
        bios::ut::expect(static_cast<int>(empty1 != empty2) == 0_i);

        // optional == nullopt
        bios::ut::expect(static_cast<int>(empty1 == std::nullopt) == 1_i);
        bios::ut::expect(static_cast<int>(empty1 != std::nullopt) == 0_i);
        bios::ut::expect(static_cast<int>(o1 == std::nullopt) == 0_i);
        bios::ut::expect(static_cast<int>(o1 != std::nullopt) == 1_i);

        // optional == value
        bios::ut::expect(static_cast<int>(o1 == 10) == 1_i);
        bios::ut::expect(static_cast<int>(o1 != 10) == 0_i);
        bios::ut::expect(static_cast<int>(o1 == 20) == 0_i);
        bios::ut::expect(static_cast<int>(o1 != 20) == 1_i);
        bios::ut::expect(static_cast<int>(10 == o1) == 1_i);
        bios::ut::expect(static_cast<int>(20 == o1) == 0_i);

        bios::ut::expect(static_cast<int>(empty1 == 10) == 0_i);
        bios::ut::expect(static_cast<int>(empty1 != 10) == 1_i);
        bios::ut::expect(static_cast<int>(10 == empty1) == 0_i);
        bios::ut::expect(static_cast<int>(10 != empty1) == 1_i);
    };
}};
