#include <iostream>
#include <omp.h>
using namespace std;

struct T {
    T();
    T(int);
    ~T();
    int t;
};

T::T()
{
    t = 0;
}

T::T(int v)
{
    t = v;
}

T::~T()
{
}

int f()
{
    return 42;
}

/* 先 extern 声明，让 threadprivate 在构造函数调用前生效 */
extern T t1;
extern T t2;
extern T t3;
#pragma omp threadprivate(t1)
#pragma omp threadprivate(t2)
#pragma omp threadprivate(t3)

/* 然后定义（调用构造函数） */
T t1;         // ① 默认构造
T t2(23);     // ② 带参数构造
T t3 = f();   // ③ 函数返回值初始化

int main()
{
    /* 第一次并行区域：每个线程初始化自己的副本 */
    #pragma omp parallel num_threads(3)
    {
        int tid = omp_get_thread_num();

        t1.t += tid;
        t2.t += tid;
        t3.t += tid;

        #pragma omp critical
        cout << "Thread " << tid
             << ": t1.t=" << t1.t
             << " t2.t=" << t2.t
             << " t3.t=" << t3.t << endl;
    }

    cout << "--- second parallel region ---" << endl;

    /* 第二次并行区域：每个线程的副本保留上一次的值 */
    #pragma omp parallel num_threads(3)
    {
        int tid = omp_get_thread_num();

        #pragma omp critical
        cout << "Thread " << tid
             << ": t1.t=" << t1.t
             << " t2.t=" << t2.t
             << " t3.t=" << t3.t << endl;
    }

    return 0;
}
