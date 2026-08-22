// Compare Java String.indexOf with Crochemore-Perrin Two-Way on the
// same adversarial inputs as Adversarial.java, plus a random control.
//
// Build: javac Adversarial.java TwoWay.java Benchmark.java
// Run:   java Benchmark
//        java Benchmark 1048576
import java.util.Locale;
import java.util.Random;

public class Benchmark {

  static String repeat(char c, int k) {
    char[] b = new char[k];
    java.util.Arrays.fill(b, c);
    return new String(b);
  }

  static long sink;

  interface Searcher {
    int search(String haystack, String needle, char[] h, char[] nd);
  }

  static final Searcher JAVA = (haystack, needle, h, nd) -> haystack.indexOf(needle);
  static final Searcher TWOWAY = (haystack, needle, h, nd) -> TwoWay.indexOf(h, nd);

  // Nanoseconds per haystack character, best of several repetitions.
  static double measure(Searcher f, String haystack, String needle, double minSeconds) {
    char[] h = haystack.toCharArray();
    char[] nd = needle.toCharArray();
    long best = Long.MAX_VALUE;
    double spent = 0.0;
    int rounds = 0;
    while (spent < minSeconds || rounds < 3) {
      long t0 = System.nanoTime();
      int r = f.search(haystack, needle, h, nd);
      long elapsed = System.nanoTime() - t0;
      sink += r;
      if (elapsed < best) {
        best = elapsed;
      }
      spent += elapsed / 1e9;
      rounds++;
      if (rounds > 1000) {
        break;
      }
    }
    return (double) best / haystack.length();
  }

  static void check(String haystack, String needle) {
    int a = haystack.indexOf(needle);
    int b = TwoWay.indexOf(haystack, needle);
    int c = TwoWay.indexOf(haystack.toCharArray(), needle.toCharArray());
    if (a != b || a != c) {
      throw new AssertionError(
          "mismatch: indexOf=" + a + " TwoWay(String)=" + b + " TwoWay(char[])=" + c
              + " haystack=" + excerpt(haystack) + " needle=" + excerpt(needle));
    }
    sink += a;
  }

  static String excerpt(String s) {
    if (s.length() <= 64) {
      return '"' + s + '"';
    }
    return '"' + s.substring(0, 24) + "...\" (len " + s.length() + ")";
  }

  static String randomString(Random rng, int len, int alphabet) {
    char[] b = new char[len];
    for (int i = 0; i < len; i++) {
      b[i] = (char) ('a' + rng.nextInt(alphabet));
    }
    return new String(b);
  }

  static void correctness() {
    check("", "");
    check("", "a");
    check("a", "");
    check("a", "a");
    check("a", "b");
    check("hello", "ll");
    check("hello", "hello");
    check("hello", "hellox");
    check("hello", "h");
    check("hello", "o");
    check("hello", "x");
    check("mississippi", "issip");
    check("mississippi", "issi");
    check("aaaaaaa", "aaaa");
    check("aaaaaaa", "b");
    check("abcabcabc", "cab");
    check("ababababab", "ababab");
    check("GCATCGCAGAGAGTATACAGTACG", "GCAGAGAG");

    // Both adversarial families, with and without a match at the end.
    for (int n = 1; n <= 200; n += 7) {
      String as = repeat('a', n);
      check(as, "a");
      check(as, "b");
      check(as, as);
      for (int m = 1; m <= n && m <= 80; m += 3) {
        String first = repeat('a', m - 1) + "b";
        String last = "b" + repeat('a', m - 1);
        check(as, first);
        check(as, last);
        check(as + first, first);
        check(as + last, last);
        check(repeat('a', m), first);
        check(repeat('a', m), last);
      }
    }

    Random rng = new Random(1);
    for (int t = 0; t < 5000; t++) {
      int n = rng.nextInt(80);
      int m = rng.nextInt(20);
      int alphabet = 1 + rng.nextInt(8);
      String haystack = randomString(rng, n, alphabet);
      String needle = randomString(rng, m, alphabet);
      check(haystack, needle);
    }

    // Periodic needles over a long run.
    check(repeat('a', 1000), repeat('a', 17));
    check(repeat('a', 1000) + "b", repeat('a', 17) + "b");
    check("ab".repeat(200), "ab".repeat(13));
    check("ab".repeat(200), "ba".repeat(13));
    check("abc".repeat(100), "cabcab");

    System.out.println("correctness: Two-Way agrees with String.indexOf");
  }

  static void warmup() {
    String warmNeedle = repeat('a', 63) + "b";
    String warmHay = repeat('a', 1 << 16);
    char[] h = warmHay.toCharArray();
    char[] nd1 = warmNeedle.toCharArray();
    char[] nd2 = ("b" + repeat('a', 63)).toCharArray();
    for (int i = 0; i < 20000; i++) {
      sink += warmHay.indexOf(warmNeedle);
      sink += warmHay.indexOf("b" + repeat('a', 63));
      sink += TwoWay.indexOf(h, nd1);
      sink += TwoWay.indexOf(h, nd2);
    }
  }

  public static void main(String[] args) {
    correctness();
    warmup();

    int n = args.length > 0 ? Integer.parseInt(args[0]) : (1 << 20);
    String haystack = repeat('a', n);

    System.out.println();
    System.out.println("Java " + System.getProperty("java.version") + " on "
        + System.getProperty("os.arch"));
    System.out.println("haystack = 'a' x " + n);
    System.out.println("first-char adversary: needle = 'a'x(m-1) + 'b'");
    System.out.println("last-char  adversary: needle = 'b' + 'a'x(m-1)");
    System.out.println();
    System.out.printf(Locale.US, "%8s %14s %14s %14s %14s%n", "m",
        "indexOf first", "indexOf last", "Two-Way first", "Two-Way last");
    System.out.printf(Locale.US, "%8s %14s %14s %14s %14s%n", "", "ns/char",
        "ns/char", "ns/char", "ns/char");

    for (int m = 8; m <= 4096; m *= 2) {
      String firstCharAdv = repeat('a', m - 1) + "b";
      String lastCharAdv = "b" + repeat('a', m - 1);
      double jf = measure(JAVA, haystack, firstCharAdv, 0.05);
      double jl = measure(JAVA, haystack, lastCharAdv, 0.05);
      double tf = measure(TWOWAY, haystack, firstCharAdv, 0.05);
      double tl = measure(TWOWAY, haystack, lastCharAdv, 0.05);
      System.out.printf(Locale.US, "%8d %14.3f %14.3f %14.3f %14.3f%n", m, jf,
          jl, tf, tl);
    }

    System.out.println();
    System.out.println(
        "Quadratic behaviour shows up as ns/char doubling when m doubles");
    System.out.println(
        "(time = Theta(n*m) at fixed n). A linear searcher stays flat.");

    System.out.println();
    System.out.println(
        "Scaling n at fixed m = 1024 (flat ns/char means time is linear in n):");
    System.out.println();
    System.out.printf(Locale.US, "%10s %14s %14s %14s %14s%n", "n",
        "indexOf first", "indexOf last", "Two-Way first", "Two-Way last");
    String firstFixed = repeat('a', 1023) + "b";
    String lastFixed = "b" + repeat('a', 1023);
    for (int k = 16; k <= 22; k++) {
      String h = repeat('a', 1 << k);
      System.out.printf(Locale.US, "%10d %14.3f %14.3f %14.3f %14.3f%n", 1 << k,
          measure(JAVA, h, firstFixed, 0.05),
          measure(JAVA, h, lastFixed, 0.05),
          measure(TWOWAY, h, firstFixed, 0.05),
          measure(TWOWAY, h, lastFixed, 0.05));
    }

    // Random lowercase text: both searchers should stay linear, and Java's
    // SIMD filter should win. Needle is 64 random letters, almost surely
    // absent.
    System.out.println();
    System.out.println(
        "Random lowercase haystack (1 MiB), random needle of length m:");
    System.out.println();
    System.out.printf(Locale.US, "%8s %14s %14s%n", "m", "indexOf", "Two-Way");
    Random rng = new Random(42);
    String randomHay = randomString(rng, n, 26);
    for (int m = 8; m <= 4096; m *= 2) {
      String needle = randomString(rng, m, 26);
      double j = measure(JAVA, randomHay, needle, 0.05);
      double t = measure(TWOWAY, randomHay, needle, 0.05);
      System.out.printf(Locale.US, "%8d %14.3f %14.3f%n", m, j, t);
    }

    if (sink == 42) {
      System.out.println(sink);
    }
  }
}
