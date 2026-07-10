#include <cmath>
#include <iostream>
#include <omp.h>

void reduction1(float *x, int *y, int n) {
    int i, b, c;
    float a, d;
    a = 0.0f;
    b = 0;
    c = y[0];
    d = x[0];

    #pragma omp parallel for private(i) shared(x, y, n) \
        reduction(+:a) reduction(^:b) \
        reduction(min:c) reduction(max:d)
    for (i = 0; i < n; i++) {
        a += x[i];              // 求和
        b ^= y[i];              // 按位异或
        if (c > y[i]) c = y[i]; // 找最小值
        d = fmaxf(d, x[i]);     // 找最大值
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

    reduction1(x, y, n);
    return 0;
}
