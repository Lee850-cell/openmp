#include <cassert>
#include <omp.h>
#include <iostream>

int A[2][2] = {1, 2, 3, 4};

void f(int n, int B[][2], int C[]) {
    int D[2][2] = {1, 2, 3, 4};
    int E[n][n];  // C99 VLA，GCC/Clang 支持

    assert(n >= 2);
    E[1][1] = 4;

    #pragma omp parallel firstprivate(B, C, D, E)
    {
        // B 是 VLA 参数 → 本质是指针，firstprivate 复制指针值（指向原数据）
        assert(sizeof(B) == sizeof(int (*)[2]));

        // C 是数组参数 → 退化为指针，firstprivate 复制指针值
        assert(sizeof(C) == sizeof(int*));

        // D 是固定大小本地数组 → firstprivate 复制整个数组
        assert(sizeof(D) == 4 * sizeof(int));

        // E 是 VLA 本地数组 → firstprivate 复制整个数组
        assert(sizeof(E) == static_cast<unsigned>(n * n) * sizeof(int));

        // B 和 C 的 firstprivate 副本仍然指向原来的 A
        assert(&B[1][1] == &A[1][1]);  // B 的指针值 = 原 A 的地址
        assert(&C[3] == &A[1][1]);     // C[0]=A[0], C[3]=A[1][1]

        // D 和 E 是完整拷贝，内容一样
        assert(D[1][1] == 4);
        assert(E[1][1] == 4);

        #pragma omp critical
        std::cout << "Thread " << omp_get_thread_num()
                  << " passed" << std::endl;
    }
}

int main() {
    f(2, A, A[0]);
    std::cout << "All assertions passed." << std::endl;
    return 0;
}
