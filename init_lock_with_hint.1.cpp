#include <omp.h>
#include <iostream>

omp_lock_t *new_locks()
{
    int i;
    omp_lock_t *lock = new omp_lock_t[1000];

    #pragma omp parallel for private(i)
    for (i=0; i<1000; i++)
    {
        omp_init_lock_with_hint(&lock[i],
            static_cast<omp_lock_hint_t>(omp_lock_hint_contended |
                                         omp_lock_hint_speculative));
    }
    return lock;
}

int main()
{
    omp_lock_t *locks = new_locks();

    std::cout << "1000 locks initialized with hints." << std::endl;

    #pragma omp parallel for
    for (int i = 0; i < 1000; i++) {
        omp_set_lock(&locks[i]);
        omp_unset_lock(&locks[i]);
    }

    std::cout << "All locks acquired and released." << std::endl;

    for (int i = 0; i < 1000; i++)
        omp_destroy_lock(&locks[i]);
    delete[] locks;

    return 0;
}
