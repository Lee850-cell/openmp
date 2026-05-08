#include <iostream>
#include <vector>
#include <omp.h>

float work1(int i) { return 1.0f * i; }
float work2(int i) { return 2.0f * i; }

void atomic_example(float *x, float *y, int *index, int n) {
    // 开启并行循环
    #pragma omp parallel for shared(x, y, index, n)
    for (int i = 0; i < n; i++) {
        
        // 关键点：由于 index[i] 可能存在重复值（例如多个 i 对应同一个格点）
        // 如果不加 atomic，多个线程同时写 x[某个位置] 会导致数据丢失
        #pragma omp atomic update
        x[index[i]] += work1(i);

        // 这里不需要 atomic，因为 y[i] 的下标 i 是循环变量
        // 并行区域会自动保证每个线程处理不同的 i，互不干扰
        y[i] += work2(i);
    }
}

int main() {
    const int X_SIZE = 1000;
    const int DATA_SIZE = 10000;

    std::vector<float> x(X_SIZE, 0.0f);
    std::vector<float> y(DATA_SIZE, 0.0f);
    std::vector<int> index(DATA_SIZE);

    // 构造索引：多个位置会映射到同一个 x 的下标
    for (int i = 0; i < DATA_SIZE; i++) {
        index[i] = i % X_SIZE; 
    }

    atomic_example(x.data(), y.data(), index.data(), DATA_SIZE);

    std::cout << "计算完成，x[0] = " << x[0] << std::endl;
    return 0;
}