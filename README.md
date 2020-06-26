# Usage

```sh
make
./test.x
```

Debug printing is on by default.  That is because this is an education project.  If you want to use this code in product, please modify appropriately.  Without debug printing, the cost of calling `vpu_count()` may be as low as 2.2 microseconds.

The program `empirical.x` is not recommended but exists in the repository for historical reasons.

# Summary

The intended user of this project is the developer of a numerical library that needs to know the throughput of floating-point FMA (fused multiply-add) on Intel® processors that support AVX-512.  *This project does not attempt to address the throughput of other operations, such as shuffles, permutations, or non-floating-point instructions.*

The project provides example code to show a user how to determine the number of AVX-512 FMAs in Intel® Xeon® Scalable processors.  We encourage potential users to adapt the code to their usage, as some of the code included is rather pedantic (although this doesn't add significant runtime overhead unless debug printing is enabled).

Relevant processors include:
- Intel® Xeon® Scalable Processors
- Intel® Xeon® W Processors
- Intel® Xeon® D Processors
- Intel® Core™ X-series Processors

Intel® Xeon Phi™ processors are not covered here.

# Processors

## Intel® Xeon® Scalable Processors (Skylake)

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

## 2nd Generation Intel® Xeon® Scalable Processors (Cascade Lake)

This information is available from https://ark.intel.com/content/www/us/en/ark/products/series/192283/2nd-generation-intel-xeon-scalable-processors.html.

| Processor Model Name | Number of AVX-512 FMAs |
|----------------------|----------------|
| Intel® Xeon® Platinum 9282 Processor | 2 |
| Intel® Xeon® Platinum 9242 Processor | 2 |
| Intel® Xeon® Platinum 8280 Processor | 2 |
| Intel® Xeon® Platinum 8276 Processor | 2 |
| Intel® Xeon® Platinum 8270 Processor | 2 |
| Intel® Xeon® Platinum 8268 Processor | 2 |
| Intel® Xeon® Platinum 8260 Processor | 2 |
| Intel® Xeon® Platinum 8256 Processor | 2 |
| Intel® Xeon® Platinum 8253 Processor | 2 |
| Intel® Xeon® Gold 6254 Processor     | 2 |
| Intel® Xeon® Gold 6252 Processor     | 2 |
| Intel® Xeon® Gold 6248 Processor     | 2 |
| Intel® Xeon® Gold 6246 Processor     | 2 |
| Intel® Xeon® Gold 6244 Processor     | 2 |
| Intel® Xeon® Gold 6242 Processor     | 2 |
| Intel® Xeon® Gold 6240 Processor     | 2 |
| Intel® Xeon® Gold 6238 Processor     | 2 |
| Intel® Xeon® Gold 6234 Processor     | 2 |
| Intel® Xeon® Gold 6230 Processor     | 2 |
| Intel® Xeon® Gold 6226 Processor     | 2 |
| Intel® Xeon® Gold 5222 Processor     | 2 |
| Intel® Xeon® Gold 5220 Processor     | 1 |
| Intel® Xeon® Gold 5218 Processor     | 1 |
| Intel® Xeon® Gold 5217 Processor     | 1 |
| Intel® Xeon® Gold 5215 Processor     | 1 |
| Intel® Xeon® Silver 4216 Processor   | 1 |
| Intel® Xeon® Silver 4215 Processor   | 1 |
| Intel® Xeon® Silver 4214 Processor   | 1 |
| Intel® Xeon® Silver 4210 Processor   | 1 |
| Intel® Xeon® Silver 4208 Processor   | 1 |
| Intel® Xeon® Bronze 3204 Processor   | 1 |

# 3rd Generation Intel® Xeon® Scalable Processors (Cooper Lake)

This information is available from https://ark.intel.com/content/www/us/en/ark/products/series/204098/3rd-generation-intel-xeon-scalable-processors.html.

| Processor Model Name | Number of AVX-512 FMAs |
|----------------------|----------------|
| Intel® Xeon® Platinum 8380HL Processor  | 2 |
| Intel® Xeon® Platinum 8380H Processor   | 2 |
| Intel® Xeon® Platinum 8376HL Processor  | 2 |
| Intel® Xeon® Platinum 8376H Processor   | 2 |
| Intel® Xeon® Platinum 8354H Processor   | 2 |
| Intel® Xeon® Platinum 8353H Processor   | 2 |
| Intel® Xeon® Gold 6348H Processor       | 2 |
| Intel® Xeon® Gold 6328HL Processor      | 2 |
| Intel® Xeon® Gold 6328H Processor       | 2 |
| Intel® Xeon® Gold 5320H Processor       | 2 |
| Intel® Xeon® Gold 5318H Processor       | 2 |

## Intel® Xeon® W Processors

This information is available from https://ark.intel.com/products/series/125035/Intel-Xeon-Processor-W-Family.

### Skylake

| Processor Model Name | Number of AVX-512 FMAs |
|----------------------|----------------|
|Intel® Xeon® W-2195 Processor | 2 |
|Intel® Xeon® W-2155 Processor | 2 |
|Intel® Xeon® W-2145 Processor | 2 |
|Intel® Xeon® W-2135 Processor | 2 |
|Intel® Xeon® W-2133 Processor | 2 |
|Intel® Xeon® W-2125 Processor | 2 |
|Intel® Xeon® W-2123 Processor | 2 |

### Cascade Lake

| Processor Model Name | Number of AVX-512 FMAs |
|----------------------|----------------|
|Intel® Xeon® W-3235 Processor  | 2 |
|Intel® Xeon® W-3265M Processor | 2 |
|Intel® Xeon® W-3245M Processor | 2 |
|Intel® Xeon® W-3275 Processor  | 2 |
|Intel® Xeon® W-3245 Processor  | 2 |
|Intel® Xeon® W-3275M Processor | 2 |
|Intel® Xeon® W-3223 Processor  | 2 |
|Intel® Xeon® W-3265 Processor  | 2 |
|Intel® Xeon® W-3225 Processor  | 2 |
|Intel® Xeon® W-2275 Processor  | 2 |
|Intel® Xeon® W-2295 Processor  | 2 |
|Intel® Xeon® W-2265 Processor  | 2 |
|Intel® Xeon® W-2255 Processor  | 2 |
|Intel® Xeon® W-2223 Processor  | 2 |
|Intel® Xeon® W-2245 Processor  | 2 |
|Intel® Xeon® W-2225 Processor  | 2 |
|Intel® Xeon® W-2235 Processor  | 2 |

## Intel® Xeon® D Processors

This information should be available from https://ark.intel.com/content/www/us/en/ark/products/series/87041/intel-xeon-d-processor.html.

There is some ambiguity here.  See https://www.servethehome.com/intel-xeon-d-2183it-benchmarks-and-review-16c-soc/2/.

## Intel® Core™ X-series Processors

This information is available from https://ark.intel.com/products/series/123588/Intel-Core-X-series-Processors.

### [Skylake](https://ark.intel.com/compare/126699,126697,126240,126695,123613,123767,123589)

| Processor Model Name | Number of AVX-512 FMAs |
|----------------------|----------------|
| Intel® Core™ i9-7980XE Extreme Edition Processor  | 2 |
| Intel® Core™ i9-7960X X-series Processor          | 2 |
| Intel® Core™ i9-7940X X-series Processor          | 2 |
| Intel® Core™ i9-7920X X-series Processor          | 2 |
| Intel® Core™ i9-7900X X-series Processor          | 2 |
| Intel® Core™ i7-7820X X-series Processor          | 2 |
| Intel® Core™ i7-7800X X-series Processor          | 2 |

### [Cascade Lake](https://ark.intel.com/compare/198017,198019,198012,198014)

| Processor Model Name | Number of AVX-512 FMAs |
|----------------------|----------------|
| Intel® Core™ i9-10980XE Extreme Edition Processor | 2 |
| Intel® Core™ i9-10900X X-series Processor         | 2 |
| Intel® Core™ i9-10920X X-series Processor         | 2 |
| Intel® Core™ i9-10940X X-series Processor         | 2 |

## Intel® Core™ U/Y-series Processors

### [Ice Lake](https://ark.intel.com/compare/196597,197120,196591,196592,196603,197119,197121,196588,197122,197123)

From https://software.intel.com/sites/default/files/managed/9e/bc/64-ia-32-architectures-optimization-manual.pdf, Section 2.1.1.5:
> All processors based on Ice Lake Client microarchitecture contain a single 512-bit FMA unit...


