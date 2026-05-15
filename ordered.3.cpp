#include <iostream>
#include <cstdio>
#include <omp.h>

void work(int i, const char* stage) {
    // 模拟某种计算任务
}

void ordered_good_fixed(int n) {
    // 必须在 for 处声明 ordered 子句
    #pragma omp parallel for ordered num_threads(4)
    for (int i = 0; i < n; i++) {
        int tid = omp_get_thread_num();

        // 修正点：循环体内只保留【唯一】一个 ordered 构造
        #pragma omp ordered
        {
            // 在内部通过逻辑分支实现不同的顺序操作
            if (i <= 10) {
                printf("[线程 %d] 迭代 %d: 执行前半段逻辑 (i <= 10)\n", tid, i);
                work(i, "Early Stage");
            }
            else {
                printf("[线程 %d] 迭代 %d: 执行后半段逻辑 (i > 10)\n", tid, i);
                work(i + 1, "Late Stage");
            }
        }
        
        // 这里的代码依然是并行的
    }
}

int main() {
    printf("--- 开始 Ordered 条件分支同步实验 ---\n");
    ordered_good_fixed(15);
    printf("--- 实验结束 ---\n");
    return 0;
}