#include <cmath>
#include <climits>
#include <iostream>
#include <omp.h>

void reduction2(float *x, int *y, int n) {
    int i, b, b_p, c, c_p;
    float a, a_p, d, d_p;

    a = 0.0f;       // 求和初始值
    b = 0;          // 异或初始值
    c = y[0];       // 取最小初始值
    d = x[0];       // 取最大初始值

    #pragma omp parallel shared(a, b, c, d, x, y, n) \
        private(a_p, b_p, c_p, d_p, i)
    {
        // 每个线程初始化自己的局部累加器
        a_p = 0.0f;          // 求和 → 0
        b_p = 0;             // 异或 → 0
        c_p = INT_MAX;       // 取最小 → 最大整数
        d_p = -HUGE_VALF;    // 取最大 → 负无穷

        // 每个线程独立计算自己的部分
        #pragma omp for
        for (i = 0; i < n; i++) {
            a_p += x[i];
            b_p ^= y[i];
            if (c_p > y[i]) c_p = y[i];
            d_p = fmaxf(d_p, x[i]);
        }

        // 各线程排队合并到共享变量
        #pragma omp critical
        {
            a += a_p;                // 累加
            b ^= b_p;                // 异或
            if (c > c_p) c = c_p;    // 取更小
            d = fmaxf(d, d_p);       // 取更大
        }
    }

    std::cout << "sum a = " << a << std::endl;
    std::cout << "xor b = " << b << std::endl;
    std::cout << "min c = " << c << std::endl;
    std::cout << "max d = " << d << std::endl;
}

int main() {
    float x[] = {1.5f, 3.0f, 0.5f, 4.0f, 2.0f};
    int   y[] = {5, 3, 7, 3, 9};
    int n = 5;

    // 对比：reduction.1 用 reduction 子句，reduction.2 手动实现
    // 结果完全一样
    std::cout << "=== reduction.2 (手动实现) ===" << std::endl;
    reduction2(x, y, n);

    return 0;
}
