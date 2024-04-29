#include <iostream>
#include <chrono>

// clang++ apple-silicon-ldp-repro.cpp -O2 -o ldp && ./ldp


__attribute__((noinline))
void Stall(int* ptr)
{
    for (int i=0; i<1000000000; i++)
    {
        int tmp1, tmp2;
        __asm__ volatile(
            "ldr %w0, [%2]\n"
            "ldr %w1, [%2, #4]\n"
            "str %w0, [%2]\n"
            "str %w1, [%2, #4]\n"
            // NOTE: stp also ruins perf
            : "=&r" (tmp1), "=&r" (tmp2)
            : "r" (ptr)
            : 
        );
    }
}


__attribute__((noinline))
void StallNoStr(int* ptr)
{
    for (int i=0; i<1000000000; i++)
    {
        int tmp1, tmp2;
        __asm__ volatile(
            "ldr %w0, [%2]\n"
            "ldr %w1, [%2, #4]\n"
            // NOTE: stp also ruins perf
            : "=&r" (tmp1), "=&r" (tmp2)
            : "r" (ptr)
            : 
        );
    }
}



__attribute__((noinline))
void StallLDP(int* ptr)
{
    for (int i=0; i<1000000000; i++)
    {
        int tmp1, tmp2;
        __asm__ volatile(
            "ldp %w0, %w1, [%2]\n"
            "str %w0, [%2]\n"
            "str %w1, [%2, #4]\n"
            // NOTE: stp also ruins perf
            : "=&r" (tmp1), "=&r" (tmp2)
            : "r" (ptr)
            : 
        );
    }
}


__attribute__((noinline))
void StallLDPNostr(int* ptr)
{
    for (int i=0; i<1000000000; i++)
    {
        int tmp1, tmp2;
        __asm__ volatile(
            "ldp %w0, %w1, [%2]\n"
            // NOTE: stp also ruins perf
            : "=&r" (tmp1), "=&r" (tmp2)
            : "r" (ptr)
            : 
        );
    }
}
int main()
{
    int data[64];
    for (int t = 0; t<3; t++)
    {
        using namespace std::chrono;
        printf("---\n");
        auto start = high_resolution_clock::now();
        Stall(data);
        auto stop = high_resolution_clock::now();
        double elapsed = duration_cast<milliseconds>(stop - start).count() / 1000.0;
        printf("it took: %f ms.\n", elapsed);
        start = high_resolution_clock::now();
        StallNoStr(data);
        stop = high_resolution_clock::now();
        elapsed = duration_cast<milliseconds>(stop - start).count() / 1000.0;
        printf("nostr it took: %f ms.\n", elapsed);
        start = high_resolution_clock::now();
        StallLDP(data);
        stop = high_resolution_clock::now();
        elapsed = duration_cast<milliseconds>(stop - start).count() / 1000.0;
        printf("ldp it took: %f ms.\n", elapsed);
        start = high_resolution_clock::now();
        StallLDPNostr(data);
        stop = high_resolution_clock::now();
        elapsed = duration_cast<milliseconds>(stop - start).count() / 1000.0;
        printf("ldpnostr it took: %f ms.\n", elapsed);
    }
    return 0;
}
