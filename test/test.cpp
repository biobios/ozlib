#include <type_traits>
#include <cstddef>
#include <array>

int main(){
    std::array<std::size_t, 2> a = {};
    a[0] = 3;
    return 0;
}