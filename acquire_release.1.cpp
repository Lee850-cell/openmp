#include <iostream>
#include <omp.h>

int main() {
    int x = 0, y = 0;

    // 开启两个线程：一个生产者，一个消费者
    #pragma omp parallel num_threads(2)
    {
        int thrd = omp_get_thread_num();

        if (thrd == 0) {
            // --- 线程 0 (生产者) ---
            x = 10; // 步骤 A: 更新普通变量

            // 步骤 B: 进入并退出 critical 区域
            // 退出 critical 带有 "Release" 语义：
            // 它保证之前所有的写入（包括 x=10）都从寄存器刷新到主存。
            #pragma omp critical
            { 
                y = 1; // 更新标志位
            }
        } 
        else {
            // --- 线程 1 (消费者) ---
            int tmp = 0;
            while (tmp == 0) {
                // 步骤 C: 进入并退出 critical 区域
                // 进入 critical 带有 "Acquire" 语义：
                // 它强制从主存重新读取数据，确保看到最新的 y 和之前的 x。
                #pragma omp critical
                { 
                    tmp = y; 
                }
            }

            // 步骤 D: 打印 x
            // 由于 Release-Acquire 的同步链，这里打印的 x 永远是 10
            std::cout << "线程 1 读取到 x = " << x << std::endl;
        }
    }

    return 0;
}