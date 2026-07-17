#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

float* work;
int size;
float tol;

// threadprivate: 每个线程有自己独立的 work, size, tol
// 值在多次 parallel 之间保持
#pragma omp threadprivate(work, size, tol)

void build() {
    int i;
    work = (float*)malloc(sizeof(float) * size);
    for (i = 0; i < size; i++) work[i] = tol;
}

void copyin_example(float t, int n) {
    tol = t;
    size = n;
    // copyin: 把主线程的 tol, size 拷贝到所有线程的私有副本
    #pragma omp parallel copyin(tol, size)
    {
        build();
        #pragma omp critical
        printf("Thread %d: work=%p, size=%d, work[0]=%.1f\n",
               omp_get_thread_num(), (void*)work, size,
               size > 0 ? work[0] : -1.0f);
    }
}

int main() {
    printf("=== copyin_example(5.0, 3) ===\n");
    copyin_example(5.0f, 3);
    return 0;
}
