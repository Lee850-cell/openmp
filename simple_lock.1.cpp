#include <iostream>
#include <omp.h>
using namespace std;

void skip(int i){
    cout << "skip thread " << i << endl;
}
void work(int i){
    cout <<"work thread " << i << endl;
}
int main(){
    omp_lock_t lck;
    int id;
    omp_init_lock(&lck);

    #pragma omp parallel shared(lck) private(id)
    {
        id = omp_get_thread_num();

        omp_set_lock(&lck);
        /*only one thread can execute this critical section*/
        cout << "Thread " << id << " is in the critical section." << endl;
        omp_unset_lock(&lck);
        
        while(! omp_test_lock(&lck)){
            skip(id);
        }
        work(id);

        omp_unset_lock(&lck);
    }
    omp_destroy_lock(&lck);

    return 0;
    
}