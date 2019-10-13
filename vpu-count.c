//
// MIT License
//
// Copyright (c) 2017 Intel Corporation
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// This code was written by Jeff Hammond <jeff.r.hammond@intel.com>.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <cpuid.h>

#if (__STDC_VERSION__ < 199901L)
#error You need to enable C99 or remove vararg debug printing by hand.
#endif

#ifdef DEBUG
#define PDEBUG(...) printf(__VA_ARGS__)
#else
#define PDEBUG(...)
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

bool is_intel(void)
{
    /* leaf 0 - Architecture */
    uint32_t leaf0[4]={0x0,0x0,0x0,0x0};

    __cpuid(leaf0[0], leaf0[0], leaf0[1], leaf0[2], leaf0[3]);
    PDEBUG("0x0: %x,%x,%x,%x\n", leaf0[0], leaf0[1], leaf0[2], leaf0[3]);

    bool intel = (leaf0[1] == 0x756e6547) && (leaf0[2] == 0x6c65746e) && (leaf0[3] == 0x49656e69);
    PDEBUG("Intel? %s\n", intel ? "yes" : "no");

    /* This is not an accurate test for Skylake... */
    //bool skylake = (leaf0[1] & 0x16);
    //PDEBUG("Skylake uarch? %s\n", skylake ? "yes" : "no");

    return (intel);
}

bool is_skylake_server(void)
{
    /* leaf 1 - Model, Family, etc. */
    uint32_t leaf1[4]={0x1,0x0,0x0,0x0};

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

    PDEBUG("signature:  %#08x\n", (leaf1[0]) );
    //PDEBUG("stepping:   %#04x=%d\n", stepping, stepping);
    PDEBUG("model:      %#04x=%d\n", model, model);
    PDEBUG("family:     %#04x=%d\n", family, family);
    //PDEBUG("proc type:  %#04x=%d\n", proctype, proctype);
    PDEBUG("ext model:  %#04x=%d\n", xmodel, xmodel);
    //PDEBUG("ext family: %#08x=%d\n", xfamily, xfamily);

    bool skylake_server = (model == 0x55); /* 85 in binary */
    PDEBUG("Skylake server? %s\n", skylake_server ? "yes" : "no");

    return (skylake_server);
}

#ifdef SUPPORT_XEON_PHI
bool is_knl(void)
{
    /* leaf 7 - AVX-512 features */
    uint32_t leaf7[4]={0x7,0x0,0x0,0x0};

    __cpuid_count(leaf7[0], leaf7[2], leaf7[0], leaf7[1], leaf7[2], leaf7[3]);
    PDEBUG("0x7: %x,%x,%x,%x\n", leaf7[0], leaf7[1], leaf7[2], leaf7[3]);

    bool knl = (leaf7[1] & 1u<<16) && /* AVX-512F  */
               (leaf7[1] & 1u<<28) && /* AVX-512CD */
               (leaf7[1] & 1u<<26) && /* AVX-512PF */
               (leaf7[1] & 1u<<27);   /* AVX-512ER */
    PDEBUG("KNL uarch? %s\n", knl ? "yes" : "no");

    return (knl);
}

bool is_knm(void)
{
    /* leaf 7 - AVX-512 features */
    uint32_t leaf7[4]={0x7,0x0,0x0,0x0};

    __cpuid_count(leaf7[0], leaf7[2], leaf7[0], leaf7[1], leaf7[2], leaf7[3]);
    PDEBUG("0x7: %x,%x,%x,%x\n", leaf7[0], leaf7[1], leaf7[2], leaf7[3]);

    bool knl = (leaf7[1] & 1u<<16) && /* AVX-512F  */
               (leaf7[1] & 1u<<28) && /* AVX-512CD */
               (leaf7[1] & 1u<<26) && /* AVX-512PF */
               (leaf7[1] & 1u<<27);   /* AVX-512ER */

    /* KNM is a superset of KNL, at least architecturally */
    bool knm = knl && (leaf7[2] & 1u<<14); /* AVX-512VPOPCNTDQ */
    PDEBUG("KNM uarch? %s\n", knl ? "yes" : "no");

    return (knm);
}
#endif // SUPPORT_XEON_PHI

bool has_skx_avx512(void)
{
    /* leaf 7 - AVX-512 features */
    uint32_t leaf7[4]={0x7,0x0,0x0,0x0};

    __cpuid_count(leaf7[0], leaf7[2], leaf7[0], leaf7[1], leaf7[2], leaf7[3]);
    PDEBUG("0x7: %x,%x,%x,%x\n", leaf7[0], leaf7[1], leaf7[2], leaf7[3]);

    bool skx = (leaf7[1] & 1u<<16) && /* AVX-512F  */
               (leaf7[1] & 1u<<17) && /* AVX-512DQ */
               (leaf7[1] & 1u<<28) && /* AVX-512CD */
               (leaf7[1] & 1u<<30) && /* AVX-512BW */
               (leaf7[1] & 1u<<31);   /* AVX-512VL */

    return (skx);
}

bool has_avx512_vnni(void)
{
    /* leaf 7 - AVX-512 features */
    uint32_t leaf7[4]={0x7,0x0,0x0,0x0};

    __cpuid_count(leaf7[0], leaf7[2], leaf7[0], leaf7[1], leaf7[2], leaf7[3]);
    PDEBUG("0x7: %x,%x,%x,%x\n", leaf7[0], leaf7[1], leaf7[2], leaf7[3]);

    bool vnni = (leaf7[2] & 1u<<11); /* AVX-512VNNI */

    return (vnni);
}

bool has_avx512_vpopcntdq(void)
{
    /* leaf 7 - AVX-512 features */
    uint32_t leaf7[4]={0x7,0x0,0x0,0x0};

    __cpuid_count(leaf7[0], leaf7[2], leaf7[0], leaf7[1], leaf7[2], leaf7[3]);
    PDEBUG("0x7: %x,%x,%x,%x\n", leaf7[0], leaf7[1], leaf7[2], leaf7[3]);

    bool vpopcntdq = (leaf7[2] & 1u<<14); /* AVX-512VPOPCNTDQ */

    return (vpopcntdq);
}

bool has_avx512_bf16(void)
{
    /* based on https://software.intel.com/sites/default/files/managed/c5/15/architecture-instruction-set-extensions-programming-reference.pdf */

    /* leaf 7 - AVX-512 features */
    uint32_t leaf7[4]={0x7,0x0,0x1,0x0};

    __cpuid_count(leaf7[0], leaf7[2], leaf7[0], leaf7[1], leaf7[2], leaf7[3]);
    PDEBUG("0x7010: %x,%x,%x,%x\n", leaf7[0], leaf7[1], leaf7[2], leaf7[3]);

    bool bf16 = (leaf7[0] & 1u<<5); /* AVX-512BF16 */

    return (bf16);
}

int vpu_count(void)
{
    /* We can either detect Skylake AVX-512 directly from leaf1 or detect it
     * via Skylake arch from leaf0 plus AVX-512 features from leaf7.
     * We are going to do all the checks. */
    /* Note that Skylake, Cascade Lake and Cooper Lake all share the same
     * CPUID bits for model and extended model. */
    bool skylake_avx512 = is_intel() && is_skylake_server() && has_skx_avx512();

    if (skylake_avx512) {
        char cpu_name[32] = {0};
        get_cpu_name32(cpu_name);

        PDEBUG("Skylake AVX-512 detected\n");
        PDEBUG("cpu_name = %s\n", cpu_name);
        PDEBUG("cpu_name[9] = %c\n", cpu_name[9]);
        PDEBUG("cpu_name[17] = %c\n", cpu_name[17]);

        /* Skylake-X series: "Intel(R) Core (TM)..." */
        /* Cascade Lake-X series: "Intel(R) Core (TM)..." */
        if (cpu_name[9] == 'C') {
            return 2;
        }
        else if (cpu_name[9] == 'X') {
            /* Xeon Scalable series: "Intel(R) Xeon(R) Platinum..." */
            if (cpu_name[17] == 'P') {
                return 2;
            }
            /* Xeon Scalable series: "Intel(R) Xeon(R) Gold..." */
            else if (cpu_name[17] == 'G') {
                /* 61xx */
                if (cpu_name[22] == '6') {
                    return 2;
                /* 5122 */
                } else if (cpu_name[22] == '5' && cpu_name[24] == '2' && cpu_name[25] == '2') {
                    return 2;
                /* 51xx */
                } else {
                    return 1;
                }
            }
            /* Xeon Scalable series: "Intel(R) Xeon(R) Silver..." */
            else if (cpu_name[17] == 'S') {
                return 1;
            }
            /* Xeon Scalable series: "Intel(R) Xeon(R) Bronze..." */
            else if (cpu_name[17] == 'B') {
                return 1;
            }
            /* Xeon W series: "Intel(R) Xeon(R) W..." */
            else if (cpu_name[17] == 'W') {
                /* 2102 or 2104 are Skylake X w/ 1 FMA */
                if (cpu_name[21] == '0') {
                    return 1;
                }
                /* 212x and higher are Skylake-X w/ 2 FMAs */
                /* 22xx and 32xx are Cascade Lake-X w/ 2 FMAs */
                else {
                    return 2;
                }
            }
            /* Xeon D series: "Intel(R) Xeon(R) D-2xxx..." */
            else if (cpu_name[17] == 'D') {
                /* 21xx series is Skylake-X */
                if (cpu_name[19] == '2' && cpu_name[20] == '1') {
                    return 1;
                }
            }
        }
        /* Pre-production parts: Genuine Intel(R) CPU 0000 */
        else if (cpu_name[0] == 'G' && cpu_name[21] == '0' && cpu_name[22] == '0' && cpu_name[23] == '0' && cpu_name[24] == '0') {
            return 2;
        }
        /* Default to 2 FMAs and hope for the best... */
        else {
            char cpu_name[48] = {0};
            get_cpu_name48(cpu_name);
            fprintf(stderr,"UNSUPPORTED SKU! cpu_name = %s\n", cpu_name);
            return 2;
        }
    }
#ifdef SUPPORT_XEON_PHI
    else if ( is_knl() || is_knm() ) {
        return 2;
    }
#endif
#ifdef DEBUG
    else {
        char cpu_name[48] = {0};
        get_cpu_name48(cpu_name);
        fprintf(stderr,"UNSUPPORTED CPU! cpu_name = %s\n", cpu_name);
        return 0;
    }
#endif
    return 0;
}
