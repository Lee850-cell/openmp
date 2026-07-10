#include <iostream>
#include <omp.h>
using namespace std;
// 模拟 Fortran COMMON /C/ X,Y
struct CommonBlock {
    float X;
    float Y;
};

// 全局共享
CommonBlock C;

void common_good2() {
    int i;

    #pragma omp parallel
    {
        // S-6: !$OMP DO PRIVATE(/C/)
        // 整个 COMMON 块在每个线程中是私有的
        #pragma omp for private(C)
        for (i = 1; i <= 10; i++) {
            C.X = static_cast<float>(i);
            C.Y = static_cast<float>(i * 2);
            cout << "Thread " << omp_get_thread_num() << ": C.X = " << C.X << ", C.Y = " << C.Y << endl;
            // do work...
        }

        // S-11: !$OMP DO PRIVATE(X)
        // Fortran: 只有 X 是私有的，Y 仍是共享的
        // C++ 中不能写 private(C.X)，改用局部变量 local_x 模拟
        #pragma omp for
        for (i = 1; i <= 10; i++) {
            float local_x;               // 每个线程私有的 X
            local_x = static_cast<float>(i);
            // Y 是共享的，多线程写需要保护
            C.Y = static_cast<float>(i * 2);
            // do work with local_x and C.Y...
            cout << "Thread " << omp_get_thread_num() << ": local_x = " << local_x << ", C.Y = " << C.Y << endl;
        }
    }
}

int main() {
    common_good2();
    return 0;
}
