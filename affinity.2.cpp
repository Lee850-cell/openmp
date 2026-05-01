#include <iostream>
#include <omp.h>
#include <vector>

void work() {
    // 這裡通常是你的 WENO5 核心計算邏輯
    #pragma omp critical
    {
        std::cout << "线程 " << omp_get_thread_num() 
                  << " 正在运行..." << std::endl;
    }
    // 模擬一些工作
}

void foo() {
    // 設定 16 個線程並均勻散佈
    // 這樣可以確保在進行大規模 3D 數組計算時，每個線程都能獲得穩定的緩存資源
    #pragma omp parallel num_threads(16) proc_bind(spread)
    {
        work();
        
        #pragma omp master
        {
            std::cout << "Successfully launched " << omp_get_num_threads() 
                      << " threads with spread affinity." << std::endl;
        }
    }
}

int main() {
    foo();
    return 0;
}