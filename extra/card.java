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
    
    public static void test() {
            final int N = (1<<16);// number of bits
        int[] input1 = new int[N/32];
        int[] input2 = new int[N/32];
        int[] output = new int[N/32];
        long[] linput1 = new long[N/64];
        long[] linput2 = new long[N/64];
        long[] loutput = new long[N/64];
        Random r = new Random();
        for(int k = 0; k<N; k+=64) {
            int i1 = r.nextInt();
            int i2 = r.nextInt();
            int i3 = r.nextInt();
            int i4 = r.nextInt();
            input1[k/32] = i1;
            input1[k/32 + 1] = i2;
            input2[k/32] = i3;
            input2[k/32 + 1] = i4;
            linput1[k/64] = ((long) i2 << 32)|i1;
            linput2[k/64] = ((long) i4 << 32)|i3;
        }
        int times = 100000;
        int bogus = 0;
        long t1 = System.currentTimeMillis();
        for(int k = 0; k < times; ++k) {
          bitwiseor(input1,input2,output);
          bogus +=  output[10];
        }
        long t2 = System.currentTimeMillis();
        for(int k = 0; k < times; ++k) {
          bitwiseor(linput1,linput2,loutput);
          bogus +=  loutput[10];
        }
        long t3 = System.currentTimeMillis();
        for(int k = 0; k < times; ++k) {
          bitwiseor(input1,input2,output);
          bogus +=  card(output);
        }
        long t4 = System.currentTimeMillis();
        for(int k = 0; k < times; ++k) {
          bitwiseor(linput1,linput2,loutput);
          bogus +=  card(loutput);
        }
        long t5 = System.currentTimeMillis();
        for(int k = 0; k < times; ++k) {
          bogus += bitwiseorcard(input1,input2,output);
          bogus += output[10];
        }
        long t6 = System.currentTimeMillis();
        for(int k = 0; k < times; ++k) {
          bogus += bitwiseorcard(linput1,linput2,loutput);
          bogus +=  loutput[10];
        }
        long t7 = System.currentTimeMillis();
        for(int k = 0; k < times; ++k) {
          bogus += bitwiseorcard(input1,input2);
        }
        long t8 = System.currentTimeMillis();
        for(int k = 0; k < times; ++k) {
          bogus += bitwiseorcard(linput1,linput2);
        }
        long t9 = System.currentTimeMillis();
        System.out.println("bitwise or (32-bit)        "+(t2-t1));
        System.out.println("bitwise or (64-bit)        "+(t3-t2));
        System.out.println("bitwise or (32-bit) + card "+(t4-t3));
        System.out.println("bitwise or (64-bit) + card "+(t5-t4));
        System.out.println("bitwise or (32-bit) w card "+(t6-t5));
        System.out.println("bitwise or (64-bit) w card "+(t7-t6));
        System.out.println("bitwise or (32-bit) j card "+(t8-t7));
        System.out.println("bitwise or (64-bit) j card "+(t9-t8));
        System.out.println("bogus = "+bogus);    
    }

    
    public static void main(String[] args) {
        for(int k = 0; k < 5; ++k) {
          test();
          System.out.println();
        }


    }
    
}
