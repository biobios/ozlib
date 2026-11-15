#include <tuple>
#include <type_traits>
#include <utility>
#include <functional>
#include <bios/ut.hpp>

using namespace bios::ut::literal;

namespace {

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

struct NonDefaultConstructible {
    int val;
    explicit NonDefaultConstructible(int v) : val(v) {}
};

} // namespace

bios::ut::suite test_tuple{[]() {
    "tuple_traits"_test = [] {
        // tuple_size
        bios::ut::expect(std::tuple_size<std::tuple<>>::value == 0_ul);
        bios::ut::expect(std::tuple_size<std::tuple<int>>::value == 1_ul);
        bios::ut::expect(std::tuple_size<std::tuple<int, double, char>>::value == 3_ul);
        bios::ut::expect(std::tuple_size_v<std::tuple<int, double>> == 2_ul);
        bios::ut::expect(std::tuple_size_v<const std::tuple<int, double>> == 2_ul);

        // tuple_element
        bios::ut::expect(static_cast<int>(std::is_same_v<std::tuple_element_t<0, std::tuple<int, double>>, int>) == 1_i);
        bios::ut::expect(static_cast<int>(std::is_same_v<std::tuple_element_t<1, std::tuple<int, double>>, double>) == 1_i);
        bios::ut::expect(static_cast<int>(std::is_same_v<std::tuple_element_t<0, const std::tuple<int, double>>, const int>) == 1_i);
    };

    "default_constructor"_test = [] {
        std::tuple<> t0;
        (void)t0;

        std::tuple<int, double> t1;
        bios::ut::expect(std::get<0>(t1) == 0_i);

        Tracker::reset_counts();
        std::tuple<Tracker, Tracker> t2;
        bios::ut::expect(Tracker::default_constructs == 2_i);

        // デフォルト構築不可な型のタプルはデフォルト構築不可
        bios::ut::expect(static_cast<int>(!std::is_default_constructible<std::tuple<NonDefaultConstructible>>::value) == 1_i);
    };

    "value_constructor"_test = [] {
        std::tuple<int, double, char> t(42, 3.14, 'a');
        bios::ut::expect(std::get<0>(t) == 42_i);
        bios::ut::expect(static_cast<int>(std::get<1>(t) == 3.14) == 1_i);
        bios::ut::expect(static_cast<int>(std::get<2>(t) == 'a') == 1_i);
    };

    "converting_constructor"_test = [] {
        // 型変換を伴うコンストラクタ
        std::tuple<double, long> t1(1, 2);
        bios::ut::expect(static_cast<int>(std::get<0>(t1) == 1.0) == 1_i);
        bios::ut::expect(std::get<1>(t1) == 2_l);

        // 別のtupleからの変換
        std::tuple<int, int> src(10, 20);
        std::tuple<double, double> t2(src);
        bios::ut::expect(static_cast<int>(std::get<0>(t2) == 10.0) == 1_i);
        bios::ut::expect(static_cast<int>(std::get<1>(t2) == 20.0) == 1_i);

        std::tuple<double, double> t3(std::move(src));
        bios::ut::expect(static_cast<int>(std::get<0>(t3) == 10.0) == 1_i);
        bios::ut::expect(static_cast<int>(std::get<1>(t3) == 20.0) == 1_i);
    };

    "copy_and_move_constructor"_test = [] {
        Tracker::reset_counts();
        {
            Tracker tr(100);
            std::tuple<Tracker> t1(tr);
            bios::ut::expect(Tracker::copy_constructs >= 1_i);

            std::tuple<Tracker> t2(t1);
            bios::ut::expect(std::get<0>(t2).value == 100_i);

            std::tuple<Tracker> t3(std::move(t1));
            bios::ut::expect(std::get<0>(t3).value == 100_i);
            bios::ut::expect(Tracker::move_constructs >= 1_i);
        }
    };

    "reference_tuple"_test = [] {
        int a = 10;
        int b = 20;
        std::tuple<int&, const int&> t(a, b);

        bios::ut::expect(std::get<0>(t) == 10_i);
        bios::ut::expect(std::get<1>(t) == 20_i);

        std::get<0>(t) = 30;
        bios::ut::expect(a == 30_i);
    };

    "get_accessors"_test = [] {
        std::tuple<int, double> t(10, 20.5);

        // 非const左辺値
        int& r0 = std::get<0>(t);
        r0 = 99;
        bios::ut::expect(std::get<0>(t) == 99_i);

        // const左辺値
        const auto& ct = t;
        const int& cr0 = std::get<0>(ct);
        bios::ut::expect(cr0 == 99_i);

        // 右辺値
        std::tuple<int, double> t_move(1, 2.0);
        int&& rr = std::get<0>(std::move(t_move));
        bios::ut::expect(rr == 1_i);

        // const右辺値
        const std::tuple<int, double> ct_move(3, 4.0);
        const int&& crr = std::get<0>(std::move(ct_move));
        bios::ut::expect(crr == 3_i);
    };

    "assignment_operators"_test = [] {
        std::tuple<int, double> t1(1, 2.0);
        std::tuple<int, double> t2(3, 4.0);

        t1 = t2;
        bios::ut::expect(std::get<0>(t1) == 3_i);
        bios::ut::expect(static_cast<int>(std::get<1>(t1) == 4.0) == 1_i);

        t1 = std::tuple<int, double>(5, 6.0);
        bios::ut::expect(std::get<0>(t1) == 5_i);
        bios::ut::expect(static_cast<int>(std::get<1>(t1) == 6.0) == 1_i);

        // 変換代入
        std::tuple<short, float> t3(7, 8.0f);
        t1 = t3;
        bios::ut::expect(std::get<0>(t1) == 7_i);
        bios::ut::expect(static_cast<int>(std::get<1>(t1) == 8.0) == 1_i);
    };

    "make_tuple"_test = [] {
        int x = 10;
        auto t1 = std::make_tuple(1, 2.5, 'c');
        bios::ut::expect(static_cast<int>(std::is_same_v<decltype(t1), std::tuple<int, double, char>>) == 1_i);
        bios::ut::expect(std::get<0>(t1) == 1_i);
        bios::ut::expect(static_cast<int>(std::get<2>(t1) == 'c') == 1_i);

        // std::refのアンラップ
        auto t2 = std::make_tuple(std::ref(x));
        bios::ut::expect(static_cast<int>(std::is_same_v<decltype(t2), std::tuple<int&>>) == 1_i);
        std::get<0>(t2) = 20;
        bios::ut::expect(x == 20_i);

        const int cx = 30;
        auto t3 = std::make_tuple(std::ref(cx));
        bios::ut::expect(static_cast<int>(std::is_same_v<decltype(t3), std::tuple<const int&>>) == 1_i);
    };

    "forward_as_tuple"_test = [] {
        int a = 1;
        int b = 2;
        auto t = std::forward_as_tuple(a, std::move(b));
        bios::ut::expect(static_cast<int>(std::is_same_v<decltype(t), std::tuple<int&, int&&>>) == 1_i);
        bios::ut::expect(std::get<0>(t) == 1_i);
        bios::ut::expect(std::get<1>(t) == 2_i);
    };

    "tie_and_ignore"_test = [] {
        int a = 0;
        double b = 0.0;
        char c = ' ';

        std::tuple<int, double, char> src(10, 20.5, 'z');
        std::tie(a, b, c) = src;
        bios::ut::expect(a == 10_i);
        bios::ut::expect(static_cast<int>(b == 20.5) == 1_i);
        bios::ut::expect(static_cast<int>(c == 'z') == 1_i);

        // std::ignore
        int d = 0;
        std::tie(d, std::ignore, std::ignore) = src;
        bios::ut::expect(d == 10_i);
    };

    "tuple_cat"_test = [] {
        std::tuple<int, double> t1(1, 2.0);
        std::tuple<char, const char*> t2('x', "hello");
        auto merged = std::tuple_cat(t1, t2);

        bios::ut::expect(std::tuple_size_v<decltype(merged)> == 4_ul);
        bios::ut::expect(std::get<0>(merged) == 1_i);
        bios::ut::expect(static_cast<int>(std::get<1>(merged) == 2.0) == 1_i);
        bios::ut::expect(static_cast<int>(std::get<2>(merged) == 'x') == 1_i);

        // 空タプルとの連結
        std::tuple<> empty;
        auto merged_empty = std::tuple_cat(empty, t1);
        bios::ut::expect(std::tuple_size_v<decltype(merged_empty)> == 2_ul);
        bios::ut::expect(std::get<0>(merged_empty) == 1_i);
    };

    "swap"_test = [] {
        std::tuple<int, double> t1(1, 2.0);
        std::tuple<int, double> t2(10, 20.0);

        t1.swap(t2);
        bios::ut::expect(std::get<0>(t1) == 10_i);
        bios::ut::expect(static_cast<int>(std::get<1>(t1) == 20.0) == 1_i);
        bios::ut::expect(std::get<0>(t2) == 1_i);
        bios::ut::expect(static_cast<int>(std::get<1>(t2) == 2.0) == 1_i);

        std::swap(t1, t2);
        bios::ut::expect(std::get<0>(t1) == 1_i);
        bios::ut::expect(static_cast<int>(std::get<1>(t1) == 2.0) == 1_i);
        bios::ut::expect(std::get<0>(t2) == 10_i);
        bios::ut::expect(static_cast<int>(std::get<1>(t2) == 20.0) == 1_i);
    };
}};
