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
            if (counters[pos] >= T) 
                  result |= (1L<<pos);
          }
          return result;
	}

	public static long threshold3(int T, long[] buffers) {
		if(buffers.length ==0 ) return 0;
		long[] v = new long[T];
		v[0] = buffers[0];
        for(int k = 1 ; k < buffers.length; ++k) {
            long c = buffers[k];
            for(int j = T-1; j >= 1; --j) {
                long ttemp = c;
                ttemp &= v[j-1];
                v[j] |= ttemp;
            }
            v[0]|=c;
        }
        return v[T-1];
    }
	
	public static long threshold4(int T, long[] buffers) {
		int B = 0;
		for(int k = 0; k<buffers.length; ++k)
		  B += Long.bitCount(buffers[k]);
		if (B>=buffers.length*8 ) 
		  return threshold3(T,buffers);
		else 
		  return threshold2(T,buffers);
    }
	
	public static void test(long w, int N, int T,int r) {



	  long bogus1 = 0;
	  long bogus2 = 0;
	  long bogus3 = 0;
	  long bogus4 = 0;
	  
	  long[][] array = new long[r][N];
	  for(int k = 0; k<r;++k)
	    for(int z = 0; z<N;++z)
	      array[k][z] = w;
	  for(int k = 0; k<r;++k) {
	      if(threshold1(T,array[k])!=threshold3(T,array[k])) {
	      	throw new RuntimeException("bug 13");
	      }
	      if(threshold1(T,array[k])!=threshold2(T,array[k])) {
	      	throw new RuntimeException("bug 12");
	      }

	  }
	  for(int time = 0; time<3;++time) {

		long bef1 = System.currentTimeMillis();
		for(int k = 0; k<r;++k)
	      bogus1 += threshold1(T,array[k]);
		long aft1 = System.currentTimeMillis();
	    long bef2 = System.currentTimeMillis();
	    for(int k = 0; k<r;++k)
	      bogus2 += threshold2(T,array[k]);
		long aft2 = System.currentTimeMillis();
	    long bef3 = System.currentTimeMillis();
	    for(int k = 0; k<r;++k)
	      bogus3 += threshold3(T,array[k]);
		long aft3 = System.currentTimeMillis();
	    long bef4 = System.currentTimeMillis();
	    for(int k = 0; k<r;++k)
	      bogus4 += threshold4(T,array[k]);
		long aft4 = System.currentTimeMillis();

        System.out.println((aft1 - bef1)+" "+(aft2 - bef2)+" "+(aft3 - bef3)+" "+(aft4 - bef4));
	  }
	  System.out.println("ignore:"+bogus1+" "+bogus2+" "+bogus3+" "+bogus4);

	}
	
	public static void main(String[] args) {
	    int r = 16*1024;
	    int N = 128;
	    int T = 63;
		System.out.println("low density, T="+T+" N="+N);
		test(1L<<5, N,T,r);
        System.out.println("medium density, T="+T+" N="+N);
		test((1L<<5)|(1L<<6)|(1L<<9)|(1L<<16)|(1L<<30), N,T,r);
        System.out.println("high density, T="+T+" N="+N);
		test(~((1L<<5)|(1L<<6)|(1L<<9)|(1L<<16)|(1L<<30)),N,T,r);
		//
	    N = 128;
	    T = 12;
	    r = 16*1024;
        System.out.println("low density, T="+T+" N="+N);
		test(1L<<5, N,T,r);
        System.out.println("medium density, T="+T+" N="+N);
		test((1L<<5)|(1L<<6)|(1L<<9)|(1L<<16)|(1L<<30), N,T,r);
        System.out.println("high density, T="+T+" N="+N);
		test(~((1L<<5)|(1L<<6)|(1L<<9)|(1L<<16)|(1L<<30)),N,T,r);

		//
	    N = 1024;
	    T = 1023;
	    r = 16;
        System.out.println("low density, T="+T+" N="+N);
		test(1L<<5, N,T,r);
        System.out.println("medium density, T="+T+" N="+N);
		test((1L<<5)|(1L<<6)|(1L<<9)|(1L<<16)|(1L<<30), N,T,r);
        System.out.println("high density, T="+T+" N="+N);
		test(~((1L<<5)|(1L<<6)|(1L<<9)|(1L<<16)|(1L<<30)),N,T,r);
		
	}


}
