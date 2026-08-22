#include <type_traits>
#include <bios/ut.hpp>

using namespace bios::ut::literal;

bios::ut::suite test_type_traits{[](){
    "is_same"_test = [] {
        bios::ut::expect(static_cast<int>(std::is_same_v<int, int>) == 1_i);
        bios::ut::expect(static_cast<int>(std::is_same_v<int, const int>) == 0_i);
        bios::ut::expect(static_cast<int>(std::is_same_v<int, double>) == 0_i);
    };

    "remove_reference"_test = [] {
        bios::ut::expect(static_cast<int>(std::is_same_v<std::remove_reference_t<int>, int>) == 1_i);
        bios::ut::expect(static_cast<int>(std::is_same_v<std::remove_reference_t<int&>, int>) == 1_i);
        bios::ut::expect(static_cast<int>(std::is_same_v<std::remove_reference_t<int&&>, int>) == 1_i);
    };

    "remove_cv"_test = [] {
        bios::ut::expect(static_cast<int>(std::is_same_v<std::remove_cv_t<const int>, int>) == 1_i);
        bios::ut::expect(static_cast<int>(std::is_same_v<std::remove_cv_t<volatile int>, int>) == 1_i);
        bios::ut::expect(static_cast<int>(std::is_same_v<std::remove_cv_t<const volatile int>, int>) == 1_i);
    };

    "is_integral"_test = [] {
        bios::ut::expect(static_cast<int>(std::is_integral_v<int>) == 1_i);
        bios::ut::expect(static_cast<int>(std::is_integral_v<bool>) == 1_i);
        bios::ut::expect(static_cast<int>(std::is_integral_v<double>) == 0_i);
        bios::ut::expect(static_cast<int>(std::is_integral_v<void>) == 0_i);
    };

    "decay"_test = [] {
        bios::ut::expect(static_cast<int>(std::is_same_v<std::decay_t<int&>, int>) == 1_i);
        bios::ut::expect(static_cast<int>(std::is_same_v<std::decay_t<const int&>, int>) == 1_i);
        bios::ut::expect(static_cast<int>(std::is_same_v<std::decay_t<int[3]>, int*>) == 1_i);
    };
    
    "enable_if"_test = [] {
        bios::ut::expect(static_cast<int>(std::is_same_v<std::enable_if_t<true, int>, int>) == 1_i);
        // enable_if_t<false> is a substitution failure, so we can't test it directly here without SFINAE.
    };
}};
