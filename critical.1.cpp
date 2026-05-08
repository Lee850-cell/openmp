#include <iostream>
#include <vector>
#include <omp.h>
#include <unistd.h> // 用于 usleep 模拟耗时

// 模拟一个简单的任务队列索引获取
// 在实际应用中，这里可能有复杂的链表操作，必须加锁
int dequeue(std::vector<float>& queue, int& head) {
    if (head < queue.size()) {
        int val = head;
        head++; // 移动指针，模拟出队
        return val;
    }
    return -1; // 队列为空
}

// 模拟具体的计算任务
void work(int index, const std::vector<float>& data, const std::string& axis) {
    if (index != -1) {
        // 模拟耗时计算
        usleep(100000); 
        printf("线程 %d 正在处理 %s 轴索引: %d，数值: %.1f\n", 
               omp_get_thread_num(), axis.c_str(), index, data[index]);
    }
}

void critical_example(std::vector<float>& x, std::vector<float>& y) {
    int x_head = 0;
    int y_head = 0;

    // 开启并行区域
    #pragma omp parallel shared(x, y, x_head, y_head)
    {
        // 每个线程尝试从两个队列中各取一个任务处理
        int ix_next, iy_next;

        // --- 命名临界区: xaxis ---
        // 即使有 100 个线程，同时只能有一个线程操作 x_head
        #pragma omp critical (xaxis)
        {
            ix_next = dequeue(x, x_head);
        }
        // 获取索引后，在临界区外并行执行 work，不阻塞其他线程取号
        work(ix_next, x, "X");

        // --- 命名临界区: yaxis ---
        // 关键点：当线程 A 在上面的 xaxis 领号时，线程 B 可以同时在这里领 yaxis 的号
        // 这就是“命名”带来的并行优化
        #pragma omp critical (yaxis)
        {
            iy_next = dequeue(y, y_head);
        }
        work(iy_next, y, "Y");
    }
}

int main() {
    // 准备模拟数据
    std::vector<float> data_x = {1.1, 2.2, 3.3, 4.4};
    std::vector<float> data_y = {10.1, 20.2, 30.3, 40.4};

    std::cout << "--- 开始命名临界区演示 ---" << std::endl;
    critical_example(data_x, data_y);
    std::cout << "--- 任务处理完毕 ---" << std::endl;

    return 0;
}