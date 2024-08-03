#include <type_traits>
#include <cstddef>
#include <array>
#include <functional>

int hoge(){
    return 0;
}

struct hoge2{
    int operator()(){
        return 0;
    }
};

struct hoge3{
    int hogehoge(){
        return 0;}
};

int main(){
    std::function<int()> f = hoge;
    auto a = f();
    return 0;
}