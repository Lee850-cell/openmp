#include <iostream>
#include <omp.h>

/**
 * 原子读取：保证在读取 *p 的过程中，它的值不会被其他线程修改。
 * 避免读到“一半是旧值，一半是新值”的情况。
 */
int atomic_read(const int *p) {
    int value;
    #pragma omp atomic read
    value = *p;
    return value;
}

/**
 * 原子写入：保证 value 被完整地存入 *p。
 * 在写入完成前，任何其他线程尝试读取 *p 都会被阻塞或排队。
 */
void atomic_write(int *p, int value) {
    #pragma omp atomic write
    *p = value;
}

int main() {
    int shared_data = 0;

    #pragma omp parallel sections
    {
        // 线程 1：负责更新状态
        #pragma omp section
        {
            atomic_write(&shared_data, 100);
            printf("线程 %d 已写入数据\n", omp_get_thread_num());
        }

        // 线程 2：负责读取状态
        #pragma omp section
        {
            int val = 0;
            // 循环等待，直到读到非零值
            while ((val = atomic_read(&shared_data)) == 0) {
                // 等待中...
            }
            printf("线程 %d 读到了数据: %d\n", omp_get_thread_num(), val);
        }
    }

    return 0;
}