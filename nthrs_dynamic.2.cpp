#include <stdio.h>
#include <omp.h>

int main()
{
    // 强制关闭动态调整，确保一定要到 10 个线程
    omp_set_dynamic(1);

    // 开启 10 个线程
    #pragma omp parallel num_threads(100)
    {
        int id = omp_get_thread_num();
        int actual_threads = omp_get_num_threads();

        // 修正点 1: 变量 i 必须初始化为 0
        // 修正点 2: 循环判断逻辑
        for (int i = 0; i < 10; i++) {
            if (id == i) {
                // 修正点 3: 避免在 if 内部重复定义同名变量 id
                printf("I am thread %d, Total threads: %d\n", id, actual_threads);
                //加一个barrier
                #pragma omp barrier
                
            }
            
            // 💡 技巧：为了让打印顺序更整齐（虽然不能 100% 保证），
            // 有时会在循环里加一个 barrier，但通常 printf 内部有锁。
        }

        /* do work here */
    }

    return 0;
}