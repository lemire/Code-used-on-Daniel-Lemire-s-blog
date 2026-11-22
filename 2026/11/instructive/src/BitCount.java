import jdk.incubator.vector.LongVector;
import jdk.incubator.vector.VectorOperators;
import jdk.incubator.vector.VectorSpecies;

/** Counting the bits of the intersection of two 1024-word arrays (a bitmap container), several ways. */
public class BitCount {
    // Long.bitCount on a word loaded from memory
    static int one(long[] a, long[] b, int limit) {
        int s = 0;
        for (int i = 0; i < a.length; i++) s += Long.bitCount(a[i]);
        return s;
    }

    // Long.bitCount on a computed word, as in BitmapContainer.andCardinality
    static int and(long[] a, long[] b, int limit) {
        int s = 0;
        for (int i = 0; i < a.length; i++) s += Long.bitCount(a[i] & b[i]);
        return s;
    }

    // the same with Integer.bitCount on each half
    static int ints(long[] a, long[] b, int limit) {
        int s = 0;
        for (int i = 0; i < a.length; i++) {
            long x = a[i] & b[i];
            s += Integer.bitCount((int) x) + Integer.bitCount((int) (x >>> 32));
        }
        return s;
    }

    // a portable population count in plain Java
    static int swar(long[] a, long[] b, int limit) {
        int s = 0;
        for (int i = 0; i < a.length; i++) {
            long x = a[i] & b[i];
            x = x - ((x >>> 1) & 0x5555555555555555L);
            x = (x & 0x3333333333333333L) + ((x >>> 2) & 0x3333333333333333L);
            x = (x + (x >>> 4)) & 0x0f0f0f0f0f0f0f0fL;
            s += (int) ((x * 0x0101010101010101L) >>> 56);
        }
        return s;
    }

    static final VectorSpecies<Long> S = LongVector.SPECIES_128;

    // the Vector API
    static int vector(long[] a, long[] b, int limit) {
        LongVector acc = LongVector.zero(S);
        int i = 0;
        for (; i < S.loopBound(a.length); i += S.length()) {
            acc = acc.add(LongVector.fromArray(S, a, i).and(LongVector.fromArray(S, b, i)).lanewise(VectorOperators.BIT_COUNT));
        }
        int s = (int) acc.reduceLanes(VectorOperators.ADD);
        for (; i < a.length; i++) s += Long.bitCount(a[i] & b[i]);
        return s;
    }

    // an early exit, as in a select: C2 does not vectorize this loop
    static int until(long[] a, long[] b, int limit) {
        int s = 0;
        for (int i = 0; i < a.length; i++) {
            s += Long.bitCount(a[i] & b[i]);
            if (s >= limit) return i;
        }
        return s;
    }

    static int untilInts(long[] a, long[] b, int limit) {
        int s = 0;
        for (int i = 0; i < a.length; i++) {
            long x = a[i] & b[i];
            s += Integer.bitCount((int) x) + Integer.bitCount((int) (x >>> 32));
            if (s >= limit) return i;
        }
        return s;
    }

    interface F {
        int f(long[] a, long[] b, int limit);
    }

    static void time(String name, F f, long[] a, long[] b) {
        long best = Long.MAX_VALUE;
        int sink = 0;
        for (int r = 0; r < 30; r++) {
            long t = System.nanoTime();
            for (int k = 0; k < 10000; k++) sink += f.f(a, b, Integer.MAX_VALUE);
            best = Math.min(best, System.nanoTime() - t);
        }
        System.out.printf("%-10s %.3f ns/word (%d)%n", name, best / 10000.0 / a.length, sink);
    }

    public static void main(String[] args) {
        java.util.Random r = new java.util.Random(1);
        long[] a = new long[1024], b = new long[1024];
        for (int i = 0; i < a.length; i++) {
            a[i] = r.nextLong();
            b[i] = r.nextLong();
        }
        for (int k = 0; k < 3; k++) {
            System.out.println("round " + k);
            time("one", BitCount::one, a, b);
            time("and", BitCount::and, a, b);
            time("ints", BitCount::ints, a, b);
            time("swar", BitCount::swar, a, b);
            time("vector", BitCount::vector, a, b);
            time("until", BitCount::until, a, b);
            time("untilInts", BitCount::untilInts, a, b);
        }
    }
}
