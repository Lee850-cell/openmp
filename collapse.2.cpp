#include <stdio.h>
#include <omp.h>

void test() {
    int j, k, jlast = 0, klast = 0;

    #pragma omp parallel num_threads(4) // 强制 4 线程观察
    {
        // collapse(2) 把 2x3 的循环压成 6 个任务
        #pragma omp for collapse(2) lastprivate(jlast, klast)
        for (k = 1; k <= 2; k++) {
            for (j = 1; j <= 3; j++) {
                int tid = omp_get_thread_num();
                printf("Thread %d 正在处理: k=%d, j=%d\n", tid, k, j);
                jlast = j;
                klast = k;
            }
        }

        #pragma omp single
        {
            printf("\n--- 最终保留的 lastprivate 值 ---\n");
            printf("klast = %d, jlast = %d\n", klast, jlast);
        }
    }
}

int main() {
    test();
    return 0;
}