#include <iostream>
#include <omp.h>

void work1() { /* 模拟耗时计算 */ }
void work2() { /* 模拟耗时计算 */ }

void single_example() {
    #pragma omp parallel
    {
        // 只有一个人负责打印开始信息
        #pragma omp single
        std::cout << "Beginning work1. (Thread " << omp_get_thread_num() << " is doing this)\n";

        // 所有线程都会执行 work1()，因为这里不在 single 块里
        work1();

        // 只有一个人负责打印结束信息，其他人在此等待同步
        #pragma omp single
        std::cout << "Finishing work1.\n";

        // 一个人负责打印，其他人直接冲向 work2()，不等待
        #pragma omp single nowait
        std::cout << "Finished work1 and beginning work2.\n";

        work2();
    }
}

int main() {
    single_example();
    return 0;
}