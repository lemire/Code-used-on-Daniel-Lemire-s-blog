// Fixed harness for the CRoaring commit history. Links against the amalgamated
// roaring.c of each commit. Loads the realdata sets and reports, per operation,
// the best-of-N time in microseconds per dataset pass.
#include "roaring.h"
#include <dirent.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define REPS 5
#define QUERIES_PER_BITMAP 2000

static double now(void) {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return ts.tv_sec + ts.tv_nsec * 1e-9;
}

typedef struct { uint32_t *v; size_t n; } arr_t;

static int cmp_str(const void *a, const void *b) { return strcmp(*(char *const *)a, *(char *const *)b); }

static size_t load_dataset(const char *dir, arr_t **out) {
  DIR *d = opendir(dir);
  if (!d) { fprintf(stderr, "cannot open %s\n", dir); exit(1); }
  char **names = NULL; size_t nn = 0;
  struct dirent *e;
  while ((e = readdir(d))) {
    if (strstr(e->d_name, ".txt")) { names = realloc(names, (nn + 1) * sizeof(char *)); names[nn++] = strdup(e->d_name); }
  }
  closedir(d);
  qsort(names, nn, sizeof(char *), cmp_str);
  arr_t *arrs = malloc(nn * sizeof(arr_t));
  for (size_t i = 0; i < nn; i++) {
    char path[4096];
    snprintf(path, sizeof(path), "%s/%s", dir, names[i]);
    FILE *f = fopen(path, "r");
    size_t cap = 1024, n = 0;
    uint32_t *v = malloc(cap * sizeof(uint32_t));
    unsigned long x;
    while (fscanf(f, "%lu,", &x) == 1) {
      if (n == cap) { cap *= 2; v = realloc(v, cap * sizeof(uint32_t)); }
      v[n++] = (uint32_t)x;
    }
    fclose(f);
    arrs[i].v = v; arrs[i].n = n;
  }
  *out = arrs;
  return nn;
}

static uint64_t rng_state = 88172645463325252ULL;
static uint64_t rng(void) { rng_state ^= rng_state << 13; rng_state ^= rng_state >> 7; rng_state ^= rng_state << 17; return rng_state; }

#define TIME(name, body) do { \
  double best = 1e30; uint64_t sink = 0; \
  for (int rep = 0; rep < REPS; rep++) { double t0 = now(); body; double t = now() - t0; if (t < best) best = t; } \
  printf("%-28s %-18s %12.1f us  (sink %llu)\n", name, dsname, best * 1e6, (unsigned long long)sink); \
} while (0)

int main(int argc, char **argv) {
  if (argc < 2) { fprintf(stderr, "usage: %s realdata_dir [datasets...]\n", argv[0]); return 1; }
  const char *root = argv[1];
  const char *defaults[] = {"census1881", "census-income", "weather_sept_85"};
  int nds = argc > 2 ? argc - 2 : 3;
  for (int di = 0; di < nds; di++) {
    const char *dsname = argc > 2 ? argv[2 + di] : defaults[di];
    char dir[4096];
    snprintf(dir, sizeof(dir), "%s/%s", root, dsname);
    arr_t *arrs; size_t nb = load_dataset(dir, &arrs);
    roaring_bitmap_t **b = malloc(nb * sizeof(*b));
    roaring64_bitmap_t **b64 = malloc(nb * sizeof(*b64));
    uint64_t *tmp64 = NULL; size_t tmpcap = 0;
    for (size_t i = 0; i < nb; i++) {
      b[i] = roaring_bitmap_of_ptr(arrs[i].n, arrs[i].v);
      roaring_bitmap_run_optimize(b[i]);
      if (arrs[i].n > tmpcap) { tmpcap = arrs[i].n; tmp64 = realloc(tmp64, tmpcap * sizeof(uint64_t)); }
      for (size_t j = 0; j < arrs[i].n; j++) tmp64[j] = ((uint64_t)(i % 4) << 32) | arrs[i].v[j];
      b64[i] = roaring64_bitmap_of_ptr(arrs[i].n, tmp64);
      roaring64_bitmap_run_optimize(b64[i]);
    }
    // contains queries: half from the set, half uniform over its range
    uint32_t *q = malloc(nb * QUERIES_PER_BITMAP * sizeof(uint32_t));
    for (size_t i = 0; i < nb; i++) {
      uint32_t lo = arrs[i].n ? arrs[i].v[0] : 0, hi = arrs[i].n ? arrs[i].v[arrs[i].n - 1] : 1;
      for (int k = 0; k < QUERIES_PER_BITMAP; k++) {
        uint32_t val = (k & 1) && arrs[i].n ? arrs[i].v[rng() % arrs[i].n] : lo + (uint32_t)(rng() % (uint64_t)(hi - lo + 1));
        q[i * QUERIES_PER_BITMAP + k] = val;
      }
    }
    // serialized forms
    char **ser = malloc(nb * sizeof(char *)); size_t *serlen = malloc(nb * sizeof(size_t));
    size_t maxcard = 0;
    for (size_t i = 0; i < nb; i++) {
      serlen[i] = roaring_bitmap_portable_size_in_bytes(b[i]);
      ser[i] = malloc(serlen[i]);
      roaring_bitmap_portable_serialize(b[i], ser[i]);
      uint64_t c = roaring_bitmap_get_cardinality(b[i]);
      if (c > maxcard) maxcard = c;
    }
    uint32_t *outarr = malloc((maxcard + 1) * sizeof(uint32_t));

    TIME("or_successive", { for (size_t i = 0; i + 1 < nb; i++) { roaring_bitmap_t *r = roaring_bitmap_or(b[i], b[i + 1]); sink += roaring_bitmap_get_cardinality(r); roaring_bitmap_free(r); } });
    TIME("and_successive", { for (size_t i = 0; i + 1 < nb; i++) { roaring_bitmap_t *r = roaring_bitmap_and(b[i], b[i + 1]); sink += roaring_bitmap_get_cardinality(r); roaring_bitmap_free(r); } });
    TIME("and_cardinality", { for (size_t i = 0; i + 1 < nb; i++) sink += roaring_bitmap_and_cardinality(b[i], b[i + 1]); });
    TIME("contains", { for (size_t i = 0; i < nb; i++) for (int k = 0; k < QUERIES_PER_BITMAP; k++) sink += roaring_bitmap_contains(b[i], q[i * QUERIES_PER_BITMAP + k]); });
    TIME("iterate", { for (size_t i = 0; i < nb; i++) { roaring_uint32_iterator_t it; roaring_iterator_init(b[i], &it); while (it.has_value) { sink += it.current_value; roaring_uint32_iterator_advance(&it); } } });
    TIME("to_array", { for (size_t i = 0; i < nb; i++) { roaring_bitmap_to_uint32_array(b[i], outarr); sink += outarr[0]; } });
    TIME("deserialize", { for (size_t i = 0; i < nb; i++) { roaring_bitmap_t *r = roaring_bitmap_portable_deserialize_safe(ser[i], serlen[i]); sink += roaring_bitmap_get_cardinality(r); roaring_bitmap_free(r); } });
    TIME("cardinality64", { for (size_t i = 0; i < nb; i++) sink += roaring64_bitmap_get_cardinality(b64[i]); });
    TIME("iterate64", { for (size_t i = 0; i < nb; i++) { roaring64_iterator_t *it = roaring64_iterator_create(b64[i]); while (roaring64_iterator_has_value(it)) { sink += roaring64_iterator_value(it); roaring64_iterator_advance(it); } roaring64_iterator_free(it); } });
    TIME("or_successive64", { for (size_t i = 0; i + 1 < nb; i++) { roaring64_bitmap_t *r = roaring64_bitmap_or(b64[i], b64[i + 1]); sink += roaring64_bitmap_get_cardinality(r); roaring64_bitmap_free(r); } });

    for (size_t i = 0; i < nb; i++) { roaring_bitmap_free(b[i]); roaring64_bitmap_free(b64[i]); free(arrs[i].v); free(ser[i]); }
    free(b); free(b64); free(arrs); free(q); free(ser); free(serlen); free(outarr); free(tmp64);
  }
  return 0;
}
