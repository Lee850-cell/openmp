#include <iostream>
#include <omp.h>

// Fortran 原意：
//   COMMON /BLOCK5/ A        → A 在 COMMON 块中
//   DIMENSION B(10)          → B 是 10 元素数组
//   EQUIVALENCE (A, B(1))    → A 和 B(1) 同地址
//   效果：A 实际上是 B 的第一个元素，且 A 后面还有 9 个元素
//
//   正常：CALL SUB1(A) → SUB1 把 A 当 X(1..10) 用
//   因为 A 通过 EQUIVALENCE 序列关联到了整个 B 数组
//
//   但 PRIVATE(/BLOCK5/) 后：
//   编译器可以切断序列关联，A 可能退化为单元素标量
//   sub1() 里写 X[0..9] 就越界了

// 模拟 SUB1(X)
void sub1(float X[]) {
    for (int i = 0; i < 10; i++) {
        X[i] = static_cast<float>(i + 1);
    }
}

// 模拟 COMMON /BLOCK5/：一个包含变量 A 的全局块
struct Block5 {
    float A;  // 正常情况 A 通过 EQUIVALENCE 关联到 B[0]
};

Block5 blk;         // COMMON /BLOCK5/
float B[10];        // DIMENSION B(10)

int main() {
    blk.A = 0.0f;

    std::cout << "=== 正常情况（无 PRIVATE）===" << std::endl;
    // EQUIVALENCE (A,B(1)) → &A == &B[0]
    // 传 A 的地址给 sub1，等价于传整个 B 数组
    sub1(&blk.A);  // 安全：A 后面实际连着 B[1..9] 的内存
    std::cout << "After sub1(A): blk.A=" << blk.A
              << " B[9]=" << B[9] << std::endl;

    std::cout << "\n=== PRIVATE 破坏序列关联 ===" << std::endl;
    std::cout << "Fortran: PRIVATE(/BLOCK5/) 后" << std::endl;
    std::cout << "  私有 A 可能退化为单个标量（序列关联被切断）" << std::endl;
    std::cout << "  sub1(A) 写 10 个元素 → 越界，未定义！" << std::endl;

    // 注意：C++ 里 private(blk) 会私有化整个 blk
    // 私有 blk.A 只是一个 float，不关联 B[1..9]
    #pragma omp parallel private(blk)
    {
        #pragma omp master
        std::cout << "Thread " << omp_get_thread_num()
                  << ": private blk.A 不再有序列关联" << std::endl;
    }

    return 0;
}
