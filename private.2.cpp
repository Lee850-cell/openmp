#include <iostream>
#include <omp.h>
using namespace std;

int a;  // 全局变量

void g(int k)
{
    a = k;
    /*
     * 这里的 a 是全局变量！
     * 从并行区域内调用时，多个线程同时写全局 a —
     * 究竟是写全局 a 还是 private a？规范说：未指定！
     */
}

void f(int n)
{
    int a = 0;  // 局部变量，遮盖全局 a

    #pragma omp parallel for private(a)
    for (int i = 1; i < n; i++) {
        a = i;       // 修改的是线程私有副本
        g(a * 2);    // g() 内部写的是全局 a ！
                     // 多线程同时写 → 竞态条件
                     // g() 读到的是 a*2（私有副本的值作参数）
                     // 但 g() 内部的 a= 则是不确定的
    }
}

int main()
{
    a = 0;

    cout << "Before f(): global a = " << a << endl;
    f(20);
    cout << "After  f(): global a = " << a << endl;
    cout << "(每次运行结果可能不同 — 竞态条件！)" << endl;

    return 0;
}
