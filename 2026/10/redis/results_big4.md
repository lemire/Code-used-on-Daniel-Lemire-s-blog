
## dau_0.1pct: 0.1% of 100M user ids, random  (bitcounts a, b, a AND b = [100000, 100000, 105])
| operation | bitset | roaring | module |
| --- | ---: | ---: | ---: |
| SETBIT (build) | 0.23 us | 0.22 us | 0.25 us |
| memory (bytes) | 12,582,950.00 | 343,967.00 | 212,257.00 |
| GETBIT | 0.06 us | 0.15 us | 0.23 us |
| SETBIT (update) | 0.10 us | 0.38 us | 0.27 us |
| BITCOUNT | 350.10 us | 11.74 us | 1.43 us |
| BITCOUNT range | 25.09 us | 2.95 us | N/A |
| BITPOS | 0.18 us | 0.20 us | N/A |
| BITFIELD_RO GET u16 | 0.13 us | 0.31 us | N/A |
| BITOP AND | 5,486.61 us | 1,294.65 us | 316.34 us |
| BITOP OR | 5,357.00 us | 1,249.65 us | 757.11 us |
| BITOP XOR | 5,357.97 us | 1,310.57 us | 790.20 us |

## dau_1pct: 1% of 100M user ids, random  (bitcounts a, b, a AND b = [1000000, 1000000, 10001])
| operation | bitset | roaring | module |
| --- | ---: | ---: | ---: |
| SETBIT (build) | 0.10 us | 0.25 us | 0.28 us |
| memory (bytes) | 14,680,100.00 | 2,442,781.00 | 2,012,257.00 |
| GETBIT | 0.06 us | 0.26 us | 0.26 us |
| SETBIT (update) | 0.09 us | 0.43 us | 0.31 us |
| BITCOUNT | 350.35 us | 11.81 us | 1.47 us |
| BITCOUNT range | 25.09 us | 2.97 us | N/A |
| BITPOS | 0.17 us | 0.24 us | N/A |
| BITFIELD_RO GET u16 | 0.14 us | 0.45 us | N/A |
| BITOP AND | 5,585.28 us | 8,281.80 us | 1,963.21 us |
| BITOP OR | 5,418.28 us | 10,889.20 us | 8,956.59 us |
| BITOP XOR | 5,504.39 us | 11,159.07 us | 8,362.94 us |

## dau_10pct: 10% of 100M user ids, random  (bitcounts a, b, a AND b = [10000000, 10000000, 1000126])
| operation | bitset | roaring | module |
| --- | ---: | ---: | ---: |
| memory (bytes) | 12,582,949.00 | 12,577,022.00 | 12,513,249.00 |
| GETBIT | 0.07 us | 0.10 us | 0.24 us |
| SETBIT (update) | 0.09 us | 0.28 us | 0.22 us |
| BITCOUNT | 361.32 us | 11.79 us | 2.41 us |
| BITCOUNT range | 26.00 us | 3.12 us | N/A |
| BITPOS | 0.15 us | 0.15 us | N/A |
| BITFIELD_RO GET u16 | 0.15 us | 0.23 us | N/A |
| BITOP AND | 5,296.95 us | 13,043.50 us | 11,756.13 us |
| BITOP OR | 5,360.10 us | 3,244.30 us | 3,883.23 us |
| BITOP XOR | 5,529.09 us | 3,288.27 us | 3,831.60 us |

## dense_50pct: 50% of 100M user ids, random  (bitcounts a, b, a AND b = [50000000, 50000000, 25000487])
| operation | bitset | roaring | module |
| --- | ---: | ---: | ---: |
| memory (bytes) | 12,582,951.00 | 12,577,024.00 | 12,513,249.00 |
| GETBIT | 0.11 us | 0.15 us | 0.27 us |
| SETBIT (update) | 0.10 us | 0.28 us | 0.23 us |
| BITCOUNT | 350.62 us | 11.81 us | 2.62 us |
| BITCOUNT range | 25.15 us | 3.14 us | N/A |
| BITPOS | 0.13 us | 0.14 us | N/A |
| BITFIELD_RO GET u16 | 0.16 us | 0.21 us | N/A |
| BITOP AND | 5,489.03 us | 3,556.68 us | 4,081.73 us |
| BITOP OR | 5,498.26 us | 3,390.77 us | 3,853.61 us |
| BITOP XOR | 5,398.07 us | 3,189.46 us | 3,915.32 us |

## hashed_2e32: 100k random 32-bit ids  (bitcounts a, b, a AND b = [100000, 100000, 4])
| operation | bitset | roaring | module |
| --- | ---: | ---: | ---: |
| SETBIT (build) | 5.11 us | 0.25 us | 1.51 us |
| memory (bytes) | 536,870,951.00 | 4,178,992.00 | 400,045.00 |
| GETBIT | 0.07 us | 0.16 us | 0.26 us |
| SETBIT (update) | 0.10 us | 0.35 us | 1.36 us |
| BITCOUNT | 27,902.35 us | 852.25 us | 47.08 us |
| BITCOUNT range | 1,537.40 us | 100.55 us | N/A |
| BITPOS | 0.85 us | 0.24 us | N/A |
| BITFIELD_RO GET u16 | 0.24 us | 0.25 us | N/A |
| BITOP AND | 342,531.19 us | 12,535.40 us | 6,015.61 us |
| BITOP OR | 345,397.00 us | 17,929.77 us | 10,255.71 us |
| BITOP XOR | 343,455.41 us | 16,232.47 us | 10,496.50 us |

## runs: 10 runs of 1M ids in 100M  (bitcounts a, b, a AND b = [10000000, 10000000, 10000000])
| operation | bitset | roaring | module |
| --- | ---: | ---: | ---: |
| memory (bytes) | 12,582,944.00 | 12,649.00 | 2,326.00 |
| GETBIT | 0.07 us | 0.09 us | 0.15 us |
| SETBIT (update) | 0.10 us | 0.40 us | 0.21 us |
| BITCOUNT | 318.92 us | 2.20 us | 0.83 us |
| BITCOUNT range | 22.87 us | 0.53 us | N/A |
| BITPOS | 20.55 us | 0.17 us | N/A |
| BITFIELD_RO GET u16 | 0.15 us | 0.14 us | N/A |
| BITOP AND | 4,803.93 us | 30.06 us | 20.48 us |
| BITOP OR | 4,781.13 us | 26.06 us | 21.43 us |
| BITOP XOR | 4,814.02 us | 28.02 us | 18.53 us |
