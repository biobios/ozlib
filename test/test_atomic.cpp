#include <atomic>
#include <bios/ut.hpp>

using namespace bios::ut::literal;

bios::ut::suite test_atomic{[](){
    "atomic_flag"_test = [] {
        std::atomic_flag flag;
        bios::ut::expect(static_cast<int>(flag.test()) == 0_i);
        bios::ut::expect(static_cast<int>(flag.test_and_set()) == 0_i);
        bios::ut::expect(static_cast<int>(flag.test()) == 1_i);
        flag.clear();
        bios::ut::expect(static_cast<int>(flag.test()) == 0_i);
    };

    "atomic_integral"_test = [] {
        std::atomic<int> a(5);
        bios::ut::expect(a.load() == 5_i);
        
        a.store(10);
        bios::ut::expect(a.load() == 10_i);

        int old = a.exchange(15);
        bios::ut::expect(old == 10_i);
        bios::ut::expect(a.load() == 15_i);

        int expected = 15;
        bool success = a.compare_exchange_strong(expected, 20);
        bios::ut::expect(static_cast<int>(success) == 1_i);
        bios::ut::expect(a.load() == 20_i);
        bios::ut::expect(expected == 15_i);

        success = a.compare_exchange_strong(expected, 25);
        bios::ut::expect(static_cast<int>(success) == 0_i);
        bios::ut::expect(a.load() == 20_i);
        bios::ut::expect(expected == 20_i);

        bios::ut::expect(a.fetch_add(5) == 20_i);
        bios::ut::expect(a.load() == 25_i);

        bios::ut::expect(a.fetch_sub(3) == 25_i);
        bios::ut::expect(a.load() == 22_i);

        bios::ut::expect(a.fetch_and(18) == 22_i); // 22 & 18 = 10110 & 10010 = 10010 = 18
        bios::ut::expect(a.load() == 18_i);


    };

    "atomic_integral_ops"_test = [] {
        std::atomic<int> a(0);
        bios::ut::expect(a++ == 0_i);
        bios::ut::expect(a.load() == 1_i);
        bios::ut::expect(++a == 2_i);
        bios::ut::expect(a.load() == 2_i);
        bios::ut::expect(a-- == 2_i);
        bios::ut::expect(a.load() == 1_i);
        bios::ut::expect(--a == 0_i);
        bios::ut::expect(a.load() == 0_i);

        bios::ut::expect((a += 5) == 5_i);
        bios::ut::expect(a.load() == 5_i);
        bios::ut::expect((a -= 2) == 3_i);
        bios::ut::expect(a.load() == 3_i);
    };

    "atomic_pointer"_test = [] {
        int arr[5] = {1, 2, 3, 4, 5};
        std::atomic<int*> p(arr);
        bios::ut::expect(static_cast<int>(p.load() == arr) == 1_i);
        
        int* old = p.fetch_add(2);
        bios::ut::expect(static_cast<int>(old == arr) == 1_i);
        bios::ut::expect(static_cast<int>(p.load() == &arr[2]) == 1_i);

        old = p.fetch_sub(1);
        bios::ut::expect(static_cast<int>(old == &arr[2]) == 1_i);
        bios::ut::expect(static_cast<int>(p.load() == &arr[1]) == 1_i);

        bios::ut::expect(static_cast<int>(++p == &arr[2]) == 1_i);
        bios::ut::expect(static_cast<int>(p++ == &arr[2]) == 1_i);
        bios::ut::expect(static_cast<int>(p.load() == &arr[3]) == 1_i);
    };

    "atomic_generic"_test = [] {
        struct Point { int x, y; };
        std::atomic<Point> p({1, 2});
        
        Point val = p.load();
        bios::ut::expect(val.x == 1_i);
        bios::ut::expect(val.y == 2_i);

        p.store({3, 4});
        val = p.load();
        bios::ut::expect(val.x == 3_i);
        bios::ut::expect(val.y == 4_i);
    };
}};
