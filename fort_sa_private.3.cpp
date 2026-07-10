#include <iostream>
#include <omp.h>

// 注意：这个例子用 C++ 很难精确复现 Fortran 的 EQUIVALENCE + PRIVATE 行为
//
// Fortran 原意：
//   EQUIVALENCE (X,Y)  → X 和 Y 是同一块内存
//   PRIVATE(X)         → 只有 X 被私有化，Y 仍指向原存储
//   结果：
//     PRINT *,Y        → Y 是原存储，但 PRIVATE(X) 后原存储变为未定义 ❌
//     Y = 10           → 修改的是原存储
//     PRINT *,X        → X 是私有副本，从未初始化，也是未定义 ❌
//
// C++ 做不到完全等价：C++ 没有 EQUIVALENCE，也没有办法
// "只私有化变量名 X 但让别名 Y 指向原存储"

float X;   // 全局变量
float& Y = X;  // 引用：Y 是 X 的别名（模拟 EQUIVALENCE）

// Y 是 X 的引用，它们永远指向同一个地址

int main() {
    X = 1.0f;
    std::cout << "Before parallel: X = " << X << ", Y = " << Y
              << "  (地址相同: &X=" << &X << ", &Y=" << &Y << ")"
              << std::endl;

    // PRIVATE(X)：X 变成私有，但 Y 仍引用全局 X
    // C++ 的 private(X) 让花括号内的 X 指向私有副本
    // 但 Y（引用）仍然指向全局 X
    #pragma omp parallel private(X)
    {
        int tid = omp_get_thread_num();

        #pragma omp critical
        {
            // S-6: PRINT *,Y  — Y 引用全局 X
            // Fortran: 全局 X 未定义 → Y 是垃圾值
            // C++:     全局 X 仍是 1.0 → Y = 1.0
            std::cout << "Thread " << tid << ": Y = " << Y
                      << "  (Fortran中是未定义; C++中仍是1.0)"
                      << std::endl;
        }

        Y = 10.0f;  // S-7: 修改全局 X（通过引用 Y）
                     // 所有线程同时写，有数据竞争！

        #pragma omp critical
        {
            // S-8: PRINT *,X  — X 是私有副本
            // Fortran: 私有 X 从未初始化 → 未定义
            // C++:    private(X) 不初始化 → 垃圾值
            std::cout << "Thread " << tid << ": private X = " << X
                      << "  (未初始化，Fortran和C++都是垃圾值)"
                      << std::endl;
        }
    }

    std::cout << "After parallel: X = " << X << "  (被Y=10修改过多次)"
              << std::endl;
    return 0;
}
