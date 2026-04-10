#include <omp.h>
#include <stdio.h>

void subdomain(float *x, int istart, int ipoints) {
    for (int i = 0; i < ipoints; i++)
        x[istart + i] = 123.456f;
}

void sub(float *x, int npoints) {
    int iam, nt, ipoints, istart;
    #pragma omp parallel default(shared) private(iam, nt, ipoints, istart)
    {
        iam = omp_get_thread_num();
        nt = omp_get_num_threads();
        ipoints = npoints / nt; 
        istart = iam * ipoints; 
        if (iam == nt - 1) 
            ipoints = npoints - istart;

        printf("線程 %d: 負責範圍 [%d, %d), 共 %d 個點\n", iam, istart, istart + ipoints, ipoints);
        subdomain(x, istart, ipoints);
    }
}

int main() {
    float array[10000];
    sub(array, 10000);
    
    // 驗證第一個和最後一個數據
    printf("驗證數據: array[0] = %f, array[9999] = %f\n", array[0], array[9999]);
    return 0;
}