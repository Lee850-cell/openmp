#include <iostream>
#include <omp.h>

void lastpriv(int n, float *a, float *b) {
    int i;

    #pragma omp parallel
    {
        #pragma omp for lastprivate(i)
        for (i = 0; i < n - 1; i++)
            a[i] = b[i] + b[i + 1];
    }
    // 循环结束后，i == n-1
    // 并行循环只处理了 a[0] ~ a[n-2]，a[n-1] 没被赋值
    // 利用 lastprivate 带出来的 i 补上最后一个元素

    a[i] = b[i];  // a[n-1] = b[n-1]
}

int main() {
    const int n = 8;
    float b[n] = {1, 2, 3, 4, 5, 6, 7, 8};
    float a[n] = {0};

    lastpriv(n, a, b);

    std::cout << "a[i] = b[i] + b[i+1]  for i=0.." << n - 2 << std::endl;
    std::cout << "a[n-1] = b[n-1]       for i=" << n - 1 << std::endl;
    std::cout << std::endl;
    std::cout << "i" << "\t" << "b[i]" << "\t" << "a[i]" << std::endl;
    for (int j = 0; j < n; j++) {
        std::cout << j << "\t" << b[j] << "\t" << a[j] << std::endl;
    }
    return 0;
}
