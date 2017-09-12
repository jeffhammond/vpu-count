#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <cpuid.h>

void get_cpu_name(char cpu_name[48])
{
    uint32_t eax, ebx, ecx, edx;

    __cpuid(0x80000002u, eax, ebx, ecx, edx);
    *(uint32_t *)&cpu_name[0]  = eax;
    *(uint32_t *)&cpu_name[4]  = ebx;
    *(uint32_t *)&cpu_name[8]  = ecx;
    *(uint32_t *)&cpu_name[12] = edx;

#ifdef DEBUG
    printf("%x %x %x %x\n", eax, ebx, ecx, edx);
#endif

    __cpuid(0x80000003u, eax, ebx, ecx, edx);
    *(uint32_t *)&cpu_name[16+0]  = eax;
    *(uint32_t *)&cpu_name[16+4]  = ebx;
    *(uint32_t *)&cpu_name[16+8]  = ecx;
    *(uint32_t *)&cpu_name[16+12] = edx;

#ifdef DEBUG
    printf("%x %x %x %x\n", eax, ebx, ecx, edx);
#endif

    __cpuid(0x80000004u, eax, ebx, ecx, edx);
    *(uint32_t *)&cpu_name[32+0]  = eax;
    *(uint32_t *)&cpu_name[32+4]  = ebx;
    *(uint32_t *)&cpu_name[32+8]  = ecx;
    *(uint32_t *)&cpu_name[32+12] = edx;

#ifdef DEBUG
    printf("%x %x %x %x\n", eax, ebx, ecx, edx);
#endif

#ifdef DEBUG
    printf("0x80000002u: \"%.*s\"\n", 16, &cpu_name[0]);
    printf("0x80000003u: \"%.*s\"\n", 16, &cpu_name[16]);
    printf("0x80000004u: \"%.*s\"\n", 16, &cpu_name[32]);
#endif
}

int vpu_count(void)
{
    uint32_t leaf7[4] = {0x7,0x0,0x0,0x0};
    __cpuid_count(leaf7[0], leaf7[2], leaf7[0], leaf7[1], leaf7[2], leaf7[3]);
#ifdef DEBUG
    printf("0x7: %x,%x,%x,%x\n", leaf7[0], leaf7[1], leaf7[2], leaf7[3]);
#endif

#ifdef DETECT_XEON_PHI
    bool knl = (leaf7[1] & 1u<<16) && /* AVX-512F  */
               (leaf7[1] & 1u<<28) && /* AVX-512CD */
               (leaf7[1] & 1u<<26) && /* AVX-512PF */
               (leaf7[1] & 1u<<27);   /* AVX-512ER */
    /* KNM is a superset of KNL, at least architecturally */
    bool knm = knl && (leaf7[2] & 1u<<14); /* AVX-512VPOPCNTDQ */

    if (knl || knm) {
#ifdef DEBUG
        if (knm) printf("KNM\n");
        else     printf("KNL\n");
#endif
        return 2;
    }
#endif

    /* Architecture, so we know it is SKL and not something later... */
    uint32_t leaf0[4]={0x0,0x0,0x0,0x0};
    __cpuid(leaf0[0], leaf0[0], leaf0[1], leaf0[2], leaf0[3]);
#ifdef DEBUG
    printf("0x0: %x,%x,%x,%x\n", leaf0[0], leaf0[1], leaf0[2], leaf0[3]);
#endif

    bool skylake_avx512 = (leaf0[1] & 0x16)   && /* Skylake   */
                          (leaf7[1] & 1u<<16) && /* AVX-512F  */
                          (leaf7[1] & 1u<<17) && /* AVX-512DQ */
                          (leaf7[1] & 1u<<28) && /* AVX-512CD */
                          (leaf7[1] & 1u<<30) && /* AVX-512BW */
                          (leaf7[1] & 1u<<31);   /* AVX-512VL */

    if (skylake_avx512) {
#ifdef DEBUG
        printf("SKX\n");
#endif
        char cpu_name[48] = {0};
        get_cpu_name(cpu_name);

#ifdef DEBUG
        printf("cpu_name = %s\n", cpu_name);
        printf("cpu_name[9] = %c\n", cpu_name[9]);
        printf("cpu_name[17] = %c\n", cpu_name[17]);
#if 0
        char letters[8] = {'C','P','G','S','B','W'};
        for (int i=0; i<6; i++) printf("%d,%d\n",i,(int)letters[i]);
#endif
#endif


        /* Skylake-X series: * "Intel(R) Core (TM)..." */
        if (cpu_name[9] == 'C') {
            /* FIXME need to figure out the answer here... */
            return 911;
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
    return 0;
}

bool vpu_avx512(void)
{
    uint32_t leaf7[4] = {0x7,0x0,0x0,0x0};
    __cpuid_count(leaf7[0], leaf7[2], leaf7[0], leaf7[1], leaf7[2], leaf7[3]);
#ifdef DEBUG
    printf("0x7: %x,%x,%x,%x\n", leaf7[0], leaf7[1], leaf7[2], leaf7[3]);
#endif

#ifdef DETECT_XEON_PHI
    bool knl = (leaf7[1] & 1u<<16) && /* AVX-512F  */
               (leaf7[1] & 1u<<28) && /* AVX-512CD */
               (leaf7[1] & 1u<<26) && /* AVX-512PF */
               (leaf7[1] & 1u<<27);   /* AVX-512ER */
    /* KNM is a superset of KNL, at least architecturally */
    bool knm = knl && (leaf7[2] & 1u<<14); /* AVX-512VPOPCNTDQ */

    if (knl || knm) {
#ifdef DEBUG
        if (knm) printf("KNM\n");
        else     printf("KNL\n");
#endif
        return 2;
    }
#endif

    /* Architecture, so we know it is SKL and not something later... */
    uint32_t leaf0[4]={0x0,0x0,0x0,0x0};
    __cpuid(leaf0[0], leaf0[0], leaf0[1], leaf0[2], leaf0[3]);
#ifdef DEBUG
    printf("0x0: %x,%x,%x,%x\n", leaf0[0], leaf0[1], leaf0[2], leaf0[3]);
#endif

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
    if (true) {
#ifdef DEBUG
        printf("SKX\n");
#endif
        char cpu_name[48] = {0};
        get_cpu_name(cpu_name);

#ifdef DEBUG
        printf("cpu_name = %s\n", cpu_name);
        printf("cpu_name[9] = %c\n", cpu_name[9]);
        printf("cpu_name[17] = %c\n", cpu_name[17]);
#if 0
        char letters[8] = {'C','P','G','S','B','W'};
        for (int i=0; i<6; i++) printf("%d,%d\n",i,(int)letters[i]);
#endif
#endif

        /* Skylake-X series: * "Intel(R) Core (TM)..." */
        if (cpu_name[9] == 'C') {
            /* FIXME need to figure out the answer here... */
            return 911;
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
    return 0;
}
