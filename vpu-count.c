#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <cpuid.h>

#if (__STDC_VERSION__ < 199901L)
#error You need to enable C99 or remove vararg debug printing by hand.
#endif

#ifdef DEBUG
#define PDEBUG(fmt, ...) do { printf(fmt, __VA_ARGS__); } while (0)
#else
#define PDEBUG(fmt, ...)
#endif

void get_cpu_name32(char cpu_name[32])
{
    uint32_t eax, ebx, ecx, edx;

    __cpuid(0x80000002u, eax, ebx, ecx, edx);
    *(uint32_t *)&cpu_name[0]  = eax;
    *(uint32_t *)&cpu_name[4]  = ebx;
    *(uint32_t *)&cpu_name[8]  = ecx;
    *(uint32_t *)&cpu_name[12] = edx;

    __cpuid(0x80000003u, eax, ebx, ecx, edx);
    *(uint32_t *)&cpu_name[16+0]  = eax;
    *(uint32_t *)&cpu_name[16+4]  = ebx;
    *(uint32_t *)&cpu_name[16+8]  = ecx;
    *(uint32_t *)&cpu_name[16+12] = edx;
}

void get_cpu_name48(char cpu_name[48])
{
    uint32_t eax, ebx, ecx, edx;

    __cpuid(0x80000002u, eax, ebx, ecx, edx);
    *(uint32_t *)&cpu_name[0]  = eax;
    *(uint32_t *)&cpu_name[4]  = ebx;
    *(uint32_t *)&cpu_name[8]  = ecx;
    *(uint32_t *)&cpu_name[12] = edx;

    PDEBUG("%x %x %x %x\n", eax, ebx, ecx, edx);

    __cpuid(0x80000003u, eax, ebx, ecx, edx);
    *(uint32_t *)&cpu_name[16+0]  = eax;
    *(uint32_t *)&cpu_name[16+4]  = ebx;
    *(uint32_t *)&cpu_name[16+8]  = ecx;
    *(uint32_t *)&cpu_name[16+12] = edx;

    PDEBUG("%x %x %x %x\n", eax, ebx, ecx, edx);

    __cpuid(0x80000004u, eax, ebx, ecx, edx);
    *(uint32_t *)&cpu_name[32+0]  = eax;
    *(uint32_t *)&cpu_name[32+4]  = ebx;
    *(uint32_t *)&cpu_name[32+8]  = ecx;
    *(uint32_t *)&cpu_name[32+12] = edx;

    PDEBUG("%x %x %x %x\n", eax, ebx, ecx, edx);

    PDEBUG("0x80000002u: \"%.*s\"\n", 16, &cpu_name[0]);
    PDEBUG("0x80000003u: \"%.*s\"\n", 16, &cpu_name[16]);
    PDEBUG("0x80000004u: \"%.*s\"\n", 16, &cpu_name[32]);
}

void get_leaf0(uint32_t leaf0[4], bool * is_intel, bool * skylake)
{
    __cpuid(leaf0[0], leaf0[0], leaf0[1], leaf0[2], leaf0[3]);
    PDEBUG("0x0: %x,%x,%x,%x\n", leaf0[0], leaf0[1], leaf0[2], leaf0[3]);

    *is_intel = (leaf0[1] == 0x756e6547) && (leaf0[2] == 0x6c65746e) && (leaf0[3] == 0x49656e69);
    PDEBUG("Intel? %s\n", *is_intel ? "yes" : "no");

    *skylake = (leaf0[1] & 0x16);
    PDEBUG("Skylake? %s\n", *skylake ? "yes" : "no");
}

void get_leaf1(uint32_t leaf1[4], bool * skylake_avx512)
{
    __cpuid(leaf1[0], leaf1[0], leaf1[1], leaf1[2], leaf1[3]);
    PDEBUG("0x1: %x,%x,%x,%x\n", leaf1[0], leaf1[1], leaf1[2], leaf1[3]);

    //uint32_t stepping  = (leaf1[0]      ) & 0x0f;
    uint32_t model     = (leaf1[0] >>  4) & 0x0f;
    uint32_t family    = (leaf1[0] >>  8) & 0x0f;
    //uint32_t proctype  = (leaf1[0] >> 12) & 0x03;
    uint32_t xmodel    = (leaf1[0] >> 16) & 0x0f;
    //uint32_t xfamily   = (leaf1[0] >> 20) & 0xff;

    if (family == 0x06) {
        model  += (xmodel << 4);
    }
    else if (family == 0x0f) {
        model  += (xmodel << 4);
        //family += xfamily;
    }
    *skylake_avx512 = (model == 0x55);

    PDEBUG("signature:  %#08x\n", (leaf1[0]) );
    //PDEBUG("stepping:   %#04x=%d\n", stepping, stepping);
    PDEBUG("model:      %#04x=%d\n", model, model);
    PDEBUG("family:     %#04x=%d\n", family, family);
    //PDEBUG("proc type:  %#04x=%d\n", proctype, proctype);
    PDEBUG("ext model:  %#04x=%d\n", xmodel, xmodel);
    //PDEBUG("ext family: %#08x=%d\n", xfamily, xfamily);
}

void get_leaf7(uint32_t leaf7[4], bool * xeon_avx512, bool * knl, bool * knm)
{
    __cpuid_count(leaf7[0], leaf7[2], leaf7[0], leaf7[1], leaf7[2], leaf7[3]);
    PDEBUG("0x7: %x,%x,%x,%x\n", leaf7[0], leaf7[1], leaf7[2], leaf7[3]);

    *knl = (leaf7[1] & 1u<<16) && /* AVX-512F  */
           (leaf7[1] & 1u<<28) && /* AVX-512CD */
           (leaf7[1] & 1u<<26) && /* AVX-512PF */
           (leaf7[1] & 1u<<27);   /* AVX-512ER */

    /* KNM is a superset of KNL, at least architecturally */
    *knm = *knl && (leaf7[2] & 1u<<14); /* AVX-512VPOPCNTDQ */

    *xeon_avx512 = (leaf7[1] & 1u<<16) && /* AVX-512F  */
                   (leaf7[1] & 1u<<17) && /* AVX-512DQ */
                   (leaf7[1] & 1u<<28) && /* AVX-512CD */
                   (leaf7[1] & 1u<<30) && /* AVX-512BW */
                   (leaf7[1] & 1u<<31);   /* AVX-512VL */

#ifdef DEBUG
    const char * name = "None";
    if (*xeon_avx512) name = "Xeon";
    else if (*knm) name = "KNM";
    else if (*knl) name = "KNL";
    PDEBUG("AVX-512: %s\n", name);
#endif
}

int vpu_count(void)
{
    /* We can either detect Skylake AVX-512 directly from leaf1 or detect it
     * via Skylake arch from leaf0 plus AVX-512 features from leaf7.
     * Clearly, the former is simpler. */
    bool skylake_avx512 = false;
#if 1
    /* leaf 1 - Model, Family, etc. */
    uint32_t leaf1[4]={0x1,0x0,0x0,0x0};
    get_leaf1(leaf1, &skylake_avx512);
#else
    /* leaf 0 - Architecture */
    bool is_intel = false, skylake = false;
    uint32_t leaf0[4]={0x0,0x0,0x0,0x0};
    get_leaf0(leaf0, &is_intel, &skylake);

    /* leaf 7 - AVX-512 features */
    bool xeon_avx512 = false, knl = false, knm = false;
    uint32_t leaf7[4]={0x7,0x0,0x0,0x0};
    get_leaf7(leaf7, &xeon_avx512, &knl, &knm);

    skylake_avx512 = skylake && xeon_avx512;
#endif

    if (skylake_avx512) {
        PDEBUG("Skylake AVX-512 detected...\n");
        char cpu_name[32] = {0};
        get_cpu_name32(cpu_name);

        PDEBUG("cpu_name = %s\n", cpu_name);
        PDEBUG("cpu_name[9] = %c\n", cpu_name[9]);
        PDEBUG("cpu_name[17] = %c\n", cpu_name[17]);

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
    return 0;
}
