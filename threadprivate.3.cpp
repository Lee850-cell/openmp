#include <iostream>
#include <omp.h>
using namespace std;

class T {
public:
    int val;
    T(int);
    T(const T&);
};

T::T(int v) { val = v; }
T::T(const T& t) { val = t.val; }

void g(T a, T b)
{
    a.val += b.val;
}

int x = 1;

/* 先声明（不定义），让 threadprivate 在构造函数调用前生效 */
extern T a;
extern T b;
#pragma omp threadprivate(a, b)  // 在 C++ 中必须先于任何"使用"

/* 然后才能定义（调用构造函数） */
T a(x);
const T b_aux(x);  // b_aux 捕获 x=1，const 保证不变
T b(b_aux);        // b 从 b_aux 拷贝构造，始终 val=1

void f(int n)
{
    x++;  // x 变成 2
    #pragma omp parallel for
    /*
     * a: 每个线程会用 x 的当前值重新构造
     *    但 x 是 1 还是 2？——规范说：未指定！
     *
     * b: 从 b_aux 拷贝构造
     *    b_aux 是 const，始终 val=1——确定性结果
     */
    for (int i = 0; i < n; i++) {
        g(a, b);  // a.val 不确定，b.val 总是 1
    }
}

int main()
{
    cout << "Before f(): x=" << x << ", a.val=" << a.val
         << ", b.val=" << b.val << endl;

    #pragma omp parallel
    {
        #pragma omp single
        f(2);
    }

    cout << "After  f(): x=" << x << ", a.val=" << a.val
         << ", b.val=" << b.val << endl;
    return 0;
}
