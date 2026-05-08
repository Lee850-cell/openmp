#include <iostream>
#include <omp.h>

void critical_work() {
    int i = 1;

    // 第一层：并行分工 (Sections)
    // 假设有多个任务块，这里只展示了一个 section
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            // 第二层：命名临界区
            // 保证同一时间只有一个线程能进入这个区域
            #pragma omp critical (name)
            {
                // 第三层：在临界区内部再次开启并行
                // 注意：这里的并行区域会产生一组“子线程团队”
                #pragma omp parallel
                {
                    // 在子线程团队中，确保只有一个线程执行 i++
                    #pragma omp single
                    {
                        i++;
                    }
                }
            }
        }
    }
    
    std::cout << "最终 i 的值: " << i << std::endl;
}

int main() {
    critical_work();
    return 0;
}