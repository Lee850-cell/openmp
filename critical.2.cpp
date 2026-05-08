#include <iostream>
#include <vector>
#include <omp.h>

// 模拟队列获取
int dequeue(int& head, int max_size) {
    if (head < max_size) {
        return head++;
    }
    return -1;
}

void work(int index, const std::string& label) {
    if (index != -1) {
        // 模拟实际计算
        double result = 0;
        for(int i=0; i<1000; i++) result += i; 
    }
}

void critical_hint_example(int n) {
    int x_head = 0;
    int y_head = 0;

    // 开启并行
    #pragma omp parallel num_threads(12)
    {
        int ix, iy;

        // 使用 hint 优化：告诉系统这里竞争很激烈（12个线程都在抢）
        #pragma omp critical (xaxis) hint(omp_lock_hint_contended)
        {
            ix = dequeue(x_head, n);
        }
        work(ix, "X");

        #pragma omp critical (yaxis) hint(omp_lock_hint_contended)
        {
            iy = dequeue(y_head, n);
        }
        work(iy, "Y");
    }
}

int main() {
    std::cout << "--- 开启带 Hint 优化的命名临界区 ---" << std::endl;
    critical_hint_example(1000);
    std::cout << "--- 计算完成 ---" << std::endl;
    return 0;
}