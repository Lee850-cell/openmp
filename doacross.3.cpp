#include <omp.h>
#include <iostream>

#define N 100

void work_wrong(double p[][N][N])
{
    int i, j, k;

    #pragma omp parallel for ordered(2) private(i,j,k)// 声明一个 ordered(2) 循环，表示该循环包含一个顺序区域，且依赖关系的距离为 2
    for (i=1; i<N-1; i++)
    {
        for (j=1; j<N-1; j++)
        {
            #pragma omp ordered depend(sink: i-1,j) depend(sink: i+1,j) \
                depend(sink: i,j-1) depend(sink: i,j+1)
            for (k=1; k<N-1; k++)
            {
                double tmp1 = p[i-1][j][k] + p[i+1][j][k];
                double tmp2 = p[i][j-1][k] + p[i][j+1][k];
                double tmp3 = p[i][j][k-1] + p[i][j][k+1];
                p[i][j][k] = (tmp1 + tmp2 + tmp3) / 6.0;
            }
            /* missing #pragma omp ordered depend(source) */
        }
    }
}

int main()
{
    double p[N][N][N] = {0.0};

    work_wrong(p);

    std::cout << "p[" << N/2 << "][" << N/2 << "][" << N/2 << "] = "
              << p[N/2][N/2][N/2] << std::endl;

    return 0;
}
