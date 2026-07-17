#include <stdlib.h>
#include <stdio.h>
#define N 100

typedef struct node_t {
    int val;
    struct node_t *next;
} node_t;

int linked_list_sum(node_t *p){
    int res = 0;

    #pragma omp taskgroup task_reduction(+:res)
    {
        node_t* aux = p;
        while(aux != 0){
            #pragma omp task in_reduction(+:res)
            res += aux->val;

            aux = aux->next;
            
        }
        
    }    
    return res;
}

int main(int argc, char *argv[]){
    int i;
    // Create a linked list of N nodes
    node_t * root = (node_t*)malloc(sizeof(node_t));
    root->val = 1;
    
    node_t * aux = root;
    // Create the rest of the nodes
    for(i=2;i<=N;i++){
        aux->next = (node_t*)malloc(sizeof(node_t));
        aux = aux->next;
        aux->val = i;
    }
    aux->next = 0;

    #pragma omp parallel
    #pragma omp single
    {
        int result = linked_list_sum(root);
        printf("Calculated: %d Analytic:%d\n", result, N*(N+1)/2);
    }
    return 0;




}
