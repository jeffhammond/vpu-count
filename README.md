# Summary

The intended user of this project is the developer of a numerical library that needs to know the throughput of floating-point FMA (fused multiply-add) on Xeon processors that support AVX-512.  *This project does not attempt to address the throughput of other operations, such as shuffles, permutations, or other non-floating-point instructions.*

The project provides example code to show a user how to determine the number of AVX-512 FMAs in an Intel processor based on the Skylake microarchitecture.  We encourage potential users to adapt the code to their usage, as some of the code included is rather pedantic (although this doesn't add significant runtime overhead unless debug printing is enabled).

Relevant processors include:
- Intel® Xeon® Scalable Processors
- Intel® Xeon® W Processors
- Intel® Core™ X-series Processors

Intel® Xeon Phi™ processors are not covered here.

# Processors

## Intel® Xeon® Scalable Processors

This information is available from https://ark.intel.com/products/series/125191/Intel-Xeon-Scalable-Processors.

| Processor Model Name | Number of AVX-512 FMAs |
|----------------------|----------------|
| Intel® Xeon® Platinum 8180 Processor | 2 |
| Intel® Xeon® Platinum 8176 Processor | 2 |
| Intel® Xeon® Platinum 8170 Processor | 2 |
| Intel® Xeon® Platinum 8168 Processor | 2 |
| Intel® Xeon® Platinum 8164 Processor | 2 |
| Intel® Xeon® Platinum 8160 Processor | 2 |
| Intel® Xeon® Platinum 8158 Processor | 2 |
| Intel® Xeon® Platinum 8156 Processor | 2 |
| Intel® Xeon® Platinum 8153 Processor | 2 |
| Intel® Xeon® Gold 6154 Processor | 2 |
| Intel® Xeon® Gold 6152 Processor | 2 |
| Intel® Xeon® Gold 6150 Processor | 2 |
| Intel® Xeon® Gold 6148 Processor | 2 |
| Intel® Xeon® Gold 6146 Processor | 2 |
| Intel® Xeon® Gold 6144 Processor | 2 |
| Intel® Xeon® Gold 6142 Processor | 2 |
| Intel® Xeon® Gold 6140 Processor | 2 |
| Intel® Xeon® Gold 6138 Processor | 2 |
| Intel® Xeon® Gold 6136 Processor | 2 |
| Intel® Xeon® Gold 6134 Processor | 2 |
| Intel® Xeon® Gold 6132 Processor | 2 |
| Intel® Xeon® Gold 6130 Processor | 2 |
| Intel® Xeon® Gold 6128 Processor | 2 |
| Intel® Xeon® Gold 6126 Processor | 2 |
| Intel® Xeon® Gold 5122 Processor | 2 |
| Intel® Xeon® Gold 5120 Processor | 1 |
| Intel® Xeon® Gold 5119 Processor | 1 |
| Intel® Xeon® Gold 5118 Processor | 1 |
| Intel® Xeon® Gold 5115 Processor | 1 |
| Intel® Xeon® Silver 4116 Processor | 1 |
| Intel® Xeon® Silver 4114 Processor | 1 |
| Intel® Xeon® Silver 4112 Processor | 1 |
| Intel® Xeon® Silver 4110 Processor | 1 |
| Intel® Xeon® Silver 4109 Processor | 1 |
| Intel® Xeon® Silver 4108 Processor | 1 |
| Intel® Xeon® Bronze 3106 Processor | 1 |
| Intel® Xeon® Bronze 3104 Processor | 1 |

## Intel® Xeon® W Processors

This information is available from https://ark.intel.com/products/series/125035/Intel-Xeon-Processor-W-Family.

| Processor Model Name | Number of AVX-512 FMAs |
|----------------------|----------------|
|Intel® Xeon® W-2195 Processor | 2 |
|Intel® Xeon® W-2155 Processor | 2 |
|Intel® Xeon® W-2145 Processor | 2 |
|Intel® Xeon® W-2135 Processor | 2 |
|Intel® Xeon® W-2133 Processor | 2 |
|Intel® Xeon® W-2125 Processor | 2 |
|Intel® Xeon® W-2123 Processor | 2 |

## Intel® Core™ X-series Processors

This information is available from https://ark.intel.com/products/series/123588/Intel-Core-X-series-Processors ([relevant SKUs](https://ark.intel.com/compare/126699,126697,126240,126695,123613,123767,123589))

| Processor Model Name | Number of AVX-512 FMAs |
|----------------------|----------------|
| Intel® Core™ i9-7980XE Extreme Edition Processor | 2 |
| Intel® Core™ i9-7960X X-series Processor         | 2 |
| Intel® Core™ i9-7940X X-series Processor         | 2 |
| Intel® Core™ i9-7920X X-series Processor         | 2 |
| Intel® Core™ i9-7900X X-series Processor         | 2 |
| Intel® Core™ i7-7820X X-series Processor         | 2 |
| Intel® Core™ i7-7800X X-series Processor         | 2 |
