#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <omp.h>
using namespace std;

int main()
{
    int x;
    omp_lock_t lck;

    omp_init_lock (&lck);
    omp_set_lock (&lck);
    x = 0;

    #pragma omp parallel shared (x)
    {
        #pragma omp master
        {
            x = x + 1;
            omp_unset_lock (&lck);
        }

        /* Some more stuff. */
    }
    omp_destroy_lock (&lck);
    if(x){
        cout << "x is " << x << endl;
    }
    else{
        cout << "lock and unlock must be in the same thread " << endl;
    }
    return 0;
}
