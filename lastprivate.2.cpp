#include <iostream>
#include <cmath>
#include <omp.h>

float condlastprivate(float *a, int n) {
    float x = 0.0f;

    #pragma omp parallel for simd lastprivate(conditional: x)
    for (int k = 0; k < n; k++) {
        if (a[k] < 108.5 || a[k] > 208.5) {
            x = sinf(a[k]);  // 只有满足条件时才写 x
        }
    }

    return x;
}

int main() {
    const int n = 6;

    // 测试1: 部分满足条件
    float a1[n] = {45.0f, 110.0f, 150.0f, 109.0f, 250.0f, 100.0f};
    float x1 = condlastprivate(a1, n);
    std::cout << "Test1: x = " << x1 << "  (最后满足条件的是 a[4]=250, sin=?)"
              << std::endl;

    // 测试2: 全部不满足条件
    float a2[n] = {150.0f, 150.0f, 150.0f, 150.0f, 150.0f, 150.0f};
    float x2 = condlastprivate(a2, n);
    std::cout << "Test2: x = " << x2 << "  (没有满足条件的，x 保持原值 0)"
              << std::endl;

    return 0;
}
