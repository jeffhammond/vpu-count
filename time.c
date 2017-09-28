#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <omp.h>

#include <cpuid.h>

int vpu_count();
void get_cpu_name32(char cpu_name[32]);

bool vpu_platinum(void)
{
    char cpu_name[32] = {0};
    get_cpu_name32(cpu_name);
    return (cpu_name[9] == 'X' && cpu_name[17] == 'P');
}

bool vpu_avx512(void)
{
    uint32_t leaf0[4]={0x0,0x0,0x0,0x0};
    __cpuid(leaf0[0], leaf0[0], leaf0[1], leaf0[2], leaf0[3]);

    uint32_t leaf7[4] = {0x7,0x0,0x0,0x0};
    __cpuid_count(leaf7[0], leaf7[2], leaf7[0], leaf7[1], leaf7[2], leaf7[3]);

    bool skylake_avx512 = (leaf0[1] & 0x16)   && /* Skylake   */
                          (leaf7[1] & 1u<<16) && /* AVX-512F  */
                          (leaf7[1] & 1u<<17) && /* AVX-512DQ */
                          (leaf7[1] & 1u<<28) && /* AVX-512CD */
                          (leaf7[1] & 1u<<30) && /* AVX-512BW */
                          (leaf7[1] & 1u<<31);   /* AVX-512VL */

    return skylake_avx512;
}

int vpu_name(void)
{
    char cpu_name[32] = {0};
    get_cpu_name32(cpu_name);

    /* Skylake-X series: * "Intel(R) Core (TM)..." */
    if (cpu_name[9] == 'C') {
        return 2;
    }
    else if (cpu_name[9] == 'X') {
        /* Xeon Scalable series: "Intel(R) Xeon(R) Platinum..." */
        if (cpu_name[17] == 'P') {
            return 2;
        }
        /* Xeon Scalable series: "Intel(R) Xeon(R) Silver..." */
        else if (cpu_name[17] == 'S') {
            return 1;
        }
        /* Xeon Scalable series: "Intel(R) Xeon(R) Bronze..." */
        else if (cpu_name[17] == 'B') {
            return 1;
        }
        /* Xeon Scalable series: "Intel(R) Xeon(R) Gold..." */
        else if (cpu_name[17] == 'G') {
            /* 61xx */
            if (cpu_name[22] == '6') {
                return 2;
            /* 5122 */
            } else if (cpu_name[22] == 5 && cpu_name[24] == 2 && cpu_name[25] == 2) {
                return 2;
            /* 51xx */
            } else {
                return 1;
            }
        }
        /* Xeon W series: "Intel(R) Xeon(R) Processor W-21xx..." */
        else if (cpu_name[27] == 'W') {
            /* 2102 or 2104 */
            if (cpu_name[31] == '0') {
                return 1;
            /* 212x and higher */
            } else {
                return 2;
            }
        }
    }
    /* If we get here, the part is not supported by the SKX logic */
    return -1;
}

int main(int argc, char * argv[])
{
    int n = (argc>1) ? atoi(argv[1]) : 1000;

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

