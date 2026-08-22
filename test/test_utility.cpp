#include <utility>
#include <type_traits>
#include <bios/ut.hpp>

using namespace bios::ut::literal;

bios::ut::suite test_utility{[](){
    "move"_test = [] {
        int x = 42;
        // move should return an rvalue reference
        bios::ut::expect(static_cast<int>(std::is_rvalue_reference_v<decltype(std::move(x))>) == 1_i);
        bios::ut::expect(std::move(x) == 42_i);
    };

    "forward"_test = [] {
        int x = 42;
        bios::ut::expect(static_cast<int>(std::is_rvalue_reference_v<decltype(std::forward<int>(x))>) == 1_i);
        bios::ut::expect(static_cast<int>(std::is_lvalue_reference_v<decltype(std::forward<int&>(x))>) == 1_i);
    };

    "monostate"_test = [] {
        std::monostate m1, m2;
        // monostate comparisons
        bios::ut::expect(static_cast<int>(m1 == m2) == 1_i);
        bios::ut::expect(static_cast<int>(m1 < m2) == 0_i);
    };

    "to_underlying"_test = [] {
        enum class Color : int { Red = 1, Green = 2 };
        bios::ut::expect(std::to_underlying(Color::Red) == 1_i);
        bios::ut::expect(static_cast<int>(std::is_same_v<decltype(std::to_underlying(Color::Red)), int>) == 1_i);
    };
}};
