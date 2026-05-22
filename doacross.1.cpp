 #include <omp.h>
 #include <iostream>

 float foo(int i);
 float bar(float a, float b);
 float baz(float b);

 void work(int N, float *A, float *B, float *C){
    int i;
    #pragma omp for ordered(1)// 声明一个 ordered(1) 循环，表示该循环包含一个顺序区域，且依赖关系的距离为 1
    for (i = 0; i < N; i++) {
        A[i] = foo(i);
        #pragma omp ordered depend(source)// 声明一个 ordered depend(source) 构造，表示该代码块是一个依赖源，后续的迭代可以依赖于它
        #pragma omp ordered depend(sink: i-1)// 声明一个 ordered depend(sink: i-1) 构造，表示该代码块依赖于前一个迭代的结果
        B[i] = bar(A[i], i > 0 ? A[i-1] : 0.0f);
        C[i] = baz(B[i]);
    }
}

int main() {
    const int N = 100;
    float A[N], B[N], C[N];
    
    #pragma omp parallel
    {
        work(N, A, B, C);
    }

    // 输出结果以验证正确性
    for (int i = 0; i < N; i++) {
        std::cout << "A[" << i << "] = " << A[i] 
                  << ", B[" << i << "] = " << B[i] 
                  << ", C[" << i << "] = " << C[i] << std::endl;
    }

    return 0;
}

float foo(int i) {
    return i * 1.0f;
}

float bar(float a, float b) {
    return a + b;
}

float baz(float b) {
    return b * 2.0f;
}
