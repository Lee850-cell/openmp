#include <iostream>
#include <omp.h>

void work(int n) {
    // 模拟工作内容
}

// 包含显式屏障的函数
void sub3(int n) {
    work(n);
    
    // 关键点：这个屏障必须被它所绑定的并行区域内的所有线程同时到达
    #pragma omp barrier
    
    work(n);
}

// 开启了一个新的并行区域并调用 sub3
void sub2(int k) {
    #pragma omp parallel shared(k)
    {
        sub3(k);
    }
}

// 嵌套调用：parallel -> for -> sub2 -> parallel -> sub3
void sub1(int n) {
    int i;
    #pragma omp parallel private(i) shared(n)
    {
        #pragma omp for
        for (i = 0; i < n; i++) {
            sub2(i);
        }
    }
}

int main() {
    std::cout << "--- 执行 sub1(2) ---" << std::endl;
    sub1(2); // 合法：sub3 的屏障绑定在 sub2 内部的 parallel 上

    std::cout << "--- 执行 sub2(2) ---" << std::endl;
    sub2(2); // 合法：sub3 的屏障绑定在 sub2 内部的 parallel 上

    std::cout << "--- 执行 sub3(2) ---" << std::endl;
    // sub3(2); 
    // 危险！如果在这里直接调用 sub3，由于它不在任何 parallel 区域内，
    // 屏障会绑定到隐式并行区域（单线程），通常合法但无意义。
    // 但如果在某些工作共享构造（如 omp for）内部直接调用 sub3，则会出错。

    return 0;
}