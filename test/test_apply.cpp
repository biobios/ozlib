#include <tuple>
#include <type_traits>
#include <utility>
#include <bios/ut.hpp>

using namespace bios::ut::literal;

namespace {

int add_func(int a, int b) {
    return a + b;
}

void void_func(int a, int& out) {
    out = a * 2;
}

struct Functor {
    int operator()(int a, int b) const {
        return a * b;
    }
};

struct Tracker {
    int value;
    bool moved = false;
    explicit Tracker(int v) : value(v), moved(false) {}
    Tracker(const Tracker& o) : value(o.value), moved(false) {}
    Tracker(Tracker&& o) noexcept : value(o.value), moved(false) {
        o.moved = true;
    }
};

struct MemberTest {
    int value = 42;
    int get_val() const { return value; }
    int add(int x) { return value + x; }
};

constexpr int constexpr_add(int a, int b, int c) {
    return a + b + c;
}

void noexcept_func() noexcept {}
void throwing_func() noexcept(false) {}

} // namespace

bios::ut::suite test_apply{[]() {
    "basic_apply"_test = [] {
        // ラムダ式
        auto res1 = std::apply([](int a, int b) { return a + b; }, std::make_tuple(1, 2));
        bios::ut::expect(res1 == 3_i);

        // 通常の関数ポインタ
        auto res2 = std::apply(add_func, std::make_tuple(10, 20));
        bios::ut::expect(res2 == 30_i);

        // 関数オブジェクト
        Functor f;
        auto res3 = std::apply(f, std::make_tuple(3, 4));
        bios::ut::expect(res3 == 12_i);

        // 異なる型の引数
        auto res4 = std::apply([](int a, double b, char c) {
            return static_cast<double>(a) + b + static_cast<double>(c);
        }, std::make_tuple(1, 2.5, 'a'));
        bios::ut::expect(static_cast<int>(res4 == 1 + 2.5 + 'a') == 1_i);
    };

    "void_return"_test = [] {
        int out = 0;
        std::apply(void_func, std::tuple<int, int&>(5, out));
        bios::ut::expect(out == 10_i);
    };

    "zero_arguments"_test = [] {
        auto res = std::apply([]() { return 42; }, std::make_tuple());
        bios::ut::expect(res == 42_i);
    };

    "perfect_forwarding_lvalue"_test = [] {
        int x = 10;
        auto t = std::tuple<int&>(x);
        std::apply([](int& ref) { ref = 20; }, t);
        bios::ut::expect(x == 20_i);
    };

    "perfect_forwarding_rvalue"_test = [] {
        Tracker tr(100);
        auto t = std::make_tuple(tr);
        auto res = std::apply([](Tracker&& m) {
            return m.value;
        }, std::move(t));
        bios::ut::expect(res == 100_i);
    };

    "return_reference"_test = [] {
        int x = 42;
        auto t = std::tuple<int&>(x);
        decltype(auto) ref = std::apply([](int& r) -> int& { return r; }, t);
        bios::ut::expect(static_cast<int>(&ref == &x) == 1_i);
        static_assert(std::is_same_v<decltype(std::apply([](int& r) -> int& { return r; }, t)), int&>);
    };

    "const_tuple"_test = [] {
        const auto t = std::make_tuple(10, 20);
        auto res = std::apply([](const int& a, const int& b) {
            return a + b;
        }, t);
        bios::ut::expect(res == 30_i);
    };

    "member_pointer"_test = [] {
        MemberTest obj{100};
        // メンバ関数ポインタ
        auto res1 = std::apply(&MemberTest::get_val, std::make_tuple(&obj));
        bios::ut::expect(res1 == 100_i);

        auto res2 = std::apply(&MemberTest::add, std::make_tuple(&obj, 50));
        bios::ut::expect(res2 == 150_i);

        // メンバ変数ポインタ
        auto res3 = std::apply(&MemberTest::value, std::make_tuple(&obj));
        bios::ut::expect(res3 == 100_i);
    };

    "constexpr_apply"_test = [] {
        constexpr auto t = std::make_tuple(1, 2, 3);
        constexpr int val = std::apply(constexpr_add, t);
        static_assert(val == 6);
        bios::ut::expect(val == 6_i);
    };

    "noexcept_propagation"_test = [] {
        auto empty_t = std::make_tuple();
        static_assert(noexcept(std::apply(noexcept_func, empty_t)));
        static_assert(!noexcept(std::apply(throwing_func, empty_t)));
    };
}};
