// Time store-to-load forwarding.
//
// Latency: the loaded value feeds the next store, so the round trips cannot
// overlap. Throughput: a loop over an array, or over one scratch buffer.
//
// The timed loops are in forward.c. This file checks their results and
// counts cycles with the counters library.
#include "forward.h"

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#include "counters/bench.h"

#if defined(__APPLE__)
#include <pthread.h>
#include <sys/sysctl.h>
#endif

static const int kLat = 1024;          // length of one counters sample
static const int kTimed = 4000000;    // iterations in one median sample
static const size_t kThr = 2048;      // elements, kept inside L1
static const int kThrRepeat = 4000;   // repeats of the array inside one sample

static void use(uint64_t x) { __asm__ volatile("" ::"r"(x) : "memory"); }

struct Timed {
    double min_ns;
    double median_ns;
    double max_ns;
};

// One long call, repeated back to back, so a frequency change shows up as a
// gap between the minimum and the median. Warm up first so the core is out
// of idle.
template <class F>
static Timed measure(F &&fn) {
    using clock = std::chrono::steady_clock;
    auto t0 = clock::now();
    while (clock::now() - t0 < std::chrono::milliseconds(30)) {
        use(fn());
    }
    double samples[11];
    for (double &sample : samples) {
        auto a = clock::now();
        use(fn());
        auto b = clock::now();
        sample = std::chrono::duration<double, std::nano>(b - a).count();
    }
    std::sort(samples, samples + 11);
    return {samples[0], samples[5], samples[10]};
}

static void pretty(const char *name, Timed t, size_t n, double add_ns,
                   const counters::event_aggregate *agg, size_t counter_n) {
    double ns = t.median_ns / double(n);
    std::printf("%-34s %8.3f ns", name, ns);
    if (agg && counters::has_performance_counters()) {
        // c is the mean cycle count per iteration. cmin is the shortest sample.
        // On the M4 the shortest sample can be far below the mean.
        double n = double(counter_n);
        std::printf("  %5.2f GHz %7.2f c %7.2f cmin %7.2f i",
                    agg->cycles() / agg->elapsed_ns(),
                    agg->cycles() / n,
                    agg->fastest_cycles() / n,
                    agg->instructions() / n);
    } else if (add_ns > 0) {
        // No counters: divide by a dependent add, which is one cycle.
        std::printf(" %7.2f c", ns / add_ns);
    }
    std::printf("   (min %.3f max %.3f)\n", t.min_ns / double(n),
                t.max_ns / double(n));
}

static std::string cpu_name() {
#if defined(__APPLE__)
    char brand[256];
    size_t len = sizeof brand;
    if (sysctlbyname("machdep.cpu.brand_string", brand, &len, nullptr, 0) == 0) {
        return brand;
    }
    return "Apple";
#elif defined(__linux__)
    FILE *f = std::fopen("/proc/cpuinfo", "r");
    if (!f) {
        return "Linux";
    }
    char line[256];
    std::string name = "Linux";
    while (std::fgets(line, sizeof line, f)) {
        if (std::strncmp(line, "model name", 10) == 0) {
            char *colon = std::strchr(line, ':');
            if (colon) {
                name = colon + 2;
                while (!name.empty() && (name.back() == '\n' || name.back() == '\r')) {
                    name.pop_back();
                }
            }
            break;
        }
    }
    std::fclose(f);
    return name;
#else
    return "unknown";
#endif
}

static uint64_t load8(const unsigned char *p) {
    uint64_t w;
    std::memcpy(&w, p, 8);
    return w;
}

static int fail(const char *what, uint64_t got, uint64_t want) {
    std::printf("check failed: %s got %016llx want %016llx\n", what,
                (unsigned long long)got, (unsigned long long)want);
    return 0;
}

static int checks() {
    alignas(64) unsigned char buf[128];
    std::memset(buf, 0x11, sizeof buf);
    uint64_t x = 0x0123456789abcdefull;

    if (lat_reg(x, 10) != x + 10) {
        return fail("reg", lat_reg(x, 10), x + 10);
    }
    if (lat_same(buf, x, 4) != x) {
        return fail("same", lat_same(buf, x, 4), x);
    }

    std::memset(buf, 0x11, sizeof buf);
    if (lat_low32(buf, x, 1) != (uint32_t)x) {
        return fail("low32", lat_low32(buf, x, 1), (uint32_t)x);
    }
    std::memset(buf, 0x11, sizeof buf);
    if (lat_hi32(buf, x, 1) != (x >> 32)) {
        return fail("hi32", lat_hi32(buf, x, 1), x >> 32);
    }
    if (lat_two(buf, x, 4) != x) {
        return fail("two", lat_two(buf, x, 4), x);
    }

    std::memset(buf, 0x11, sizeof buf);
    unsigned char ref[128];
    std::memcpy(ref, buf, sizeof buf);
    unsigned char b = (unsigned char)x;
    std::memcpy(ref, &b, 1);
    uint64_t got = lat_byte(buf, x, 1);
    if (got != load8(ref)) {
        return fail("byte", got, load8(ref));
    }

    std::memset(buf, 0x11, sizeof buf);
    std::memset(ref, 0x11, sizeof ref);
    uint32_t lo = (uint32_t)x;
    std::memcpy(ref, &lo, 4);
    got = lat_u32(buf, x, 1);
    if (got != load8(ref)) {
        return fail("u32", got, load8(ref));
    }

    std::memset(buf, 0x11, sizeof buf);
    std::memset(ref, 0x11, sizeof ref);
    std::memcpy(ref, &x, 8);
    got = lat_overlap4(buf, x, 1);
    if (got != load8(ref + 4)) {
        return fail("overlap4", got, load8(ref + 4));
    }

    std::memset(buf, 0x11, sizeof buf);
    std::memset(ref, 0x11, sizeof ref);
    std::memcpy(ref, &x, 8);
    got = lat_overlap1(buf, x, 1);
    if (got != load8(ref + 1)) {
        return fail("overlap1", got, load8(ref + 1));
    }

    alignas(4096) static unsigned char pages[8192];
    std::memset(pages, 0x5a, sizeof pages);
    uint64_t at64 = 0x1111222233334444ull;
    uint64_t at4k = 0x5555666677778888ull;
    std::memcpy(pages + 64, &at64, 8);
    std::memcpy(pages + 4096, &at4k, 8);
    if (lat_plus64(pages, x, 3) != at64) {
        return fail("plus64", lat_plus64(pages, x, 3), at64);
    }
    if (lat_plus4k(pages, x, 3) != at4k) {
        return fail("plus4k", lat_plus4k(pages, x, 3), at4k);
    }

    uint64_t words_a[32], words_b[32];
    uint32_t lo_a[32], hi_a[32];
    for (int i = 0; i < 32; i++) {
        words_a[i] = words_b[i] = 0x1000ull * (uint64_t)i + 0xabcdef;
        lo_a[i] = 0x01000000u * (uint32_t)i + 0x11u;
        hi_a[i] = 0x02000000u * (uint32_t)i + 0x22u;
    }
    uint64_t hb = thr_flip_byte(words_a, 32);
    uint64_t hw = thr_flip_word(words_b, 32);
    if (hb != hw || std::memcmp(words_a, words_b, sizeof words_a) != 0) {
        return fail("flip", hb, hw);
    }
    alignas(64) unsigned char tmp[8];
    uint64_t hm = thr_join_mem(tmp, lo_a, hi_a, 32);
    uint64_t hr = thr_join_reg(lo_a, hi_a, 32);
    if (hm != hr) {
        return fail("join", hm, hr);
    }
    return 1;
}

// c* is the median time divided by the median time of a dependent add.
// On Intel, c and i come from the performance counters.
template <class Fmed, class Fcnt>
static void run(const char *name, size_t nmed, Fmed &&medfn, size_t ncnt, Fcnt &&cntfn,
                double add_ns, bool with_counters) {
    Timed t = measure(medfn);
    counters::event_aggregate agg{};
    const counters::event_aggregate *p = nullptr;
    if (with_counters) {
        agg = counters::bench([&] { use(cntfn()); });
        p = &agg;
    }
    pretty(name, t, nmed, add_ns, p, ncnt);
}

int main() {
#if defined(__APPLE__)
    pthread_set_qos_class_self_np(QOS_CLASS_USER_INTERACTIVE, 0);
#endif
    std::printf("# cpu: %s\n", cpu_name().c_str());
    std::printf("# compiler: %s\n", __VERSION__);
    std::printf("# counters: %s\n",
                counters::has_performance_counters() ? "yes" : "no");
    std::printf("# median iterations: %d\n", kTimed);
    std::printf("# counter iterations: %d\n", kLat);
    std::printf("# throughput elements: %zu, repeated %d\n", kThr, kThrRepeat);
    std::printf("# c is mean cycles per iteration from the counters\n");
    std::printf("# cmin is the cycle count of the shortest sample\n");
    if (!checks()) {
        return 1;
    }
    std::printf("# checks ok\n");

    unsigned char *page = static_cast<unsigned char *>(std::aligned_alloc(4096, 8192));
    if (!page) {
        std::printf("aligned_alloc failed\n");
        return 1;
    }
    std::memset(page, 0x11, 8192);
    uint64_t far = 0x1111222233334444ull;
    uint64_t alias = 0x5555666677778888ull;
    std::memcpy(page + 64, &far, 8);
    std::memcpy(page + 4096, &alias, 8);
    unsigned char *p = page + 256;
    uint64_t x = 0x0123456789abcdefull;

    bool with = counters::has_performance_counters();
    Timed add_t = measure([&] { return lat_reg(x, kTimed); });
    double add_ns = add_t.median_ns / double(kTimed);
    counters::event_aggregate add_agg{};
    const counters::event_aggregate *add_p = nullptr;
    if (with) {
        add_agg = counters::bench([&] { use(lat_reg(x, kLat)); });
        add_p = &add_agg;
    }

    std::printf("\n# latency, one dependent iteration\n");
    pretty("register", add_t, kTimed, add_ns, add_p, kLat);
    run("store 8 / load 8", kTimed, [&] { return lat_same(p, x, kTimed); }, kLat,
        [&] { return lat_same(p, x, kLat); }, add_ns, with);
    run("store 8 / load 4", kTimed, [&] { return lat_low32(p, x, kTimed); }, kLat,
        [&] { return lat_low32(p, x, kLat); }, add_ns, with);
    run("store 8 / load 4 at +4", kTimed, [&] { return lat_hi32(p, x, kTimed); }, kLat,
        [&] { return lat_hi32(p, x, kLat); }, add_ns, with);
    run("store 4 / load 8", kTimed, [&] { return lat_u32(p, x, kTimed); }, kLat,
        [&] { return lat_u32(p, x, kLat); }, add_ns, with);
    run("store 1 / load 8", kTimed, [&] { return lat_byte(p, x, kTimed); }, kLat,
        [&] { return lat_byte(p, x, kLat); }, add_ns, with);
    run("store 4+4 / load 8", kTimed, [&] { return lat_two(p, x, kTimed); }, kLat,
        [&] { return lat_two(p, x, kLat); }, add_ns, with);
    run("store 8 / load 8 at +4", kTimed, [&] { return lat_overlap4(p, x, kTimed); }, kLat,
        [&] { return lat_overlap4(p, x, kLat); }, add_ns, with);
    run("store 8 / load 8 at +1", kTimed, [&] { return lat_overlap1(p, x, kTimed); }, kLat,
        [&] { return lat_overlap1(p, x, kLat); }, add_ns, with);
    run("store 8 / load 8 at +64", kTimed, [&] { return lat_plus64(page, x, kTimed); }, kLat,
        [&] { return lat_plus64(page, x, kLat); }, add_ns, with);
    run("store 8 / load 8 at +4096", kTimed, [&] { return lat_plus4k(page, x, kTimed); }, kLat,
        [&] { return lat_plus4k(page, x, kLat); }, add_ns, with);

    uint64_t *words = static_cast<uint64_t *>(std::aligned_alloc(64, kThr * sizeof(uint64_t)));
    uint64_t *words2 = static_cast<uint64_t *>(std::aligned_alloc(64, kThr * sizeof(uint64_t)));
    uint32_t *lo = static_cast<uint32_t *>(std::aligned_alloc(64, kThr * sizeof(uint32_t)));
    uint32_t *hi = static_cast<uint32_t *>(std::aligned_alloc(64, kThr * sizeof(uint32_t)));
    if (!words || !words2 || !lo || !hi) {
        std::printf("aligned_alloc failed\n");
        return 1;
    }
    uint64_t s = 0x123456789abcdefull;
    for (size_t i = 0; i < kThr; i++) {
        s = s * 0x9E3779B97F4A7C15ull + 1;
        words[i] = words2[i] = s;
        lo[i] = (uint32_t)s;
        hi[i] = (uint32_t)(s >> 32);
    }
    alignas(64) unsigned char tmp[8];

    std::printf("\n# throughput, one element\n");
    auto thr = [&](const char *name, auto &&once) {
        size_t n = (size_t)kThrRepeat * kThr;
        Timed t = measure([&] {
            uint64_t h = 0;
            for (int i = 0; i < kThrRepeat; i++) {
                h ^= once();
            }
            return h;
        });
        counters::event_aggregate agg{};
        const counters::event_aggregate *p = nullptr;
        if (with) {
            agg = counters::bench([&] { use(once()); });
            p = &agg;
        }
        pretty(name, t, n, add_ns, p, kThr);
    };
    thr("flip byte, reload word", [&] { return thr_flip_byte(words, kThr); });
    thr("flip bit, in a register", [&] { return thr_flip_word(words2, kThr); });
    thr("join two stores, reload", [&] { return thr_join_mem(tmp, lo, hi, kThr); });
    thr("join in a register", [&] { return thr_join_reg(lo, hi, kThr); });

    std::free(page);
    std::free(words);
    std::free(words2);
    std::free(lo);
    std::free(hi);
    return 0;
}
