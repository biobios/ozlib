namespace __cxxabiv1 {
extern "C" {

using guard = unsigned long long;
using retType = int;
static_assert(sizeof(guard) == 8, "guard must be 8 bytes long");
static_assert(sizeof(retType) == 4, "retType must be 4 bytes long");

retType __cxa_guard_acquire(guard* guard_object);
void __cxa_guard_release(guard* guard_object);

constexpr guard on_init_flag = 0x0000000000000100;
constexpr guard init_flag = on_init_flag | 0x0000000000000001;

retType __cxa_guard_acquire(guard* guard_object) {
    // guardの1バイト目は初期化完了フラグ
    // 2バイト目以降は初期化中フラグ
    // 初期化完了フラグが立っていたら初期化済みなので何もしない
    // 初期化中フラグが立っていたら他のスレッドが初期化中なので待つ
    // 初期化中フラグが立っていなかったら初期化中フラグを立てる
    // これをアトミックに行う

    guard new_val;
    guard prev_val = *guard_object;
    prev_val &= on_init_flag;

    __asm__ volatile(
        "movq %[prev], %%rax\n"
        "xchgq %%rax, %[guard]\n"
        "movq %%rax, %[new_]\n"
        : [guard] "=m"(*guard_object), [new_] "=r"(new_val)
        : [prev] "r"(prev_val)
        : "%rax");

    if (new_val == 0) return 1;

    while (true) {
        new_val = *guard_object;
        if (new_val == init_flag) break;
    }

    return 0;
}

void __cxa_guard_release(guard* guard_object) {
    // 初期化完了フラグを立てる
    guard new_val = init_flag;

    __asm__ volatile("movq %[new_], %[guard]\n"
                     : [guard] "=m"(*guard_object)
                     : [new_] "r"(new_val));
}
}
}  // namespace __cxxabiv1