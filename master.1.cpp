#include <iostream>
#include <vector>
#include <omp.h>

// 模擬你的平均值計算函數
float average(float a, float b, float c) {
    return (a + b + c) / 3.0f;
}

void master_example(std::vector<float>& x, std::vector<float>& xold, int n, float tol) {
    int c = 0;
    int toobig;

    #pragma omp parallel
    {
        do {
            // 並行備份舊數據
            #pragma omp for
            for (int i = 1; i < n - 1; ++i) {
                xold[i] = x[i];
            }

            // 隨便找一人初始化計數器
            #pragma omp single
            toobig = 0;

            // 並行計算新值，並統計誤差
            #pragma omp for reduction(+:toobig)
            for (int i = 1; i < n - 1; ++i) {
                float y = x[i];
                x[i] = average(xold[i - 1], x[i], xold[i + 1]);
                float error = y - x[i];
                if (error > tol || error < -tol) {
                    ++toobig;
                }
            }

            // 只有 Master 線程負責記錄和輸出
            #pragma omp master
            {
                ++c;
                std::cout << "Iteration " << c << ", toobig = " << toobig << std::endl;
            }
            
            // 注意：這裡隱含了一個 barrier，確保所有人都拿到最新的 toobig 值來判斷 while
        } while (toobig > 0);
    }
}