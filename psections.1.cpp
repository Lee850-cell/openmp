#include <iostream>
#include <omp.h>

// 模拟 X, Y, Z 轴的处理函数
void XAXIS() { std::cout << "Thread " << omp_get_thread_num() << " processing X-AXIS\n"; }
void YAXIS() { std::cout << "Thread " << omp_get_thread_num() << " processing Y-AXIS\n"; }
void ZAXIS() { std::cout << "Thread " << omp_get_thread_num() << " processing Z-AXIS\n"; }

void sect_example() {
    // 启动并行区域并划分任务块
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            XAXIS();
        }

        #pragma omp section
        {
            YAXIS();
        }

        #pragma omp section
        {
            ZAXIS();
        }
    } 
    // 所有 section 执行完后，这里有一个隐式的 Barrier（同步路障）
}

int main() {
    sect_example();
    return 0;
}