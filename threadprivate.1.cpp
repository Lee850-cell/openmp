#include <iostream>
#include <omp.h>
using namespace std;

int counter = 0;
#pragma omp threadprivate(counter)

int increment_counter()
{
    counter++;
    return counter;
}

int main()
{
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int val = increment_counter();
        #pragma omp critical
        cout << "Thread " << tid << ": counter = " << val << endl;
    }

    return 0;
}
