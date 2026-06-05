#include <iostream>
#include <omp.h>
using namespace std;

int increment_counter_2()
{
    static int counter = 0;
    #pragma omp threadprivate(counter)
    counter++;
    return counter;
}

int main()
{
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int val = increment_counter_2();
        #pragma omp critical
        cout << "Thread " << tid << ": counter = " << val << endl;
    }

    return 0;
}
