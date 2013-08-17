import java.util.Arrays;
import java.util.BitSet;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Random;
import java.util.TreeSet;
public class UniformDistinct {
    abstract class Bench {
        abstract int[] generate(int N);
        long run(int N) {
            long bef = System.nanoTime();
            for (int T = 0; T < TIMES; ++T) {
                int[] x = generate(N);
                if (x.length != N) throw new RuntimeException("bug");
            }
            long aft = System.nanoTime();
            return aft - bef;
        }
    }
    int M, Max, TIMES;
    private double rate(int n, long t) {
        return ((n * 1000.0 * TIMES) / t);
    }
    void run() {
        Max = 1 << M;
        Bench b1 = new Bench() {
            @Override int[] generate(int N) {
                return generateUniformHash(N, Max);
            }
        };
        Bench b2 = new Bench() {
            @Override int[] generate(int N) {
                return generateUniformHashWithNegate(N, Max);
            }
        };
        Bench b3 = new Bench() {
            @Override int[] generate(int N) {
                return generateUniformHybridHashBitmap(N, Max, 2048);
            }
        };
        Bench b4 = new Bench() {
            @Override int[] generate(int N) {
                return generateUniformReservoirSampling(N, Max);
            }
        };
        Bench b5 = new Bench() {
            @Override int[] generate(int N) {
                return generateUniformTree(N, Max);
            }
        };
        Bench b6 = new Bench() {
            @Override int[] generate(int N) {
                return generateUniformShuffle(N, Max);
            }
        };
        Bench b7 = new Bench() {
            UniformDistinctShuffle o = new UniformDistinctShuffle(Max);
            @Override int[] generate(int N) {
                return o.generate(N);
            }
        };
        System.out.println("reporting speed Max: " + Max);
        // .................12345678123456789012345678901234567890123456789012345678901234567890
        System.out.println("              hash  hash/neg    hybrid reservoir      tree   shuffle  shuffle2");
        for (int times = 0; times < 3; ++times) {
            for (int k = M - 12; k < M; ++k) {
                int N = 1 << k;
                long t1 = b1.run(N);
                long t2 = b2.run(N);
                long t3 = b3.run(N);
                long t4 = b4.run(N);
                long t5 = b5.run(N);
                long t6 = b6.run(N);
                long t7 = b7.run(N);
                System.out.println(String.format("%6d  %10.2f%10.2f%10.2f%10.2f%10.2f%10.2f%10.2f",//
                        (Max / N), //
                        rate(N, t1), //
                        rate(N, t2), //
                        rate(N, t3), //
                        rate(N, t4), //
                        rate(N, t5), //
                        rate(N, t6), //
                        rate(N, t7) //
                ));
            }
        }
    }
    static Random rand = new Random();
    static int[] generateUniformBitmap(int N, int Max) {
        if (N > Max) throw new RuntimeException("not possible");
        int[] ans = new int[N];
        if (N == Max) {
            for (int k = 0; k < N; ++k)
                ans[k] = k;
            return ans;
        }
        BitSet bs = new BitSet(Max);
        int cardinality = 0;
        while (cardinality < N) {
            int v = rand.nextInt(Max);
            if (!bs.get(v)) {
                bs.set(v);
                cardinality++;
            }
        }
        int pos = 0;
        for (int i = bs.nextSetBit(0); i >= 0; i = bs.nextSetBit(i + 1)) {
            ans[pos++] = i;
        }
        return ans;
    }
    /**
     * Generate N random integers from 0 to Max
     */
    static int[] generateUniformHash(int N, int Max) {
        if (N > Max) throw new RuntimeException("not possible");
        int[] ans = new int[N];
        if (N == Max) {
            for (int k = 0; k < N; ++k)
                ans[k] = k;
            return ans;
        }
        HashSet<Integer> s = new HashSet<Integer>();
        while (s.size() < N)
            s.add(new Integer(rand.nextInt(Max)));
        Iterator<Integer> i = s.iterator();
        for (int k = 0; k < N; ++k)
            ans[k] = i.next().intValue();
        Arrays.sort(ans);
        return ans;
    }
    /**
     * Generate N random integers from 0 to Max
     */
    static int[] generateUniformHashWithNegate(int N, int Max) {
        if (2 * N > Max) return negate(generateUniformHash(Max - N, Max), Max);
        return generateUniformHash(N, Max);
    }
    /**
     * Generate N random integers from 0 to Max
     */
    static int[] generateUniformHybridHashBitmap(int N, int Max, int T) {
        if (T * N > Max) return generateUniformBitmap(N, Max);
        return generateUniformHash(N, Max);
    }
    static int[] generateUniformReservoirSampling(int N, int Max) {
        if (N > Max) throw new RuntimeException("not possible");
        int[] ans = new int[N];
        for (int k = 0; k < N; ++k)
            ans[k] = k;
        for (int k = N; k < Max; ++k) {
            int v = rand.nextInt(k + 1);
            if (v < N) {
                ans[v] = k;
            }
        }
        Arrays.sort(ans);
        return ans;
    }
    static class UniformDistinctShuffle {
        int Max;
        private int[] a1;
        private Random r = new Random();
        public UniformDistinctShuffle(int _max) {
            Max = _max;
            a1 = new int[Max];
            for (int i = 0; i < Max; ++i) {
                a1[i] = i;
            }
        }
        public int[] generate(int N) {
            if (N > Max) throw new RuntimeException("not possible");
            for (int i1 = 0; i1 < N; ++i1) {
                int i2 = r.nextInt(Max);
                int v = a1[i1];
                a1[i1] = a1[i2];
                a1[i2] = v;
            }
            // return a1;
            // Non-cheat
            int[] a2 = new int[N];
            // System.arraycopy(a1, 0, a2, 0, N);
            for (int i = 0; i < N; ++i) {
                a2[i] = a1[i];
            }
            return a2;
        }
    }
    /**
     * Generate N random integers from 0 to Max using a simple shuffle. Note:
     * This uses a lot of space for large Max.
     */
    static int[] generateUniformShuffle(int N, int Max) {
        if (N > Max) throw new RuntimeException("not possible");
        int[] a = new int[Max];
        for (int i = 0; i < Max; ++i) {
            a[i] = i;
        }
        for (int i1 = 0; i1 < N; ++i1) {
            int i2 = rand.nextInt(Max);
            int v = a[i1];
            a[i1] = a[i2];
            a[i2] = v;
        }
        // Cheat
        // return a;
        // Non-cheat
        int[] a2 = new int[N];
        System.arraycopy(a, 0, a2, 0, N);
        return a2;
    }
    /**
     * Generate N random integers from 0 to Max
     */
    static int[] generateUniformTree(int N, int Max) {
        if (N > Max) throw new RuntimeException("not possible");
        int[] ans = new int[N];
        if (N == Max) {
            for (int k = 0; k < N; ++k)
                ans[k] = k;
            return ans;
        }
        TreeSet<Integer> s = new TreeSet<Integer>();
        while (s.size() < N)
            s.add(new Integer(rand.nextInt(Max)));
        Iterator<Integer> i = s.iterator();
        for (int k = 0; k < N; ++k)
            ans[k] = i.next().intValue();
        return ans;
    }
    public static void main(String[] args) {
        new UniformDistinct() {
            {
                M = 20;
                TIMES = 10;
            }
        }.run();
        new UniformDistinct() {
            {
                M = 16;
                TIMES = 10;
            }
        }.run();
    }
    /**
     * output all integers from the range [0,Max) that are not in the array
     */
    static int[] negate(int[] x, int Max) {
        int[] ans = new int[Max - x.length];
        int i = 0;
        int c = 0;
        for (int j = 0; j < x.length; ++j) {
            int v = x[j];
            for (; i < v; ++i)
                ans[c++] = i;
            ++i;
        }
        while (c < ans.length)
            ans[c++] = i++;
        return ans;
    }
}
