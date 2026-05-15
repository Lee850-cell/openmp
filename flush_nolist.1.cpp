#include <iostream>
#include <omp.h>

int x, *p = &x; // 全局变量，所有函数都可见

void f1(int *q) {
    *q = 1;
    // 显式 Flush
    #pragma omp flush
    /* * 此时被刷新的是：x, p (全局), 以及 *q (通过指针访问的 j)。
     * q 本身（指针变量）是函数参数，属于线程私有，不被刷新。
     */
}

void f2(int *q) {
    // 隐式 Flush：barrier 指令自带一次全范围 flush
    #pragma omp barrier
    *q = 2;
    #pragma omp barrier
    /* * 同样的规则：只有在当前作用域内“可访问”的共享变量会被刷新。
     */
}

int g(int n) {
    int i = 1, j, sum = 0;
    *p = 1;

    // 开启 10 个线程进行并行规约
    #pragma omp parallel reduction(+: sum) num_threads(10) firstprivate(i, n)
    {
        // 1. 调用 f1
        f1(&j); 
        /* * 在 f1 内部执行 flush 时：
         * - j 会被刷新，因为它的地址传给了 f1。
         * - i, n, sum 不会被刷新！因为 f1 根本看不见它们。
         */
        sum += j;

        // 2. 调用 f2
        f2(&j);
        /* * f2 里的 barrier 也会触发 flush，但范围仅限于 f2 能看见的变量。
         */
        sum += i + j + *p + n;
    }
    return sum;
}

int main() {
    int result = g(7);
    std::cout << "计算结果: " << result << std::endl;
    return 0;
}