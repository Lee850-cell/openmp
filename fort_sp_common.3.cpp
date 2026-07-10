#include <iostream>
#include <omp.h>

// 模拟 Fortran COMMON /C/ X,Y
struct CommonBlock {
    float X;
    float Y;
};

CommonBlock C;

void common_good3() {
    // S-4: !$OMP PARALLEL PRIVATE (/C/)
    // 整个 COMMON 块在每个线程中是私有的
    #pragma omp parallel private(C)
    {
        int tid = omp_get_thread_num();
        C.X = static_cast<float>(tid);
        C.Y = static_cast<float>(tid * 10.0f);

        #pragma omp critical
        std::cout << "PRIVATE: Thread " << tid
                  << ", X=" << C.X << ", Y=" << C.Y
                  << " (addr &C=" << &C << ")" << std::endl;
        // do work here
    }

    std::cout << "After PRIVATE region: global C.X="
              << C.X << ", C.Y=" << C.Y << std::endl;

    // S-7: !$OMP PARALLEL SHARED (/C/)
    // 整个 COMMON 块在所有线程间共享
    #pragma omp parallel shared(C)
    {
        int tid = omp_get_thread_num();

        #pragma omp critical
        {
            C.X += 1.0f;
            C.Y += 10.0f;
            std::cout << "SHARED: Thread " << tid
                      << " updated X=" << C.X << ", Y=" << C.Y
                      << " (addr &C=" << &C << ")" << std::endl;
        }
        // do work here
    }

    std::cout << "After SHARED region: global C.X="
              << C.X << ", C.Y=" << C.Y << std::endl;
}

int main() {
    C.X = 0.0f;
    C.Y = 0.0f;
    common_good3();
    return 0;
}
