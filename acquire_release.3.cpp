#include <iostream>
#include <omp.h>

int main() {
    int x = 0;
    int y = 0;

    #pragma omp parallel num_threads(2)
    {
        int thrd = omp_get_thread_num();

        if (thrd == 0) {
            // --- 线程 0 (生产者) ---
            x = 10;

            // 显式释放屏障 (Release barrier)
            // 确保在执行 y=1 之前，x=10 已经写回内存且顺序固定
            #pragma omp flush release 

            // 这里的原子写是“松散”的（默认或显式 relaxed）
            #pragma omp atomic write
            y = 1;
        } 
        else {
            // --- 线程 1 (消费者) ---
            int tmp = 0;
            while (tmp == 0) {
                // 松散原子读
                #pragma omp atomic read
                tmp = y;
            }

            // 显式获取屏障 (Acquire barrier)
            // 看到 y=1 后，强制刷新本地视图，确保能读到 x 的最新值
            #pragma omp flush acquire

            printf("线程 1 读取到 x = %d\n", x); // 结果始终为 10
        }
    }

    return 0;
}