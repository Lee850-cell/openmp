#include <stdio.h>
#include <omp.h>

int main (void)
{
    // S-5: 启用嵌套并行（在现代 OpenMP 中建议使用 omp_set_max_active_levels）
    omp_set_nested(1);
    
    // S-6: 关闭动态线程调整，确保系统严格遵守我们设置的线程数
    omp_set_dynamic(0);

    // 第一部分：嵌套开启
    #pragma omp parallel
    {
        #pragma omp parallel
        {
            #pragma omp single
            {
                // 如果设置 OMP_NUM_THREADS=2,3 (外层2, 内层3)
                // 这里会打印 "Inner: num_thds=3"，总共打印 2 次（因为外层有2个线程）
                printf ("Inner: num_thds=%d\n", omp_get_num_threads());
            }
        }
        
        #pragma omp barrier // 等待内层嵌套全部完成

        // S-26: 在外层并行块内，中途关闭嵌套支持
        omp_set_nested(0);

        // 第二部分：嵌套关闭后
        #pragma omp parallel
        {
            #pragma omp single
            {
                // 即使设置了内层为 3，但因为嵌套被禁，内层只能拿到 1 个线程
                printf ("Inner: num_thds=%d\n", omp_get_num_threads());
            }
        }

        #pragma omp barrier

        // 第三部分：查看当前层（外层）
        #pragma omp single
        {
            // 打印外层线程数（应为 2）
            printf ("Outer: num_thds=%d\n", omp_get_num_threads());
        }
    }
    return 0;
}