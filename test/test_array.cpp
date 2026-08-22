#include <array>
#include <bios/ut.hpp>

using namespace bios::ut::literal;

bios::ut::suite test_array{[](){
    "array basic element access"_test = [] {
        std::array<int, 3> arr;
        arr[0] = 10;
        arr[1] = 20;
        arr[2] = 30;
        
        bios::ut::expect(arr.size() == 3_ul);
        bios::ut::expect(arr[0] == 10_i);
        bios::ut::expect(arr[2] == 30_i);
    };

    "array empty"_test = [] {
        std::array<int, 0> empty_arr;
        bios::ut::expect(empty_arr.size() == 0_ul);
        bios::ut::expect(static_cast<int>(empty_arr.empty()) == 1_i);
        
        std::array<int, 3> arr;
        bios::ut::expect(static_cast<int>(arr.empty()) == 0_i);
    };

    "array front back"_test = [] {
        std::array<int, 3> arr;
        arr[0] = 10;
        arr[1] = 20;
        arr[2] = 30;
        
        bios::ut::expect(arr.front() == 10_i);
        bios::ut::expect(arr.back() == 30_i);
    };

    "array data"_test = [] {
        std::array<int, 3> arr;
        arr[0] = 42;
        bios::ut::expect(arr.data()[0] == 42_i);
    };

    "array fill"_test = [] {
        std::array<int, 3> arr;
        arr.fill(7);
        bios::ut::expect(arr[0] == 7_i);
        bios::ut::expect(arr[1] == 7_i);
        bios::ut::expect(arr[2] == 7_i);
    };

    "array iterators"_test = [] {
        std::array<int, 3> arr;
        arr[0] = 1; arr[1] = 2; arr[2] = 3;
        
        int sum = 0;
        for (auto it = arr.begin(); it != arr.end(); ++it) {
            sum += *it;
        }
        bios::ut::expect(sum == 6_i);
    };
}};
