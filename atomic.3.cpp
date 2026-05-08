#include <iostream>
#include <omp.h>

// 原子读取函数（参考之前的例子）
int atomic_read(const int *p) {
    int value;
    #pragma omp atomic read
    value = *p;
    return value;
}

// 核心：获取当前值并自增
int fetch_and_add(int *p) {
    int old;
    // capture 允许在一个原子块内同时完成读取和更新
    #pragma omp atomic capture
    { 
        old = *p; 
        (*p)++; 
    }
    return old;
}

// 自定义锁结构
struct locktype {
    int ticketnumber; // 下一个要发的号
    int turn;         // 当前叫到的号
};

void work() {
    // 模拟受保护的临界区工作
    printf("线程 %d 正在执行锁定任务...\n", omp_get_thread_num());
}

void do_locked_work(struct locktype *lock) {
    // 1. 领号 (Obtain the lock)
    // 保证领号过程是原子的，不会有两个人领到同一个号
    int myturn = fetch_and_add(&lock->ticketnumber);

    // 2. 等待叫号 (Spin wait)
    // 只有当叫号机显示的数字等于我的票号时，才能进入
    while (atomic_read(&lock->turn) != myturn) {
        // 自旋等待
    }

    // 3. 内存刷新 (Flush)
    // 确保临界区外的数据对所有线程可见
    #pragma omp flush
    work();
    #pragma omp flush

    // 4. 叫下一号 (Release the lock)
    // 将 turn 加 1，允许下一个排队的线程进入
    fetch_and_add(&lock->turn);
}

int main() {
    locktype mylock = {0, 0};

    #pragma omp parallel num_threads(4)
    {
        do_locked_work(&mylock);
    }

    return 0;
}