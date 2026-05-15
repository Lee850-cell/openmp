#include <iostream>
#include <cstdio>
#include <omp.h>

// 封装第一个顺序操作
void first_ordered_region(int i) {
    #pragma omp ordered
    {
        printf("[线程 %d] 迭代 %d: 成功拿到令牌，执行第一次排序操作\n", omp_get_thread_num(), i);
    }
}

// 封装第二个顺序操作
void second_ordered_region(int i) {
    #pragma omp ordered
    {
        // 这一行在运行时理论上永远不会打印，因为令牌已经传给 i+1 了
        printf("[线程 %d] 迭代 %d: 试图再次申请令牌...\n", omp_get_thread_num(), i);
    }
}

int main() {
    int n = 5;
    printf("--- 正在尝试复现死锁 (Deadlock Demo) ---\n");
    printf("程序预计会在打印出第一个迭代后停止响应...\n");

    // 必须声明 ordered 子句
    #pragma omp parallel for ordered num_threads(4)
    for (int i = 0; i < n; i++) {
        
        // 第一次调用：执行完后，令牌会自动传给迭代 i+1
        first_ordered_region(i);

        // 第二次调用：此时迭代 i 再次索要已经失去的令牌，导致死锁
        second_ordered_region(i);
    }

    printf("如果你看到了这行字，说明死锁没有发生（这不科学）。\n");
    return 0;
}