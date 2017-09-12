#include <stdio.h>
#include <stdbool.h>

int vpu_count();

int main(void)
{
    int n = vpu_count();
    printf("CPU has %d AVX-512 VPUs\n", n);
    return 0;
}

