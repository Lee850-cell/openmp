#include <iostream>
#include <omp.h>
using namespace std;

class T {
public:
    static int i;
    #pragma omp threadprivate(i)  // 静态成员 → 每个线程独立副本
};

int T::i = 0;  // 类外定义静态成员

int main()
{
    cout << "=== First parallel region ===" << endl;
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        T::i += tid + 1;

        #pragma omp critical
        cout << "Thread " << tid << ": T::i = " << T::i << endl;
    }

    cout << "\n=== Second parallel region (values persist) ===" << endl;
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();

        #pragma omp critical
        cout << "Thread " << tid << ": T::i = " << T::i << endl;
    }

    return 0;
}
