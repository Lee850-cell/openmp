#include <iostream>
#include <omp.h>

void work(int k) {
    // 只有进入 ordered 块的代码才会按顺序执行
    #pragma omp ordered
    {
        printf("当前处理索引: %d (来自线程 %d)\n", k, omp_get_thread_num());
    }
}

void ordered_example(int lb, int ub, int stride) {
    // 必须在 for 构造中显式声明 ordered 子句
    // schedule(dynamic) 意味着线程领任务的顺序是随机的
    #pragma omp parallel for ordered schedule(dynamic)
    for (int i = lb; i < ub; i += stride) {
        // 执行一些可以并行的重型计算（这里省略）
        // ... 
        
        work(i);
    }
}

int main() {
    // 从 0 到 100，步长为 5
    ordered_example(0, 100, 5);
    return 0;
}