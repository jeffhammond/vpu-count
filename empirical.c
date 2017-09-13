#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <immintrin.h>

#include <omp.h>

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
    __declspec(align(64)) int shuf_vec[16] = {0, 1, 2, 3,4, 5, 6, 7,8, 9, 10, 11,12, 13, 14, 15};

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

int main(void)
{
    double t0 = omp_get_wtime();

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
    printf("%llu FMA server\n", number_of_fma_units_per_core);
    double t1 = omp_get_wtime();
    printf("%f us, %f us\n", 1.e6*(t1-t0), 1.e6*(t1-t0)/1000.);
    return 0;
}
