#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include <cpuid.h>

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

#ifdef DEBUG
    for (int i=0; i<48; i++) {
        printf("%c", cpu_name[i]);
    }
    printf("\n");
#endif

    return 0;
}


#ifdef USE_STRNSTR
#define STRSTR(haystack,needle) strnstr((haystack),(needle),sizeof(haystack))
#else
#define STRSTR(haystack,needle) strstr((haystack),(needle))
#endif

int vpu_count(void)
{
    char cpu_name[48] = {0};

    get_cpu_name(cpu_name);
    printf("cpu_name = %s\n", cpu_name);

    const char platinum[26] = "Intel(R) Xeon(R) Platinum";
    const char gold[22]     = "Intel(R) Xeon(R) Gold";
    const char silver[24]   = "Intel(R) Xeon(R) Silver";
    const char bronze[24]   = "Intel(R) Xeon(R) Bronze";

    char skustr[5] = {0};
    int  skunum = 0;

    const char * loc;

    loc = STRSTR(cpu_name, platinum);
    if (loc != NULL) {
        memcpy(&skustr,loc+sizeof(platinum),4);
        skunum = atoi(skustr);
        if (skunum >= 8000) {
            return 2;
        } else {
            printf("%s %d does not exist.\n", platinum, skunum);
        }
    }

    loc = STRSTR(cpu_name, gold);
    if (loc != NULL) {
        memcpy(&skustr,loc+sizeof(gold),4);
        skunum = atoi(skustr);
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
        memcpy(&skustr,loc+sizeof(silver),4);
        skunum = atoi(skustr);
        if (4999 >= skunum && skunum >= 4000) {
            return 1;
        } else {
            printf("%s %d does not exist.\n", silver, skunum);
        }
    }

    loc = STRSTR(cpu_name, bronze);
    if (loc != NULL) {
        memcpy(&skustr,loc+sizeof(bronze),4);
        skunum = atoi(skustr);
        if (3999 >= skunum && skunum >= 3000) {
            return 1;
        } else {
            printf("%s %d does not exist.\n", bronze, skunum);
        }
    }

    return 0;
}
