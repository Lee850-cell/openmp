#include <stdio.h>
#include <limits.h>

struct point {
    int x;
    int y;
};

void minproc(struct point *out, struct point *in) {
    if (in->x < out->x) out->x = in->x;
    if (in->y < out->y) out->y = in->y;
}

void maxproc(struct point *out, struct point *in) {
    if (in->x > out->x) out->x = in->x;
    if (in->y > out->y) out->y = in->y;
}

#pragma omp declare reduction(min : struct point : \
    minproc(&omp_out, &omp_in)) \
    initializer(omp_priv = {INT_MAX, INT_MAX})

#pragma omp declare reduction(max : struct point : \
    maxproc(&omp_out, &omp_in)) \
    initializer(omp_priv = {0, 0})

    void find_enclosing_rectangle ( int n,struct point points[])
    {
        struct point minp = {INT_MAX, INT_MAX}, maxp = {0, 0};
        int i;
    
    #pragma omp parallel for reduction(min : minp) reduction(max : maxp)
        for(i=0; i<n; i++){
            if (points[i].x < minp.x) minp.x = points[i].x;
            if (points[i].y < minp.y) minp.y = points[i].y;
            if (points[i].x > maxp.x) maxp.x = points[i].x;
            if (points[i].y > maxp.y) maxp.y = points[i].y;
        }
        printf("min = (%d, %d)\n", minp.x, minp.y);
        printf("max = (%d, %d)\n", maxp.x, maxp.y);
    }

int main() {
    struct point points[] = {
        {3, 8}, {1, 5}, {7, 2}, {9, 4}, {5, 6},
        {2, 9}, {4, 1}, {8, 7}, {6, 3}, {0, 5}
    };
    int n = 10;
    find_enclosing_rectangle(n, points);
    return 0;
}