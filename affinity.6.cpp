#include <iostream>
#include <vector>
#include <omp.h>

/**
 * 模拟分配和初始化 B 数组的任务
 * 使用 中的 alloc_init_B 逻辑
 */
double* alloc_init_B(double* A, int N) {
    double* B = new double[N];
    for (int i = 0; i < N; ++i) {
        B[i] = A[i] * 2.0; // 模拟初始化工作
    }
    return B;
}

/**
 * 模拟在 B 数组上进行计算的任务
 * 使用 中的 compute_on_B 逻辑
 */
void compute_on_B(double* B, int N) {
    for (int i = 0; i < N; ++i) {
        B[i] += 10.0; // 模拟计算工作
    }
}

/**
 * 核心任务亲和性演示函数
 */
void task_affinity_demo(double* A, int N) {
    double* B = nullptr;

    // 开启并行区域
    #pragma omp parallel num_threads(4)
    {
        // 任务必须由单线程发起，其他线程负责执行
        #pragma omp single
        {
            // 任务 1：分配并初始化。声明对 A 的亲和性
            #pragma omp task depend(out:B) shared(B) affinity(A[0:N])
            {
                int tid = omp_get_thread_num();
                printf("任务 1 (初始化) 由线程 %d 执行\n", tid);
                B = alloc_init_B(A, N);
            }

            // 任务 2：计算。依赖于 B，且声明对 A 的亲和性
            // OpenMP 5.0 会尽量让执行任务 1 的核心来执行任务 2，因为数据 A 已经在该核心的缓存里了
            #pragma omp task depend(in:B) shared(B) affinity(A[0:N])
            {
                int tid = omp_get_thread_num();
                printf("任务 2 (计算)   由线程 %d 执行\n", tid);
                compute_on_B(B, N);
            }

            // 等待所有任务完成
            #pragma omp taskwait
            
            printf("所有任务已完成。验证 B[0]: %.1f\n", B[0]);
            delete[] B; // 清理内存
        }
    }
}

int main() {
    const int N = 1000;
    std::vector<double> A(N, 1.0);

    std::cout << "--- 开始 OpenMP 5.0 任务亲和性演示 ---" << std::endl;
    task_affinity_demo(A.data(), N);

    return 0;
}