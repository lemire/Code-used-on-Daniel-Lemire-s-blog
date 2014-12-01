import java.util.Arrays;
import java.util.HashSet;
import java.util.Random;

public final class bitcount {
    
    public static byte[] arraycnt = new byte[1<<16];


    // use small functions that Java can compile, not inline code
    public static int summe1(int[] ints) {
        int card = 0;
        for(int k=0; k<ints.length; k++)
            card+=count(ints[k]);
        return card;
    }

    public static int summe2(int[] ints) {
        int card = 0;
        for(int k=0; k<ints.length; k++)
            card+=Integer.bitCount(ints[k]);
        return card;
    }
    public static int summe2(long[] ints) {
        int card = 0;
        for(int k=0; k<ints.length; k++)
            card+=Long.bitCount(ints[k]);
        return card;
    }
    
    public static int summe3(int[] ints) {
        int card = 0;
        for(int k=0; k<ints.length; k++)
            card+=countlookup(ints[k]);
        return card;
    }
    public static int summe3(long[] ints) {
        int card = 0;
        for(int k=0; k<ints.length; k++)
            card+=countlookup(ints[k]);
        return card;
    }
    public static void main(String[] args) {
         for(int k = 0; k< 1<<16; ++k) arraycnt[k] = (byte)Integer.bitCount(k);
        // always do a dry-run so that Java has time to compile
        for(int n=100; n<=1000000; n*=10) {
            System.out.println("Dry-run Number of ints = "+n);
            DataGenerator gen = new DataGenerator(n);
            gen.setUniform();
            int ints[] = gen.getUniform(0.500);
            int card = 0;
            for(int i=0; i<100; i++) {
                card += summe1(ints);
            }
            card = 0;
            for(int i=0; i<100; i++) {
                card += summe2(ints);
            }
            card = 0;
            for(int i=0; i<100; i++) {
                card += summe3(ints);
            }
            long[] longs = new long[ints.length];
            for(int k = 0; k < ints.length; ++k) longs[k] = ints[k];
            card = 0;
            for(int i=0; i<100; i++) {
                card += summe2(longs);
            }
            card = 0;
            for(int i=0; i<100; i++) {
                card += summe3(longs);
            }
        }

        for(int n=100; n<=10000000; n*=10) {
            System.out.println("Number of ints = "+n);
            long bef, after;
            DataGenerator gen = new DataGenerator(n);
            gen.setUniform();
            int ints[] = gen.getUniform(0.500);
            //trying count of concise
            int card = 0;
            bef = System.nanoTime();
            for(int i=0; i<100; i++) {
                card += summe1(ints);
            }
            after = System.nanoTime();

            System.out.println("card = "+card+", Count concise time = "+((after-bef)/100)+" nanosec");

            //trying Integer.bitCount (popcnt)
            card = 0;
            bef = System.nanoTime();
            for(int i=0; i<100; i++) {
                card += summe2(ints);
            }
            after = System.nanoTime();

            System.out.println("card = "+card+", Integer.bitCount(popcnt) time = "+((after-bef)/100)+" nanosec");
            
            //trying table look-up
            card = 0;
            bef = System.nanoTime();
            for(int i=0; i<100; i++) {
                card += summe3(ints);
            }
            after = System.nanoTime();

            System.out.println("card = "+card+", table look-up time = "+((after-bef)/100)+" nanosec");
            long[] longs = new long[ints.length];
            for(int k = 0; k < ints.length; ++k) longs[k] = ints[k];
            card = 0;
            bef = System.nanoTime();
            for(int i=0; i<100; i++) {
                card += summe2(longs);
            }
            after = System.nanoTime();

            System.out.println("card = "+card+", Long.bitCount(popcnt) time = "+((after-bef)/100)+" nanosec");
            
            //trying table look-up
            card = 0;
            bef = System.nanoTime();
            for(int i=0; i<100; i++) {
                card += summe3(longs);
            }
            after = System.nanoTime();

            System.out.println("card = "+card+", table look-up time = "+((after-bef)/100)+" nanosec");

        }
    }

    /**
     * Population count
     * <p>
     * It counts a single word
     *
     * @param word
     *            word to count
     * @return population count
     */
    public static int count(int word) {
        word -= ((word >>> 1) & 0x55555555);
        word = (word & 0x33333333) + ((word >>> 2) & 0x33333333);
        word = (word + (word >>> 4)) & 0x0F0F0F0F;
        return (word * 0x01010101) >>> 24;
    }

    /**
     * Population count
     * <p>
     * It counts a single word
     *
     * @param word
     *            word to count
     * @return population count
     */
    public static int countlookup(int word) {
        return arraycnt[word & 0xFFFF] + arraycnt[(word>>>16) & 0xFFFF];
    }



    /**
     * Population count
     * <p>
     * It counts a single word
     *
     * @param word
     *            word to count
     * @return population count
     */
    public static int countlookup(long word) {
        return arraycnt[(int)(word & 0xFFFF)] + arraycnt[(int)((word>>>16) & 0xFFFF)] + arraycnt[(int)((word>>>32) & 0xFFFF)] + arraycnt[(int)((word>>>24) & 0xFFFF)];
    }
}

class DataGenerator {
    public Random rand = new Random();
    public int N;
    boolean zipfian = false;

    /**
     * Will generate arrays with default size (100000)
     */
    public DataGenerator() {
        N = 100000;
    }

    /**
     * @param n size of the arrays
     */
    public DataGenerator(final int n) {
        if(n<1) throw new IllegalArgumentException("number of ints should be positive");
        N = n;
    }

    /**
     * @return whether the data generator is in zipfian mode
     */
    public boolean is_zipfian() {
        return zipfian;
    }

    /**
     * set the data generator in uniform  mode
     */
    public void setUniform() {
        zipfian = false;
    }

    /**
     * set the data generator in Zipfian mode
     */
    public void setZipfian() {
        zipfian = true;
    }
    /**
     * Generate a random array (sorted integer set)
     *
     * @param d should vary from 0 to 0.999
     * @return an array with a uniform or zipfian distribution
     */
    public int[] getRandomArray(double d) {
        if(zipfian)
            return getZipfian(d);
        return getUniform(d);
    }
    /**
     * Generate a random array (sorted integer set)
     *
     * @param d should vary from 0 to 1.000
     * @return an array with a uniform distribution
     */
    public int[] getUniform(double d) {
        ////////////////// (from arxiv version)
        //at each generation of a pseudo-random number a in [0, 1),
        //in uniform sets an integer corresponding to floor(a * max) was added,
        //where max =
        //105/d by varying d (the density) from 0.001 to 0.999.
        //////////////////
        if((d<0) || (d>1.000)) throw new IllegalArgumentException("parameter should be in [0.005,0.999]");
        if(d>=0.99) {
            int[] answer = new int[N];
            for(int k = 0; k<N; ++k) answer[k] = k;
            return answer;
        }
        final HashSet<Integer> hash = new HashSet<Integer>();
        final double max = N / d;

        while(hash.size()<N) {
            final double a = rand.nextDouble();
            final int x = (int) Math.floor(a * max);
            hash.add(x);
        }
        int c = 0;
        int[] answer = new int[N];
        for(int x : hash)
            answer[c++] = x;
        Arrays.sort(answer);
        return answer;
    }

    /**
     * Generate a random array (sorted integer set).
     *
     * This is what Colantonio and Di Pietro called Zipfian.
     *
     * @param d should vary from 0 to 1.000
     * @return an array with a "Zipfian" distribution
     */
    public int[] getZipfian(double d) {
        ////////////////// (from arXiv version)
        //Similarly, in Zipfian sets, at each number generation, an
        //integer corresponding to floor(max * a^4) was added, where
        //max in [1.2 * 10^ 5, 6 * 10^9]. In this way, we generated skewed
        //data such that most of the integers were concentrated to lower
        //values, while numbers with high values were very sparse.
        //////////////////
        ////////////////
        // in  Colantonio and Di Pietro's arXiv version of their  paper at the
        // end of page 3, they clearly state that it is a^4
        // However, the version that they published (IPL) states a^2.
        ////////////////
        if((d<0) || (d>1.000)) throw new IllegalArgumentException("parameter should be in [0.005,0.999]");
        if(d>=0.99) {
            int[] answer = new int[N];
            for(int k = 0; k<N; ++k) answer[k] = k;
            return answer;
        }
        final double max = N / d;
        HashSet<Integer> hash = new HashSet<Integer>();
        int loopcount = 0;
        while(hash.size()<N) {
            final double a = rand.nextDouble();
            final int x = (int) Math.floor(a*a * max);
            hash.add(x);
            if(loopcount++ > 10*N) throw new RuntimeException("zipfian generation is too expensive");
        }
        int c = 0;
        int[] answer = new int[N];
        for(int x : hash)
            answer[c++] = x;
        Arrays.sort(answer);
        return answer;
    }

}