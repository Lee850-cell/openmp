#include <iostream>
#include <omp.h>

void work() {
    // 打印当前线程所在的 CPU 核心 ID (在 Linux 下常用 sched_getcpu)
    #pragma omp critical
    {
        std::cout << "线程 " << omp_get_thread_num() 
                  << " 正在运行..." << std::endl;
    }
}

int main() {
    // proc_bind(spread): 将线程均匀散布在不同的 CPU 槽位或核心上
    // 这能有效提高内存带宽，非常适合处理大型 3D 数组
    #pragma omp parallel proc_bind(master) num_threads(12)
    {
        work();
    }

    return 0;
}