#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include <cpuid.h>

#ifdef USE_STRNSTR
#define STRSTR(haystack,needle) strnstr((haystack),(needle),sizeof(haystack))
#else
#define STRSTR(haystack,needle) strstr((haystack),(needle))
#endif

static void get_arch(bool * skylake)
{
    uint32_t eax=0x0, ebx=0x0, ecx=0x0, edx=0x0;
    __cpuid(0x0, eax, ebx, ecx, edx);
#ifdef DEBUG
    printf("0x0: %x,%x,%x,%x\n", eax, ebx, ecx, edx);
#endif
    *skylake = (ebx & 0x16);
}

#if 0
static void get_avx(bool * avx, bool * fma)
{
    uint32_t eax=0x1, ebx=0x0, ecx=0x0, edx=0x0;
    __cpuid(eax, eax, ebx, ecx, edx);
#ifdef DEBUG
    printf("0x1: %x,%x,%x,%x\n", eax, ebx, ecx, edx);
#endif
    *avx = (ecx & 1u<<28);
    *fma = (ecx & 1u<<12);
}
#endif

static void get_avx512(bool * knl, bool * skx)
{
    uint32_t eax=0x7, ebx=0x0, ecx=0x0, edx=0x0;
    __cpuid_count(eax, ecx, eax, ebx, ecx, edx);
#ifdef DEBUG
    printf("0x7: %x,%x,%x,%x\n", eax, ebx, ecx, edx);
#endif
#if 0
    *avx2     = (ebx & 1u<<5);
#endif
    const bool avx512f  = (ebx & 1u<<16);
    const bool avx512dq = (ebx & 1u<<17);
    const bool avx512pf = (ebx & 1u<<26);
    const bool avx512er = (ebx & 1u<<27);
    const bool avx512cd = (ebx & 1u<<28);
    const bool avx512bw = (ebx & 1u<<30);
    const bool avx512vl = (ebx & 1u<<31);
    *knl = avx512f && avx512cd && avx512pf && avx512er;
    *skx = avx512f && avx512cd && avx512bw && avx512dq && avx512vl;
}

static void get_cpu_name(char cpu_name[48])
{
    uint32_t eax, ebx, ecx, edx;

    __cpuid(0x80000002u, eax, ebx, ecx, edx);
    //printf("%x %x %x %x\n", eax, ebx, ecx, edx);

    *(uint32_t *)&cpu_name[0]  = eax;
    *(uint32_t *)&cpu_name[4]  = ebx;
    *(uint32_t *)&cpu_name[8]  = ecx;
    *(uint32_t *)&cpu_name[12] = edx;

    __cpuid(0x80000003u, eax, ebx, ecx, edx);
    //printf("%x %x %x %x\n", eax, ebx, ecx, edx);

    *(uint32_t *)&cpu_name[16+0]  = eax;
    *(uint32_t *)&cpu_name[16+4]  = ebx;
    *(uint32_t *)&cpu_name[16+8]  = ecx;
    *(uint32_t *)&cpu_name[16+12] = edx;

    __cpuid(0x80000004u, eax, ebx, ecx, edx);
    //printf("%x %x %x %x\n", eax, ebx, ecx, edx);

    *(uint32_t *)&cpu_name[32+0]  = eax;
    *(uint32_t *)&cpu_name[32+4]  = ebx;
    *(uint32_t *)&cpu_name[32+8]  = ecx;
    *(uint32_t *)&cpu_name[32+12] = edx;
}

int vpu_count(void)
{
    char cpu_name[48] = {0};

    get_cpu_name(cpu_name);
#ifdef DEBUG
    printf("cpu_name = %s\n", cpu_name);
#endif

    const char skx[17]     = "Intel(R) Xeon(R)";
    const char skw[30]     = "Intel(R) Xeon(R) Processor W-";
    const char knl[29]     = "Intel(R) Xeon Phi(TM) CPU 72";
    /* Intel(R) Core(TM) i7-5960X CPU */
    const char i7[22]      = "Intel(R) Core(TM) i7-";
    const char i9[22]      = "Intel(R) Core(TM) i9-";
    const char platinum[9] = "Platinum";
    const char gold[5]     = "Gold";
    const char silver[7]   = "Silver";
    const char bronze[7]   = "Bronze";

    const char * loc;

    bool is_skl, is_knl, is_skx;
    get_arch(&is_skl);
    get_avx512(&is_knl, &is_skx);

    /* Xeon Phi 72xx aka KNL always has 2 VPU */
    loc = STRSTR(cpu_name, knl);
    if (loc != NULL) {
        return 2;
    }

    /* Skylake-X series */
    loc = STRSTR(cpu_name, knl);
    if (loc != NULL) {
        return 2;
    }

    /* If it is not Xeon, it doesn't have AVX-512 */
    loc = STRSTR(cpu_name, skx);
    if (loc == NULL) {
        return 0;
    }

    loc = STRSTR(cpu_name, platinum);
    if (loc != NULL) {
        char skustr[5] = {0};
        memcpy(&skustr,loc+sizeof(platinum),4);
        const int skunum = atoi(skustr);
        if (8199 >= skunum && skunum >= 8100) {
            return 2;
        } else {
            printf("%s %d does not exist.\n", platinum, skunum);
        }
    }

    loc = STRSTR(cpu_name, gold);
    if (loc != NULL) {
        char skustr[5] = {0};
        memcpy(&skustr,loc+sizeof(gold),4);
        const int skunum = atoi(skustr);
        if (skunum == 5122) {
            /* https://ark.intel.com/products/120475/Intel-Xeon-Gold-5122-Processor-16_5M-Cache-3_60-GHz */
            return 2;
        } else if (6199 >= skunum && skunum >= 6100) {
            return 2;
        } else if (5199 >= skunum && skunum >= 5100) {
            return 1;
        } else {
            printf("%s %d does not exist.\n", gold, skunum);
        }
    }

    loc = STRSTR(cpu_name, silver);
    if (loc != NULL) {
        char skustr[5] = {0};
        memcpy(&skustr,loc+sizeof(silver),4);
        const int skunum = atoi(skustr);
        if (4199 >= skunum && skunum >= 4100) {
            return 1;
        } else {
            printf("%s %d does not exist.\n", silver, skunum);
        }
    }

    loc = STRSTR(cpu_name, bronze);
    if (loc != NULL) {
        char skustr[5] = {0};
        memcpy(&skustr,loc+sizeof(bronze),4);
        const int skunum = atoi(skustr);
        if (3199 >= skunum && skunum >= 3100) {
            return 1;
        } else {
            printf("%s %d does not exist.\n", bronze, skunum);
        }
    }

    /* Skylake-W series */
    loc = STRSTR(cpu_name, skw);
    if (loc != NULL) {
        char skustr[5] = {0};
        memcpy(&skustr,loc+sizeof(skw),4);
        const int skunum = atoi(skustr);
        if (2199 >= skunum && skunum >= 2120) {
            return 2;
        } else if (2120 > skunum && skunum >= 2100) {
            return 1;
        } else {
            printf("%s%d does not exist.\n", skw, skunum);
        }
    }


    return 0;
}
