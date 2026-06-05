#include <iostream>
#include <cassert>
#include <omp.h>
using namespace std;

void priv_example3()
{
    int i, a;

    #pragma omp parallel private(a)
    {
        a = 1;  // 外层 private a

        #pragma omp parallel for private(a)
        for (i = 0; i < 10; i++) {
            a = 2;  // 内层 private a —— 全新的副本！
        }

        /*
         * 断言：a == 1
         * 内层的 a=2 修改的是内层 private 副本，
         * 外层的 private a 不受影响！
         */
        #pragma omp critical
        cout << "Thread " << omp_get_thread_num()
             << ": a = " << a << " (should be 1)" << endl;

        assert(a == 1);
    }
}

int main()
{
    omp_set_nested(1);  // 启用嵌套并行
    priv_example3();
    cout << "All assertions passed!" << endl;
    return 0;
}
