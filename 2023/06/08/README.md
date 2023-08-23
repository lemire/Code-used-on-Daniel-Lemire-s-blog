This assumes Linux/macOS and a processor with SSE 4.1 (virtually any x64 processor in operation today).

This works is based on the research by Mula. (Mula is Polish and his name cannot be coded in ASCII. I transliterate his name deliberately.) 

Wojciech Mula, SIMD-ized faster parse of IPv4 addresses, 2023-04-09
http://0x80.pl/notesen/2023-04-09-faster-parse-ipv4.html

In this article, Mula describes several SIMD-based algorithm. Our approach is similar with the following differences:

1. Mula distinguishes between the case where we have no 3-digit numbers, or no 2-digit numbers. We have a single case (less branching). We don't expect that this optimization, by Mula, is very useful in practice.
2. Mula has a distinct step to check that the input is all digits and dots. We integrate this check in the main computation.
3. Mula relies on two different hashing functions, an expensive one that reduces memory usage, and a fast one that leads to more memory usage. We opted for a difference choice that is nearly as compact as the slow Mula hashing and nearly as fast as the fastest Mula routine.
4. We validate by comparing the expected length of the address (which we get from the mask) instead of Mula's approach of validating by store the dot mask.

Overall, we get a performance that is similar or better than the fastest Mula's scheme, while using relatively little memory.

We benchmark against inet_pton, with the IPv4 flag, which is documented as follows:

> src points to a character string containing an IPv4 network address in
> dotted-decimal format, “ddd.ddd.ddd.ddd”, where ddd is a decimal
> number of up to three digits in the range 0 to 255.