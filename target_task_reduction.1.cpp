#include <stdio.h>
void device_compute(int *);
#pragma omp declare target to(device_compute)
void host_compute(int *);
int main()
{
    int sum=0;

    #pragma omp parallel master
    #pragma omp taskgroup task_reduction(+:sum)
    {
        #pragma omp target in_reduction(+:sum) nowait
            device_compute(&sum);

        #pragma omp task in_reduction(+:sum)
            host_compute(&sum);
    }
    printf("sum = %d\n", sum);
    return 0;
}
void device_compute(int *sum){ *sum = 1;}
void host_compute(int *sum){ *sum = 1;}