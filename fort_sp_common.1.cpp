#include <iostream>
#include <omp.h>

// C++ 中模拟 Fortran COMMON 块 /C/ 的方式：全局变量
// 相当于原作中的 COMMON /C/ X,Y
struct CommonBlock {
    float X;
    float Y;
};

// 全局共享变量（模拟 COMMON 块）
CommonBlock C;

void common_good() {
    // S-5: !$OMP PARALLEL PRIVATE (/C/)
    // 每个线程拥有自己独立的 X, Y 副本
    #pragma omp parallel private(C)
    {
        int tid = omp_get_thread_num();
        // 每个线程给自己的私有副本赋值
        C.X = static_cast<float>(tid * 10.0f);
        C.Y = static_cast<float>(tid * 20.0f);

        #pragma omp critical
        std::cout << "PRIVATE region: Thread " << tid
                  << ", C.X = " << C.X
                  << ", C.Y = " << C.Y
                  << " (addr C: " << &C << ")" << std::endl;
    }
    // 离开并行区后，全局 C 的值未被修改（各线程修改的是私有副本）

    std::cout << "After PRIVATE region: global C.X = " << C.X
              << ", C.Y = " << C.Y << std::endl;

    // S-8: !$OMP PARALLEL SHARED (X,Y)
    // 所有线程共享同一个 X, Y
    #pragma omp parallel shared(C)
    {
        int tid = omp_get_thread_num();

        #pragma omp critical
        {
            C.X += 1.0f;
            C.Y += 2.0f;
            std::cout << "SHARED region: Thread " << tid
                      << " updated C.X = " << C.X
                      << ", C.Y = " << C.Y
                      << " (addr C: " << &C << ")" << std::endl;
        }
    }

    std::cout << "After SHARED region: global C.X = " << C.X
              << ", C.Y = " << C.Y << std::endl;
}

int main() {
    // 初始化全局 COMMON 块
    C.X = 0.0f;
    C.Y = 0.0f;

    common_good();
    return 0;
}
