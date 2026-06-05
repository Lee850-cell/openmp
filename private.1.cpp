#include <iostream>
#include <cassert>
#include <omp.h>
using namespace std;

int main()
{
    int i, j;
    int *ptr_i, *ptr_j;

    i = 1;
    j = 2;

    ptr_i = &i;  // 指向主线程的 i
    ptr_j = &j;  // 指向主线程的 j

    #pragma omp parallel private(i) firstprivate(j)
    {
        i = 3;           // 修改的是线程私有副本！
        j = j + 2;       // 修改的是线程私有副本（初始值 = 2）

        /* 指针仍然指向主线程的原始变量，不受 private 影响 */
        #pragma omp critical
        cout << "Thread " << omp_get_thread_num()
             << ": *ptr_i=" << *ptr_i    // = 1 (主线程的 i 没变)
             << " *ptr_j=" << *ptr_j     // = 2 (主线程的 j 没变)
             << " | i=" << i             // = 3 (私有副本)
             << " j=" << j << endl;      // = 4 (私有副本, 2+2)

        assert(*ptr_i == 1 && *ptr_j == 2);
        /* 断言成立：指针指向的是主线程的原始变量 */
    }

    /* 并行区域结束后，主线程的 i 和 j 不变 */
    cout << "\nAfter parallel: i=" << i << " j=" << j << endl;
    assert(i == 1 && j == 2);

    return 0;
}
