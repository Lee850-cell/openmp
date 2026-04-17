#include <iostream>
#include <vector>
#include <iomanip>
#include <omp.h>

int main() {
    // 使用 constexpr 替代 #define，具備類型安全性
    constexpr int N = 100;

    // 使用 std::vector 替代 C 數組，避免棧溢出且方便管理
    std::vector<float> a(N);
    std::vector<float> b(N / 2);
    int i, j;

    // 數據初始化
    for (i = 0; i < N; i++) {
        a[i] = static_cast<float>(i) + 1.0f;
    }

    j = 0;

    // OpenMP 並行計算邏輯保持不變
    #pragma omp parallel
    {
        #pragma omp for linear(j:1)
        for (i = 0; i < N; i += 2) {
            b[j] = a[i] * 2.0f;
            j++;
        }
    }

    // 使用 std::cout 替代 printf
    // std::fixed 和 std::setprecision(1) 對應之前的 %.1f
    std::cout << j << " " 
              << std::fixed << std::setprecision(1) << b[0] << " " 
              << b[j - 1] << std::endl;

    return 0;
}