import java.util.Random;

public class card {

    public static void bitwiseor(int[] input1, int[] input2, int[] output) {
        for(int k = 0; k < input1.length; ++k) {
            output[k] = input1[k] | input2[k];
        }
    }

    public static void bitwiseor(long[] input1, long[] input2, long[] output) {
        for(int k = 0; k < input1.length; ++k) {
            output[k] = input1[k] | input2[k];
        }
    }


    public static int card(int[] input1) {
        int card = 0;
        for(int k = 0; k < input1.length; ++k) {
            card += Integer.bitCount(input1[k]);
        }
        return card;
    }

    public static int card(long[] input1) {
        int card = 0;
        for(int k = 0; k < input1.length; ++k) {
            card += Long.bitCount(input1[k]);
        }
        return card;
    }

    public static int bitwiseorcard(int[] input1, int[] input2) {
        int card = 0;
        for(int k = 0; k < input1.length; ++k) {
            card += Integer.bitCount(input1[k] | input2[k]);
        }
        return card;
    }

    public static int bitwiseorcard(long[] input1, long[] input2) {
        int card = 0;
        for(int k = 0; k < input1.length; ++k) {
            card += Long.bitCount(input1[k] | input2[k]);
        }
        return card;
    }


    public static int bitwiseorcard(int[] input1, int[] input2, int[] output) {
        int card = 0;
        for(int k = 0; k < input1.length; ++k) {
            output[k] = input1[k] | input2[k];
            card += Integer.bitCount(output[k]);
        }
        return card;
    }

    public static int bitwiseorcard(long[] input1, long[] input2, long[] output) {
        int card = 0;
        for(int k = 0; k < input1.length; ++k) {
            output[k] = input1[k] | input2[k];
            card += Long.bitCount(output[k]);
        }
        return card;
    }

    public static void test(final int N) {
        long[] linput1 = new long[N/64];
        long[] linput2 = new long[N/64];
        long[] loutput = new long[N/64];
        Random r = new Random();
        for(int k = 0; k<N; k+=64) {
            int i1 = r.nextInt();
            int i2 = r.nextInt();
            int i3 = r.nextInt();
            int i4 = r.nextInt();
            linput1[k/64] = ((long) i2 << 32)|i1;
            linput2[k/64] = ((long) i4 << 32)|i3;
        }
        int times = 10000*((1<<22)/N);
        int mintimes = 2;
        int bogus = 0;
        long cost = Long.MAX_VALUE;
        for(int z = 0; z<mintimes; ++z) {
            long t1 = System.currentTimeMillis();
            for(int k = 0; k < times; ++k) {
                bitwiseor(linput1,linput2,loutput);
                bogus +=  loutput[10];
            }
            long t2 = System.currentTimeMillis();
            if(t2-t1 < cost) cost = t2-t1;
        }
        long cost2 = Long.MAX_VALUE;
        for(int z = 0; z<mintimes; ++z) {
            long t1 = System.currentTimeMillis();
            for(int k = 0; k < times; ++k) {
                bitwiseor(linput1,linput2,loutput);
                bogus +=  card(loutput);
            }
            long t2 = System.currentTimeMillis();
            if(t2-t1 < cost2) cost2 = t2-t1;
        }
        long cost3 = Long.MAX_VALUE;
        for(int z = 0; z<mintimes; ++z) {
            long t1 = System.currentTimeMillis();
            for(int k = 0; k < times; ++k) {
                bogus += bitwiseorcard(linput1,linput2,loutput);
            }
            long t2 = System.currentTimeMillis();
            if(t2-t1 < cost3) cost3 = t2-t1;
        }
        for(int z = 0; z<mintimes; ++z) {
            long t1 = System.currentTimeMillis();
            for(int k = 0; k < times; ++k) {
                bitwiseor(linput1,linput2,loutput);
                bogus +=  loutput[10];
            }
            long t2 = System.currentTimeMillis();
            if(t2-t1 < cost) cost = t2-t1;
        }
        for(int z = 0; z<mintimes; ++z) {
            long t1 = System.currentTimeMillis();
            for(int k = 0; k < times; ++k) {
                bitwiseor(linput1,linput2,loutput);
                bogus +=  card(loutput);
            }
            long t2 = System.currentTimeMillis();
            if(t2-t1 < cost2) cost2 = t2-t1;
        }
        for(int z = 0; z<mintimes; ++z) {
            long t1 = System.currentTimeMillis();
            for(int k = 0; k < times; ++k) {
                bogus += bitwiseorcard(linput1,linput2,loutput);
            }
            long t2 = System.currentTimeMillis();
            if(t2-t1 < cost3) cost3 = t2-t1;
        }
        System.out.println("Number of integers         "+N/64);
        System.out.println("bitwise or (64-bit)        "+(N/64)*0.001*times/cost+" millions integers per second");
        System.out.println("bitwise or (64-bit) + card "+(N/64)*0.001*times/cost2+" millions integers per second");
        System.out.println("bitwise or (64-bit) w card "+(N/64)*0.001*times/cost3+" millions integers per second");
        System.out.println("ratio bitwise or + card over bitwise or = "+ (double)cost2/cost);
        System.out.println("ratio bitwise or w card over bitwise or = "+ (double)cost3/cost);
        System.out.println("bogus = "+bogus);
    }


    public static void main(String[] args) {
        for(int k = 0; k < 10; ++k) {
            test(1<<21);
            System.out.println();
        }


    }

}
