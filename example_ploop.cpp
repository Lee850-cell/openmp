#include <iostream>
#include <vector>
#include <omp.h>    // OpenMP 核心頭文件
#include <stdio.h>

// S-1: 定義函數
void simple(int n, float *a, float *b)
{
    int i;
    // S-5: 開啟並行循環
    #pragma omp parallel for
    for (i = 1; i < n; i++) { // S-6: i 在並行循環中默認為私有 (private)
        // S-7: 計算均值
        b[i] = (a[i] + a[i - 1]) / 2.0f;
    }
}

int main() {
    const int n = 10;
    float a[n], b[n];

    // 1. 初始化數據
    for (int i = 0; i < n; i++) {
        a[i] = (float)(i * 10);
        b[i] = 0.0f;
    }

    // 2. 設置線程數 (例如 4 執行緒)
    omp_set_num_threads(4);

    // 3. 調用並行計算函數
    simple(n, a, b);

    // 4. 驗證結果輸出
    printf("Index | Original (a) | Smoothed (b)\n");
    printf("----------------------------------\n");
    for (int i = 1; i < n; i++) {
        printf("%5d | %12.2f | %12.2f\n", i, a[i], b[i]);
    }

    return 0;
}