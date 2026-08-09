// Adversarial inputs for Java's String.indexOf.
//
// Java has three substring searchers and which one runs depends on the
// architecture, so the adversary does too:
//
//   * StringLatin1.indexOf (the portable Java code, and what runs with the
//     intrinsics disabled) scans for the needle's FIRST character and verifies.
//   * The HotSpot x86 intrinsic replaces the scan with SSE4.2 PCMPESTRI but
//     keeps the same first-character shape.
//   * The HotSpot AArch64 intrinsic is a different algorithm: for
//     8 <= m < 256 with n >= 4m it builds a 256-entry bad-character table and
//     runs Horspool (bad-character rule only, keyed on the LAST character);
//     outside that range it is a NEON scan over the LAST character.
//
// None of the three has a linear-time guarantee, so each has an adversary:
//
//   FIRST-CHARACTER adversary   needle = "a"*(m-1) + "b",  haystack = "a"*n
//       Every position passes the filter; every verification compares m-1
//       characters before failing. Theta(n*m).
//
//   LAST-CHARACTER adversary    needle = "b" + "a"*(m-1),  haystack = "a"*n
//       The needle's last character is 'a', so the NEON scan finds a candidate
//       at every position. For Horspool the bad-character shift for 'a' is
//       (m-1) - (m-2) = 1, the smallest possible, while each window matches
//       m-1 characters before failing at index 0. Theta(n*m).
//
// Each is the other's control: on this AArch64 machine the first-character
// adversary is linear (the filter character 'b' never occurs) and the
// last-character adversary is quadratic. Run with the intrinsics disabled and
// the two swap roles, which is the architecture dependence made visible.
//
// Build: javac Adversarial.java
// Run:   java Adversarial
//        java -XX:DisableIntrinsic=_indexOfL,_indexOfU,_indexOfUL,_indexOfIL,\
//             _indexOfIU,_indexOfIUL,_indexOfL_char,_indexOfU_char \
//             -XX:+UnlockDiagnosticVMOptions Adversarial
public class Adversarial {

  static String repeat(char c, int k) {
    char[] b = new char[k];
    java.util.Arrays.fill(b, c);
    return new String(b);
  }

  static long sink;

  // Nanoseconds per haystack character, best of several repetitions.
  static double measure(String haystack, String needle, double minSeconds) {
    long best = Long.MAX_VALUE;
    double spent = 0.0;
    int rounds = 0;
    while (spent < minSeconds || rounds < 3) {
      long t0 = System.nanoTime();
      int r = haystack.indexOf(needle);
      long elapsed = System.nanoTime() - t0;
      sink += r;
      if (elapsed < best) best = elapsed;
      spent += elapsed / 1e9;
      rounds++;
      if (rounds > 1000) break;
    }
    return (double) best / haystack.length();
  }

  public static void main(String[] args) {
    int n = args.length > 0 ? Integer.parseInt(args[0]) : (1 << 20);
    String haystack = repeat('a', n);

    // Warm the JIT up so the intrinsic (or its absence) is what we time.
    String warmNeedle = repeat('a', 63) + "b";
    String warmHay = repeat('a', 1 << 16);
    for (int i = 0; i < 20000; i++) {
      sink += warmHay.indexOf(warmNeedle);
      sink += warmHay.indexOf("b" + repeat('a', 63));
    }

    System.out.println("Java " + System.getProperty("java.version") + " on "
        + System.getProperty("os.arch") + ", String.indexOf");
    System.out.println("haystack = 'a' x " + n);
    System.out.println("first-char adversary: needle = 'a'x(m-1) + 'b'");
    System.out.println("last-char  adversary: needle = 'b' + 'a'x(m-1)");
    System.out.println();
    System.out.printf("%8s %18s %18s%n", "m", "first-char adv", "last-char adv");
    System.out.printf("%8s %18s %18s%n", "", "ns/char", "ns/char");

    for (int m = 8; m <= 4096; m *= 2) {
      String firstCharAdv = repeat('a', m - 1) + "b";
      String lastCharAdv = "b" + repeat('a', m - 1);
      double a = measure(haystack, firstCharAdv, 0.05);
      double b = measure(haystack, lastCharAdv, 0.05);
      System.out.printf("%8d %18.3f %18.3f%n", m, a, b);
    }

    System.out.println();
    System.out.println("Quadratic behaviour shows up as ns/char doubling when m doubles");
    System.out.println("(time = Theta(n*m) at fixed n). A linear searcher stays flat.");

    // The sweep above varies m at fixed n, establishing the m factor. Vary n at
    // fixed m to establish the other one: ns/char flat in n means time ~ n, so
    // together the cost is Theta(n*m) rather than, say, Theta(n^2) or Theta(m^2).
    System.out.println();
    System.out.println("Scaling n at fixed m = 1024 (flat ns/char means time is linear in n):");
    System.out.println();
    System.out.printf("%10s %18s %18s%n", "n", "first-char adv", "last-char adv");
    String firstFixed = repeat('a', 1023) + "b";
    String lastFixed = "b" + repeat('a', 1023);
    for (int k = 16; k <= 22; k++) {
      String h = repeat('a', 1 << k);
      System.out.printf("%10d %18.3f %18.3f%n", 1 << k,
          measure(h, firstFixed, 0.05), measure(h, lastFixed, 0.05));
    }

    if (sink == 42) System.out.println(sink);
  }
}
