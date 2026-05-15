#include <iostream>
#include <omp.h>
#include <iomanip>

void atomic_wrong_union() {
    // 定义联合体：n 和 x 共享 4 字节空间
    union {
        int n;
        float x;
    } u;

    u.n = 0; // 初始状态，内存里全是 0

    std::cout << "--- Union 冲突实验 ---" << std::endl;

    #pragma omp parallel num_threads(2)
    {
        int tid = omp_get_thread_num();
        
        if (tid == 0) {
            // 线程 0 认为这是个整数，尝试加 1
            #pragma omp atomic update
            u.n++;
        } else {
            // 线程 1 认为这是个浮点数，尝试加 1.0
            // 1.0 在 IEEE 754 格式下的二进制是 0x3f800000
            #pragma omp atomic update
            u.x += 1.0f;
        }
    }

    // 此时 u.n 的结果绝不会是 1 或 2，而是一个奇怪的数字
    std::cout << "最终 u.n 的十进制结果: " << u.n << std::endl;
    std::cout << "最终 u.n 的十六进制表示: 0x" << std::hex << u.n << std::endl;
    std::cout << "这就是由于类型不兼容导致的内存数据污染！" << std::dec << std::endl;
}

int main() {
    atomic_wrong_union();
    return 0;
}