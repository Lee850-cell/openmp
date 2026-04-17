#include <iostream>
#include <vector>
#include <stdexcept> // 替代 abort()，抛出异常更优雅
#include <omp.h>

// 模拟处理 1/16 数据的函数
void do_by_16(float* x, int iam, int ipoints) {
    // 实际的计算逻辑，比如计算第 iam 块格点
}

void dynthreads(std::vector<float>& x, int npoints) {
    // 1. 关闭动态线程调整
    omp_set_dynamic(0);
    
    // 2. 强制要求 16 个线程
    const int target_threads = 16;
    omp_set_num_threads(target_threads);

    #pragma omp parallel shared(x, npoints)
    {
        // 3. 运行时检查线程数是否符合预期
        int actual_threads = omp_get_num_threads();
        if (actual_threads != target_threads) {
            // 在多线程中直接 throw 比较危险，通常建议打印错误并退出
            #pragma omp master
            std::cerr << "致命错误：无法获得 " << target_threads << " 个线程！" << std::endl;
            std::exit(EXIT_FAILURE);
        }

        // 4. 每个线程根据 ID 分派任务
        int iam = omp_get_thread_num();
        int ipoints = npoints / target_threads;
        
        // 这里的 x.data() 获取 vector 的底层指针，方便传入旧式接口
        do_by_16(x.data(), iam, ipoints);
    }
}

int main() {
    std::vector<float> data(1600, 0.0f);
    try {
        dynthreads(data, 1600);
        std::cout << "计算圆满完成！" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "发生错误: " << e.what() << std::endl;
    }
    return 0;
}