// This code was extracted from https://software.intel.com/sites/default/files/managed/9e/bc/64-ia-32-architectures-optimization-manual.pdf,
// which contains the following statement:
//
// Intel technologies features and benefits depend on system configuration and may require enabled hardware, software, or service activation.
// Learn more at intel.com, or from the OEM or retailer.
//
// No computer system can be absolutely secure. Intel does not assume any liability for lost or stolen data or systems or any damages
// resulting from such losses.
//
// You may not use or facilitate the use of this document in connection with any infringement or other legal analysis concerning Intel
// products described herein. You agree to grant Intel a non-exclusive, royalty-free license to any patent claim thereafter drafted which
// includes subject matter disclosed herein.
//
// No license (express or implied, by estoppel or otherwise) to any intellectual property rights is granted by this document.
// The products described may contain design defects or errors known as errata which may cause the product to deviate from published
// specifications. Current characterized errata are available on request.
//
// This document contains information on products, services and/or processes in development. All information provided here is subject
// to change without notice. Contact your Intel representative to obtain the latest Intel product specifications and roadmaps.
//
// Results have been estimated or simulated using internal Intel analysis or architecture simulation or modeling, and provided to you for
// informational purposes. Any differences in your system hardware, software or configuration may affect your actual performance.
//
// Copies of documents which have an order number and are referenced in this document, or other Intel literature, may be obtained
// by calling 1-800-548-4725, or by visiting http://www.intel.com/design/literature.htm.
//
// Intel, the Intel logo, Intel Atom, Intel Core, Intel SpeedStep, MMX, Pentium, VTune, and Xeon are trademarks of Intel Corporation
// in the U.S. and/or other countries.
//
// *Other names and brands may be claimed as the property of others.
//
// Copyright © 1997-2017, Intel Corporation. All Rights Reserved

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <immintrin.h>

#if 1
#include <omp.h>
#else
#include <mpi.h>
#endif

static uint64_t rdtsc(void)
{
    unsigned int ax, dx;
    __asm__ __volatile__ ("rdtsc" : "=a"(ax), "=d"(dx));
    return ((((uint64_t)dx) << 32) | ax);
}

uint64_t fma_shuffle_tpt(uint64_t loop_cnt)
{
    uint64_t loops = loop_cnt;
    __declspec(align(64)) double one_vec[8] = {1, 1, 1, 1,1, 1, 1, 1};
    __declspec(align(64)) int shuf_vec[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

    __asm {
        vmovups zmm0, [one_vec]
        vmovups zmm1, [one_vec]
        vmovups zmm2, [one_vec]
        vmovups zmm3, [one_vec]
        vmovups zmm4, [one_vec]
        vmovups zmm5, [one_vec]
        vmovups zmm6, [one_vec]
        vmovups zmm7, [one_vec]
        vmovups zmm8, [one_vec]
        vmovups zmm9, [one_vec]
        vmovups zmm10, [one_vec]
        vmovups zmm11, [one_vec]
        vmovups zmm12, [shuf_vec]
        vmovups zmm13, [shuf_vec]
        vmovups zmm14, [shuf_vec]
        vmovups zmm15, [shuf_vec]
        vmovups zmm16, [shuf_vec]
        vmovups zmm17, [shuf_vec]
        vmovups zmm18, [shuf_vec]
        vmovups zmm19, [shuf_vec]
        vmovups zmm20, [shuf_vec]
        vmovups zmm21, [shuf_vec]
        vmovups zmm22, [shuf_vec]
        vmovups zmm23, [shuf_vec]
        vmovups zmm30, [shuf_vec]
        mov rdx, loops
    loop1:
        vfmadd231pd zmm0, zmm0, zmm0
        vfmadd231pd zmm1, zmm1, zmm1
        vfmadd231pd zmm2, zmm2, zmm2
        vfmadd231pd zmm3, zmm3, zmm3
        vfmadd231pd zmm4, zmm4, zmm4
        vfmadd231pd zmm5, zmm5, zmm5
        vfmadd231pd zmm6, zmm6, zmm6
        vfmadd231pd zmm7, zmm7, zmm7
        vfmadd231pd zmm8, zmm8, zmm8
        vfmadd231pd zmm9, zmm9, zmm9
        vfmadd231pd zmm10, zmm10, zmm10
        vfmadd231pd zmm11, zmm11, zmm11
        vpermd zmm12, zmm30, zmm30
        vpermd zmm13, zmm30, zmm30
        vpermd zmm14, zmm30, zmm30
        vpermd zmm15, zmm30, zmm30
        vpermd zmm16, zmm30, zmm30
        vpermd zmm17, zmm30, zmm30
        vpermd zmm18, zmm30, zmm30
        vpermd zmm19, zmm30, zmm30
        vpermd zmm20, zmm30, zmm30
        vpermd zmm21, zmm30, zmm30
        vpermd zmm22, zmm30, zmm30
        vpermd zmm23, zmm30, zmm30
        dec rdx
        jg loop1
    }
}

uint64_t fma_only_tpt(int loop_cnt) {
    uint64_t loops = loop_cnt;
    __declspec(align(64)) double one_vec[8] = {1, 1, 1, 1, 1, 1, 1, 1};
    __asm {
        vmovups zmm0, [one_vec]
        vmovups zmm1, [one_vec]
        vmovups zmm2, [one_vec]
        vmovups zmm3, [one_vec]
        vmovups zmm4, [one_vec]
        vmovups zmm5, [one_vec]
        vmovups zmm6, [one_vec]
        vmovups zmm7, [one_vec]
        vmovups zmm8, [one_vec]
        vmovups zmm9, [one_vec]
        vmovups zmm10, [one_vec]
        vmovups zmm11, [one_vec]
        mov rdx, loops
    loop1:
        vfmadd231pd zmm0, zmm0, zmm0
        vfmadd231pd zmm1, zmm1, zmm1
        vfmadd231pd zmm2, zmm2, zmm2
        vfmadd231pd zmm3, zmm3, zmm3
        vfmadd231pd zmm4, zmm4, zmm4
        vfmadd231pd zmm5, zmm5, zmm5
        vfmadd231pd zmm6, zmm6, zmm6
        vfmadd231pd zmm7, zmm7, zmm7
        vfmadd231pd zmm8, zmm8, zmm8
        vfmadd231pd zmm9, zmm9, zmm9
        vfmadd231pd zmm10, zmm10, zmm10
        vfmadd231pd zmm11, zmm11, zmm11
        dec rdx
        jg loop1
    }
}

int measure(void)
{
    //double t0 = omp_get_wtime();

    uint64_t number_of_fma_units_per_core = 2;

    for (int r=0; r<1000; r++) {
        uint64_t fma_shuf_tpt_test[3];
        uint64_t fma_shuf_tpt_test_min;
        uint64_t fma_only_tpt_test[3];
        uint64_t fma_only_tpt_test_min;
        uint64_t start = 0;
        //uint64_t number_of_fma_units_per_core = 2;

        /*********************************************************/
        /* Step 1: Warmup to get the AVX-512 license */
        /*********************************************************/
        fma_only_tpt(100000);
        /*********************************************************/
        /* Step 2: Execute FMA and Shuffle TPT Test */
        /*********************************************************/
        for(int i = 0; i < 3; i++){
            start = rdtsc();
            fma_shuffle_tpt(1000);
            fma_shuf_tpt_test[i] = rdtsc() - start;
        }
        /*********************************************************/
        /* Step 3: Execute FMA only TPT Test */
        /*********************************************************/
        for(int i = 0; i < 3; i++){
            start = rdtsc();
            fma_only_tpt(1000);
            fma_only_tpt_test[i] = rdtsc() - start;
        }
        /*********************************************************/
        /* Step 4: Decide if 1 FMA server or 2 FMA server */
        /*********************************************************/
        fma_shuf_tpt_test_min = fma_shuf_tpt_test[0];
        fma_only_tpt_test_min = fma_only_tpt_test[0];
        for(int i = 1; i < 3; i++){
            if ((int)fma_shuf_tpt_test[i] < (int)fma_shuf_tpt_test_min) fma_shuf_tpt_test_min = fma_shuf_tpt_test[i];
            if ((int)fma_only_tpt_test[i] < (int)fma_only_tpt_test_min) fma_only_tpt_test_min = fma_only_tpt_test[i];
        }
        if (((double)fma_shuf_tpt_test_min/(double)fma_only_tpt_test_min) < 1.5) {
            number_of_fma_units_per_core = 1;
        }
        //printf("%llu FMA server\n", number_of_fma_units_per_core);
    }
    //printf("%llu FMA server\n", number_of_fma_units_per_core);
    //double t1 = omp_get_wtime();
    //printf("%f us, %f us\n", 1.e6*(t1-t0), 1.e6*(t1-t0)/1000.);
    //return 0;
    return number_of_fma_units_per_core;
}

int main(void)
{
#if 1
    #pragma omp parallel
    {
        int vpu = measure();
        #pragma omp critical
        {
            printf("vpu=%d\n", vpu);
        }
    }
#else
    MPI_Init(NULL,NULL);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Barrier(MPI_COMM_WORLD);
    int vpu = measure();
    MPI_Barrier(MPI_COMM_WORLD);
    printf("%d: vpu=%d\n", rank, vpu);
    MPI_Finalize();
#endif
    return 0;
}
