import java.util.*;

public class bitsetcounters {
	public static long threshold1(int T, long[] b) {
          long result = 0L;
          long[] buffers = Arrays.copyOf(b,b.length);
          for (int pos=0; pos < 64; ++pos) {
              result <<= 1;
              int count=0;
              for (int j = 0; j < buffers.length; ++j) {
                  if ( (buffers[j] & (1L<<63)) != 0) ++count;
                  buffers[j] <<= 1;
              }
              if (count >= T) 
                  result |= 1;
          }
          return result;
	}


	public static long threshold2(int T, long[] buffers) {
          long result = 0L;
          int[] counters = new int[64];
          for(long bitset : buffers) {
            while (bitset != 0) {
               long t = bitset & -bitset;
               counters[Long.bitCount(t-1)]++;
               bitset ^= t;
            }
          }
          for (int pos=0; pos < 64; ++pos) {
          	result <<= 1;
//          	System.out.println(pos+" "+counters[pos]);
            if (counters[pos] >= T) 
                  result |= 1;
          }
          return result;
	}
	
	public static void test(long w) {
	  final int N = 32;
	  final int T = 15;
	  final int r = 16*1024;

	  long bogus1 = 0;
	  long bogus2 = 0;
	  long[][] array = new long[r][N];
	  for(int k = 0; k<r;++k)
	    for(int z = 0; z<N;++z)
	      array[k][z] = w;
	  for(int time = 0; time<3;++time) {
		long bef1 = System.currentTimeMillis();
	    for(int k = 0; k<r;++k)
	      bogus1 += threshold1(T,array[k]);
		long aft1 = System.currentTimeMillis();
	    long bef2 = System.currentTimeMillis();
	    for(int k = 0; k<r;++k)
	      bogus2 += threshold2(T,array[k]);
		long aft2 = System.currentTimeMillis();
        System.out.println((aft1 - bef1)+" "+(aft2 - bef2));
	  }
	  System.out.println("ignore:"+bogus1+" "+bogus2);

	}
	
	public static void main(String[] args) {
		System.out.println("low density");
		test(1L<<5);
        System.out.println("medium density");
		test((1L<<5)|(1L<<6)|(1L<<9)|(1L<<16)|(1L<<30));
        System.out.println("high density");
		test(~((1L<<5)|(1L<<6)|(1L<<9)|(1L<<16)|(1L<<30)));

	}


}