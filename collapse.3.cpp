#include <omp.h>
#include <stdio.h>

void work(int a, int j, int k) {
    // 模拟一些计算
}

void sub() {
    int j, k, a = 0;
    // 强制使用 2 个线程进行演示
    #pragma omp parallel num_threads(2)
    {
        // collapse(2) 压扁循环，ordered 声明该循环包含顺序区域
        #pragma omp for collapse(2) ordered private(j,k) schedule(static,3)
        for (k = 1; k <= 3; k++) {
            for (j = 1; j <= 2; j++) {
                
                // 只有这个代码块是顺序执行的
                #pragma omp ordered
                {
                    printf("Thread %d handles: k=%d, j=%d\n", omp_get_thread_num(), k, j);
                }

                // 这个部分依然是并行的
                work(a, j, k);
            }
        }
    }
}

int main() {
    sub();
    return 0;
}