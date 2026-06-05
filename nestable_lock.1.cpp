#include <iostream>
#include <omp.h>
using namespace std;

typedef struct {
    int a, b;
    omp_nest_lock_t lck;
} Pair;

int work1() { return 1; }
int work2() { return 2; }
int work3() { return 3; }

void incr_a(Pair *p, int a)
{
    /* Called only from incr_pair, no need to lock. */
    p->a += a;
}

void incr_b(Pair *p, int b)
{
    /* Called both from incr_pair and elsewhere, */
    /* so need a nestable lock. */
    omp_set_nest_lock(&p->lck);
    p->b += b;
    omp_unset_nest_lock(&p->lck);
}

void incr_pair(Pair *p, int a, int b)
{
    omp_set_nest_lock(&p->lck);
    incr_a(p, a);
    incr_b(p, b);
    omp_unset_nest_lock(&p->lck);
}

void f(Pair *p)
{
    #pragma omp parallel sections
    {
        #pragma omp section
        incr_pair(p, work1(), work2());
        #pragma omp section
        incr_b(p, work3());
    }
}

int main()
{
    Pair p;
    p.a = 0;
    p.b = 0;
    omp_init_nest_lock(&p.lck);

    f(&p);

    omp_destroy_nest_lock(&p.lck);
    cout << "a = " << p.a << ", b = " << p.b << endl;
    return 0;
}
