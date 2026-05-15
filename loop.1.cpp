#include <iostream>
#include <vector>
#include <omp.h>

int main() {
    constexpr int N = 100;
    std::vector<float> x(N), y(N, 0.0f);
    float a = 2.0f;

    // 初始化數據
    for (int i = 0; i < N; ++i) {
        x[i] = static_cast<float>(i);
    }

    #pragma omp parallel
    {
        // OpenMP 5.0 的 loop 指令：讓編譯器決定最優並行策略
        #pragma omp loop
        for (int i = 0; i < N; ++i) {
            y[i] = a * x[i] + y[i];
        }
    }

    // 驗證結果
    if (y[N - 1] != static_cast<float>(N - 1) * 2.0f) {
        std::cerr << "Error: 2*(N-1) != y[N-1] = " << y[N - 1] << std::endl;
    } else {
        std::cout << "計算成功! y[N-1] = " << y[N - 1] << std::endl;
    }

    return 0;
}