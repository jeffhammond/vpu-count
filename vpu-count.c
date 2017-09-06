#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <cpuid.h>

#ifdef USE_STRNSTR
#define STRSTR(haystack,needle) strnstr((haystack),(needle),sizeof(haystack))
#else
#define STRSTR(haystack,needle) strstr((haystack),(needle))
#endif

static int get_cpu_name(char cpu_name[48])
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

    return 0;
}

int vpu_count(void)
{
    char cpu_name[48] = {0};

    get_cpu_name(cpu_name);
#ifdef DEBUG
    printf("cpu_name = %s\n", cpu_name);
#endif

    const char xeon[17] = "Intel(R) Xeon(R)";
    const char platinum[9] = "Platinum";
    const char gold[5]     = "Gold";
    const char silver[7]   = "Silver";
    const char bronze[7]   = "Bronze";

    const char * loc;

    /* FIXME: KNL should return 2 before the Xeon check. */

    loc = STRSTR(cpu_name, xeon);
    if (loc == NULL) {
        return 0;
    }

    loc = STRSTR(cpu_name, platinum);
    if (loc != NULL) {
        return 2;
    }

    loc = STRSTR(cpu_name, gold);
    if (loc != NULL) {
        char skustr[5] = {0};
        memcpy(&skustr,loc+sizeof(gold),4);
        const int skunum = atoi(skustr);
        if (skunum == 5122) {
            /* https://ark.intel.com/products/120475/Intel-Xeon-Gold-5122-Processor-16_5M-Cache-3_60-GHz */
            return 2;
        } else if (6999 >= skunum && skunum >= 6000) {
            return 2;
        } else if (5999 >= skunum && skunum >= 5000) {
            return 1;
        } else {
            printf("%s %d does not exist.\n", gold, skunum);
        }
    }

    loc = STRSTR(cpu_name, silver);
    if (loc != NULL) {
        return 1;
    }

    loc = STRSTR(cpu_name, bronze);
    if (loc != NULL) {
        return 1;
    }

    return 0;
}
