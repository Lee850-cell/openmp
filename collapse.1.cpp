#include <stdio.h>
#include <omp.h>

// 模擬一個 3D 處理函數
void bar(float *a, int i, int j, int k) {
    // 這裡可以做你的 WENO 數值通量計算
}

// 循環範圍變量
int kl=0, ku=10, ks=1;
int jl=0, ju=10, js=1;
int il=0, iu=10, is=1;

void sub(float *a) {
    int i, j, k;

    // collapse(2) 會合併 k 和 j 循環
    #pragma omp parallel
    {
        #pragma omp for collapse(2) private(i, j, k)
        for (k = kl; k <= ku; k += ks) {
            for (j = jl; j <= ju; j += js) {
                // i 循環在線程內部順序執行
                for (i = il; i <= iu; i += is) {
                    bar(a, i, j, k);
                }
            }
        }
    }
    #pragma omp parallel
    {
    
    printf("當前運行的總線程數: %d\n", omp_get_num_threads());
    
    // ... 剩下的代碼
    }
}

int main() {
    float data[2000];
    sub(data);
    printf("3D Collapse 執行完成\n");
    return 0;
}