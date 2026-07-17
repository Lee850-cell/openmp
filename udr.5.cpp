#include <stdio.h>

class V {
    float *p;
    int n;

public:
    V(int _n) : n(_n)        { p = new float[n]; for(int i=0;i<n;i++) p[i]=0; }
    V(const V& m) : n(m.n)   { p = new float[n]; for(int i=0;i<n;i++) p[i]=m.p[i]; }
    ~V()                     { delete[] p; }

    V& operator+= (const V& m) {
        for (int i = 0; i < n; i++)
            p[i] += m.p[i];
        return *this;
    }

    float& operator[](int i) { return p[i]; }
    float  operator[](int i) const { return p[i]; }
    int size() const { return n; }
};

// UDR 必须放在类外部（文件作用域）
#pragma omp declare reduction(+ : V : omp_out += omp_in) \
    initializer(omp_priv(omp_orig))

int main() {
    const int N = 10;
    V vec(N), ones(N);

    // 初始化
    for (int i = 0; i < N; i++) {
        vec[i]  = static_cast<float>(i);     // vec  = [0,1,2,...,9]
        ones[i] = 1.0f;                      // ones = [1,1,1,...,1]
    }

    // 并行归约：vec += ones 共 100 次
    #pragma omp parallel for reduction(+ : vec)
    for (int k = 0; k < 100; k++) {
        vec += ones;   // 每次迭代：vec 每个元素 +1
    }

    // 验证：vec[i] = i + 100
    printf("vec after 100 iterations of += ones:\n");
    for (int i = 0; i < N; i++)
        printf("vec[%d] = %.0f (expected %d)\n", i, vec[i], i + 100);

    return 0;
}
