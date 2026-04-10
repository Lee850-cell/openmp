#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <vector>

void nowait_example(int n, int m, float *a, float *b, float *y, float *z)
{
    #pragma omp parallel num_threads(4) // 這裡固定 4 個線程方便觀察
    {
        int tid = omp_get_thread_num();

        // 第一個循環：計算平均值
        #pragma omp for nowait
        for (int i = 1; i < n; i++) {
            b[i] = (a[i] + a[i - 1]) / 2.0f;
            // 模擬第一個循環中，線程 0 跑得特別慢（增加負載不均）
            if (tid == 0 && i == 1) {
                for (volatile int delay = 0; delay < 10000000; delay++); 
            }
        }
        // 如果沒有 nowait，所有線程都會在這裡停下等線程 0
        printf("Thread %d 完成了第一個循環\n", tid);

        // 第二個循環：計算平方根
        #pragma omp for nowait
        for (int i = 0; i < m; i++) {
            y[i] = sqrt(z[i]);
        }
        printf("Thread %d 完成了第二個循環\n", tid);
    }
}

int main() {
    const int N = 100, M = 100;
    float a[N], b[N], y[M], z[M];

    // 初始化數據
    for (int i = 0; i < N; i++) { a[i] = i * 1.0f; z[i] = i * 1.0f; }

    printf("--- 開始執行 (使用 nowait) ---\n");
    nowait_example(N, M, a, b, y, z);
    printf("--- 執行結束 ---\n");

    return 0;
}