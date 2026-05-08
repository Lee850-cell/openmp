#include <stdio.h>
#include <omp.h>

int main(void) {
    // 假设系统最大线程数为 8
    
    // 1. 手动调用 API 显示当前（主线程）的亲和性
    // 此时尚未进入并行区，主线程通常可以在所有核心上活动
    omp_display_affinity("nest_level= %L, thread_num= %n, affinity= %A");
    // 输出示例: team_num= 0, thread_num= 0, thread_affinity= 0,1,2,3,4,5,6,7

    printf("\n--- 进入第一个并行区域 (满负载) ---\n");

    // 2. 开启并行区，线程数设为 CPU 核心总数
    // 如果设置了环境变量 OMP_DISPLAY_AFFINITY=TRUE，系统会自动打印每个线程的绑定情况
    #pragma omp parallel num_threads(omp_get_num_procs())
    {
        if(omp_get_thread_num() == 1)
            printf("1st Parallel Region -- Affinity Reported\n");
            
        // 此时系统输出会显示每个线程被精确锁定在 1 个核心上
        // 例如: thread_num= 1, thread_affinity= 1
    }

    printf("\n--- 进入第二个并行区域 (无变化，不重复报告) ---\n");

    // 3. 再次开启相同的并行区
    #pragma omp parallel num_threads(omp_get_num_procs())
    {
        // 关键点：如果亲和性没有变化，OpenMP 为了防止刷屏，不会再次打印信息
        if(omp_get_thread_num() == 0)
            printf("Same Affinity as before -- No repeated report.\n");
    }

    printf("\n--- 进入第三个并行区域 (半负载) ---\n");

    // 4. 使用一半的线程运行
    #pragma omp parallel num_threads(omp_get_num_procs() / 2)
    {
        if(omp_get_thread_num() == 0)
            printf("Report Affinity for using 1/2 of max threads.\n");
            
            
        // 此时每个线程可能会被分配到 2 个物理核心的活动范围
        // 例如: thread_num= 0, thread_affinity= 0,1
    }

    return 0;
}