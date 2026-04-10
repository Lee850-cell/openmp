#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define N 1000

int main() {
    int nteams_required = 2, max_thrds, tm_id;
    // 分別定義單精度和雙精度數組
    float  sp_x[N], sp_y[N], sp_a = 0.0001e0;
    double dp_x[N], dp_y[N], dp_a = 0.0001e0;   

    // 獲取當前環境的最大線程限制（示意用）
    max_thrds = omp_get_max_threads();

    // S-13: 創建 2 個團隊，每個團隊可以執行不同的任務
    #pragma omp teams num_teams(nteams_required) thread_limit(max_thrds) private(tm_id)
    {
        tm_id = omp_get_team_num();

        // 檢查是否成功獲得了 2 個團隊
        if (omp_get_num_teams() != 2) {
            printf("error: Insufficient teams on host, 2 required\n");
            exit(0);
        }

        // --- 團隊 0：執行單精度計算 (SAXPY) ---
        if (tm_id == 0) {
            #pragma omp parallel
            {
                #pragma omp for // 初始化數據
                for (int i = 0; i < N; i++) {
                    sp_x[i] = i * 0.0001f;
                    sp_y[i] = (float)i;
                }

                // 使用 SIMD 向量化加速，向量長度為 8
                #pragma omp for simd simdlen(8)
                for (int i = 0; i < N; i++) {
                    sp_x[i] = sp_a * sp_x[i] + sp_y[i];
                }
            }
        }

        // --- 團隊 1：執行雙精度計算 (DAXPY) ---
        if (tm_id == 1) {
            #pragma omp parallel
            {
                #pragma omp for // 初始化數據
                for (int i = 0; i < N; i++) {
                    dp_x[i] = i * 0.0001;
                    dp_y[i] = (double)i;
                }

                // 使用 SIMD 向量化加速，向量長度為 4
                #pragma omp for simd simdlen(4)
                for (int i = 0; i < N; i++) {
                    dp_x[i] = dp_a * dp_x[i] + dp_y[i];
                }
            }
        }
    } // Teams 區域結束

    // 打印結果驗證
    printf("i=%d sp|dp %f %f \n", N - 1, sp_x[N - 1], dp_x[N - 1]);
    printf("i=%d sp|dp %f %f \n", N / 2, sp_x[N / 2], dp_x[N / 2]);

    return 0;
}