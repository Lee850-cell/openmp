#include <iostream>
#include <omp.h>

void atomic_wrong_pointer() {
    int x = 0;
    int *i_ptr = &x;
    float *f_ptr = (float *)&x; // 危险：强制类型转换

    std::cout << "\n--- 指针强转冲突实验 ---" << std::endl;

    #pragma omp parallel num_threads(2)
    {
        int tid = omp_get_thread_num();

        if (tid == 0) {
            // 线程 0 以 int 方式更新
            #pragma omp atomic update
            *i_ptr += 1;
        } else {
            // 线程 1 以 float 方式更新
            #pragma omp atomic update
            *f_ptr += 1.0f;
        }
    }

    // 在你的 3D 模拟里，如果发生这种更新，你的物理量会立刻变成 NaN 或巨大错误
    std::cout << "地址 " << &x << " 上的数据由于非对等原子操作已损坏。" << std::endl;
    std::cout << "读取为 int: " << x << std::endl;
    std::cout << "读取为 float: " << *(float*)&x << std::endl;
}

int main() {
    // 运行第一个实验
    // atomic_wrong_union(); 
    
    // 运行第二个实验
    atomic_wrong_pointer();
    
    return 0;
}