// Adversarial inputs for the macOS C library's strstr / memmem.
//
// Apple's libc inherits the FreeBSD strstr: scan for the needle's first byte,
// then call strncmp/memcmp on the whole needle. That is the naive
// filter-and-verify loop, with no linear-time fall-back. The filter is the
// FIRST byte, so the adversary makes every position pass the filter and every
// verification fail on the last byte:
//
//     haystack = "aaaa....a"        (n bytes)
//     needle   = "aaaa....ab"       (m bytes: m-1 'a' then one 'b')
//
// Every one of the n-m+1 alignments matches the first byte, and each
// verification compares m-1 bytes before failing. Total work is Theta(n*m).
//
// The control uses the mirror needle "b" + "a"*(m-1): the filter byte 'b'
// never occurs, so the scan is a single memchr-style pass, Theta(n).
//
// Build:  cc -O2 -fno-builtin -o adversarial adversarial.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static double now_seconds(void) {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (double)ts.tv_sec + 1e-9 * (double)ts.tv_nsec;
}

// Keeps the optimizer from deleting the search.
static volatile size_t sink;

typedef const char *(*searcher)(const char *haystack, size_t n,
                               const char *needle, size_t m);

static const char *call_strstr(const char *haystack, size_t n,
                               const char *needle, size_t m) {
  (void)n;
  (void)m;
  return strstr(haystack, needle);
}

static const char *call_memmem(const char *haystack, size_t n,
                               const char *needle, size_t m) {
  return (const char *)memmem(haystack, n, needle, m);
}

// Runs `f` enough times to spend at least min_seconds, returns nanoseconds per
// haystack byte for the fastest repetition.
static double measure(searcher f, const char *haystack, size_t n,
                      const char *needle, size_t m, double min_seconds) {
  double best = 1e300;
  double spent = 0.0;
  int rounds = 0;
  while (spent < min_seconds || rounds < 3) {
    double t0 = now_seconds();
    const char *r = f(haystack, n, needle, m);
    double elapsed = now_seconds() - t0;
    sink += (size_t)(r - haystack);
    if (elapsed < best) best = elapsed;
    spent += elapsed;
    rounds++;
    if (rounds > 1000) break;
  }
  return best * 1e9 / (double)n;
}

int main(int argc, char **argv) {
  size_t n = (argc > 1) ? (size_t)strtoul(argv[1], NULL, 10) : (1u << 20);

  char *haystack = malloc(n + 1);
  if (!haystack) return 1;
  memset(haystack, 'a', n);
  haystack[n] = '\0';

  printf("macOS libc: strstr / memmem, haystack = 'a' x %zu\n", n);
  printf("adversary: needle = 'a'x(m-1) + 'b'   control: needle = 'b' + 'a'x(m-1)\n\n");
  printf("%8s %14s %14s %14s %14s\n", "m", "strstr adv", "strstr ctl",
         "memmem adv", "memmem ctl");
  printf("%8s %14s %14s %14s %14s\n", "", "ns/byte", "ns/byte", "ns/byte",
         "ns/byte");

  for (size_t m = 8; m <= 4096; m *= 2) {
    char *adv = malloc(m + 1);
    char *ctl = malloc(m + 1);
    if (!adv || !ctl) return 1;

    memset(adv, 'a', m);
    adv[m - 1] = 'b';  // filter byte 'a' hits everywhere, verify fails last
    adv[m] = '\0';

    memset(ctl, 'a', m);
    ctl[0] = 'b';  // filter byte 'b' never hits
    ctl[m] = '\0';

    double sa = measure(call_strstr, haystack, n, adv, m, 0.05);
    double sc = measure(call_strstr, haystack, n, ctl, m, 0.05);
    double ma = measure(call_memmem, haystack, n, adv, m, 0.05);
    double mc = measure(call_memmem, haystack, n, ctl, m, 0.05);

    printf("%8zu %14.3f %14.3f %14.3f %14.3f\n", m, sa, sc, ma, mc);
    fflush(stdout);
    free(adv);
    free(ctl);
  }

  printf("\nQuadratic behaviour shows up as ns/byte doubling when m doubles\n");
  printf("(time = Theta(n*m) at fixed n). A linear searcher stays flat.\n");
  free(haystack);
  return 0;
}
