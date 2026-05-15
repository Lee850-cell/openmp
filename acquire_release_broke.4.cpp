#include <iostream>
#include <omp.h>

int main() {
    // !!! 这是一个反面教材，请勿模仿这种同步方式 !!!
    int x = 0, y = 0;

    #pragma omp parallel num_threads(2)
    {
        int thrd = omp_get_thread_num();

        if (thrd == 0) {
            // 线程 0 (生产者)
            #pragma omp critical
            { 
                x = 10; 
            } 
            // 退出 critical 时确实有 flush，但它只保证之前的写入完成了。
            
            // ！！！致命缺陷点！！！
            // 编译器或 CPU 可能会为了优化，把 y = 1 挪到 critical 之前执行！
            // 或者 y = 1 执行了，但没有 release 语义，导致 x=10 还没写回内存，y=1 就被对方看到了。
            
            #pragma omp atomic write
            y = 1;

        } else {
            // 线程 1 (消费者)
            int tmp = 0;
            while (tmp == 0) {
                // 这里虽然有 acquire，但它只能同步“对方对应的 release”之后的数据。
                #pragma omp atomic read acquire
                tmp = y;
            }

            // ！！！错误点！！！
            // 因为线程 0 在写 y=1 时没有使用 release 语义，
            // 线程 1 这里的 acquire 就像是“在错误频道调台”，接不到线程 0 的信号。
            #pragma omp critical
            { 
                printf("线程 1 读取到 x = %d\n", x); // 结果可能是 0
            }
        }
    }
    return 0;
}