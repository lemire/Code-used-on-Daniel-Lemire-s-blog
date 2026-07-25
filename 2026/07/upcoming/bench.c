#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static double now_ns(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec * 1e9 + t.tv_nsec;
}

// volatile must qualify the pointer itself, not the pointee, or the store is
// dead and the (pure) strstr call gets optimized away.
static const void *volatile sink;

// strstr is declared pure, so a loop-invariant call gets hoisted out of the
// timing loop. Launder the pointer so the compiler cannot prove invariance.
static inline const char *launder(const char *p) {
#if defined(__GNUC__) || defined(__clang__)
    __asm__ volatile("" : "+r"(p) : : "memory");
#endif
    return p;
}

int main(void) {
    // glibc strstr is quadratic on this input: one call costs ~n*m. Keep n and
    // max_m small or the benchmark never finishes.
    const size_t n = 512ULL << 10;          // 512 KiB
    const size_t max_m = 4ULL << 10;        // 4 KiB
    char *hay = malloc(n + 1);
    char *nee = malloc(max_m + 1);
    if (!hay || !nee) {
        free(hay);
        free(nee);
        return EXIT_FAILURE;
    }

    setvbuf(stdout, NULL, _IOLBF, 0);       // stream rows even when redirected

    printf("worst-case strstr  n=%zu\n", n);
    printf("%8s %14s %10s\n", "m", "ns", "GB/s");

    for (size_t m = 128; m <= max_m; m *= 2) {
        // haystack = (a^{m-1} b)*
        for (size_t i = 0; i < n; i++)
            hay[i] = (i % m == m - 1) ? 'b' : 'a';
        hay[n] = 0;

        // needle = a^m, which never occurs in the haystack
        memset(nee, 'a', m);
        nee[m] = 0;

        // cheap cases (small m) get more reps; the quadratic ones need few
        int reps = m <= 512 ? 5 : 2;

        // warm-up
        sink = strstr(launder(hay), launder(nee));

        double t0 = now_ns();
        for (int r = 0; r < reps; r++)
            sink = strstr(launder(hay), launder(nee));
        double t1 = now_ns();

        double ns = (t1 - t0) / reps;
        if (ns < 1.0) ns = 1.0;           // never report 0

        printf("%8zu %14.1f %10.3f\n",
               m, ns, n / ns);
    }

    free(hay);
    free(nee);
    return EXIT_SUCCESS;
}
