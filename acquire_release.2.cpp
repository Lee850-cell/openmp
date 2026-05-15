#include <iostream>
#include <omp.h>

int main() {
    int x = 0;
    int y = 0;

    // 开启两个线程：生产者与消费者
    #pragma omp parallel num_threads(2)
    {
        int thrd = omp_get_thread_num();

        if (thrd == 0) {
            // --- 线程 0 (生产者) ---
            x = 10; 

            // 使用 atomic write 并加上 release 提示
            // Release 语义确保：在此操作之前的写入 (x=10) 
            // 必须对执行对应 Acquire 操作的线程可见。
            #pragma omp atomic write release
            y = 1;
        } 
        else {
            // --- 线程 1 (消费者) ---
            int tmp = 0;
            while (tmp == 0) {
                // 使用 atomic read 并加上 acquire 提示
                // Acquire 语义确保：后续读取 (x) 
                // 必须能看到对应 Release 操作之前写入的所有数据。
                #pragma omp atomic read acquire
                tmp = y;
            }

            // 结果始终为 10
            printf("线程 1 读取到 x = %d\n", x);
        }
    }

    return 0;
}