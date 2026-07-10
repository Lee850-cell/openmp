#include <iostream>
#include <omp.h>

// 模拟 Fortran COMMON /BLOCK/ X
float X;  // 全局变量

// 模拟 SUBROUTINE SUB()
void sub() {
    // Fortran: PRINT *,X
    // ⚠️ Fortran 行为：并行区 private(X) 后，COMMON 中的 X 是未定义的
    // ✅ C++ 行为：全局 X 依然保留原值，sub() 能正常访问
    std::cout << "In SUB(): X = " << X
              << "  (C++中仍是原值1; Fortran中会是垃圾值)" << std::endl;
}

int main() {
    X = 1.0f;
    std::cout << "Before parallel: X = " << X << std::endl;

    #pragma omp parallel private(X)
    {
        // X = 2.0  赋值给私有副本
        X = 2.0f;

        #pragma omp critical
        {
            std::cout << "Thread " << omp_get_thread_num()
                      << ": private X = " << X << std::endl;
            // CALL SUB()
            // Fortran: sub() 看到的是未定义的 COMMON X
            // C++:    sub() 看到的是全局 X = 1.0（原封不动）
            sub();
        }
    }

    std::cout << "After parallel: X = " << X << std::endl;
    return 0;
}
