#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <omp.h>

int vpu_count();
bool vpu_avx512();
int vpu_name();
bool vpu_platinum();

int main(int argc, char * argv[])
{
    int n = (argc>1) ? atoi(argv[1]) : 1000000;

    vpu_count();

    for (int r=0; r<10; r++) {
        double t0, t1;
        int j;

        j = 0;
        t0 = omp_get_wtime();
        for (int i=0; i<n; i++) {
            j += vpu_count();
        }
        t1 = omp_get_wtime();
        printf("vpu_count: n=%d, dt=%f us, dt/n=%f us, j=%d\n", n, 1.e6*(t1-t0), 1.e6*(t1-t0)/n, j);

        j = 0;
        t0 = omp_get_wtime();
        for (int i=0; i<n; i++) {
            j += (int)vpu_avx512();
        }
        t1 = omp_get_wtime();
        printf("vpu_avx512: n=%d, dt=%f us, dt/n=%f us, j=%d\n", n, 1.e6*(t1-t0), 1.e6*(t1-t0)/n, j);

        j = 0;
        t0 = omp_get_wtime();
        for (int i=0; i<n; i++) {
            j += vpu_name();
        }
        t1 = omp_get_wtime();
        printf("vpu_name: n=%d, dt=%f us, dt/n=%f us, j=%d\n", n, 1.e6*(t1-t0), 1.e6*(t1-t0)/n, j);

        j = 0;
        t0 = omp_get_wtime();
        for (int i=0; i<n; i++) {
            j += (int)vpu_platinum();
        }
        t1 = omp_get_wtime();
        printf("vpu_platinum: n=%d, dt=%f us, dt/n=%f us, j=%d\n", n, 1.e6*(t1-t0), 1.e6*(t1-t0)/n, j);
    }
    return 0;
}

