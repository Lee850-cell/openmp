#include <stdio.h>
int main(void){
    int N = 100, M =10;
    int i, x;

    x=0;
    #pragma omp parallel num_threads(M) reduction(task,+:x)
    {
        x++;

        #pragma omp single
        for(i=0;i<N;i++)
            #pragma omp task in_reduction(+:x)
            x++;
        
    }
    printf("x=%d =M+N\n",x);

    x=0;
    #pragma omp parallel num_threads(M) reduction(task,+:x)
    for(i=0;i<N;i++){
        x++;

        if(i%2==0){
            #pragma omp task in_reduction(+:x)
            x--;
        }
    }
    printf("x=%d =M+N/2\n",x);

    return 0;
}