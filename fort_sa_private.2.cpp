#include <iostream>
#include <omp.h>

// 模拟 Fortran COMMON /BLOCK2/
// Fortran 中 COMMON 块按名字(/BLOCK2/)关联，不按变量名
// main 中叫 X，sub 中叫 Y，但它们指向同一块内存！
struct Block2 {
    float val;  // main 叫它 X, sub 叫它 Y, 实际是同一个存储位置
};

Block2 blk;  // 全局 COMMON /BLOCK2/

// 模拟 SUBROUTINE SUB()
void sub() {
    // Fortran: COMMON /BLOCK2/ Y
    // Y 就是 blk.val（和 main 中的 X 是同一个东西）
    // 但在并行区 private(X) 后，原 COMMON 块变成未定义！

    // Fortran: PRINT *,X   — X 在 SUB 的 COMMON 里没声明，
    //                         是一个未定义的局部变量
    float local_x;  // 模拟 Fortran 中未声明的 X，值未定义
    std::cout << "In SUB(): X = " << local_x
              << "  <-- 未定义(SUB没声明COMMON X)" << std::endl;

    // Fortran: PRINT *,Y   — Y 关联 COMMON /BLOCK2/
    //                         但该块在并行区已被 PRIVATE，值未定义！
    std::cout << "In SUB(): Y = " << blk.val
              << "  <-- 未定义(PRIVATE让COMMON块失效)" << std::endl;
}

int main() {
    blk.val = 1.0f;
    std::cout << "Before parallel: blk.val = " << blk.val << std::endl;

    #pragma omp parallel private(blk)
    {
        blk.val = 2.0f;  // 改的是私有副本

        #pragma omp critical
        {
            std::cout << "Thread " << omp_get_thread_num()
                      << ": private blk.val = " << blk.val << std::endl;
            sub();  // sub() 访问全局 blk.val → Fortran 中是未定义
                    // C++ 中全局 blk.val 仍是 1.0
        }
    }

    std::cout << "After parallel: blk.val = " << blk.val << std::endl;
    return 0;
}
