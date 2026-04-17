#include <iostream>
#include <omp.h>

int main() {
    constexpr int NT = 4;
    int section_count = 0;

    // 禁用动态调整线程数，确保测试环境稳定
    omp_set_dynamic(0);
    omp_set_num_threads(NT);

    #pragma omp parallel
    {
        // firstprivate 确保每个任务开始时都能继承 section_count 的初始值 0
        #pragma omp sections firstprivate(section_count)
        {
            #pragma omp section
            {
                section_count++;
                std::cout << "Section A: section_count = " << section_count << std::endl;
                std::cout << "Thread " << omp_get_thread_num() << " is executing section A" <<std::endl; 
            }

            #pragma omp section
            {
                section_count++;
                std::cout << "Section B: section_count = " << section_count << std::endl;
                std::cout << "Thread " << omp_get_thread_num() << " is executing section B" <<std::endl; 
            }
        }
    }

    return 0;
}