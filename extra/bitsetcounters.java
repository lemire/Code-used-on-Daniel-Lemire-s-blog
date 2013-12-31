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
    private static int[] bufcounters = new int[64];

	public static long threshold2buf(int T, long[] buffers) {
          long result = 0L;
          int[] counters = bufcounters;
          Arrays.fill(counters,0);
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
		if(T>=128  ) 
		  return threshold2buf(T,buffers);
		int B = 0;
		for(int k = 0; k<buffers.length; ++k)
		  B += Long.bitCount(buffers[k]);
		if (2*B>=buffers.length*T ) 
		  return threshold3(T,buffers);
		else 
		  return threshold2buf(T,buffers);
    }
	
	public static void test(long w, int N, int T,int r) {

	  long bogus1 = 0;
	  long bogus2 = 0;
	  long bogus2buf = 0;
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
	      if(threshold1(T,array[k])!=threshold2buf(T,array[k])) {
	      	throw new RuntimeException("bug 12buf");
	      }

	  }
	  for(int time = 0; time<1;++time) {

		long bef1 = System.currentTimeMillis();
		for(int k = 0; k<r;++k)
	      bogus1 += threshold1(T,array[k]);
		long aft1 = System.currentTimeMillis();
	    long bef2 = System.currentTimeMillis();
	    for(int k = 0; k<r;++k)
	      bogus2 += threshold2(T,array[k]);
		long aft2 = System.currentTimeMillis();
	    long bef2buf = System.currentTimeMillis();
	    for(int k = 0; k<r;++k)
	      bogus2buf += threshold2buf(T,array[k]);
		long aft2buf = System.currentTimeMillis();
	    long bef3 = System.currentTimeMillis();
	    for(int k = 0; k<r;++k)
	      bogus3 += threshold3(T,array[k]);
		long aft3 = System.currentTimeMillis();
	    long bef4 = System.currentTimeMillis();
	    for(int k = 0; k<r;++k)
	      bogus4 += threshold4(T,array[k]);
		long aft4 = System.currentTimeMillis();

        System.out.println((aft1 - bef1)+" "+(aft2 - bef2)+" "+(aft2buf - bef2buf)+" "+(aft3 - bef3)+" "+(aft4 - bef4));
	  }
	  //System.out.println("ignore:"+bogus1+" "+bogus2+" "+bogus2buf+" "+bogus3+" "+bogus4);

	}
	
	public static void testalldensities(int N, int T,int r) {
		long w = 0;
		for(int pos = 0; pos<64; ++pos) {
			w |= (1L << (~pos));
			if((pos+1) / 8 * 8 != pos+1) continue;
			System.out.print("density "+(pos+1)+": ");;
			test(w, N,T,r);
		} 
	}
	
	
	public static void main(String[] args) {
	    //int r = 16*1024;
	    //int N = 128;
	   for(int N = 16; N<=1024; N*=2) {
	    System.out.println("N="+N);
	    int r = 2097152/N;
	    for(int T = 1; T<N;T+=N/4) {
	    	System.out.println("T="+T);
	      testalldensities( N,  T, r);
	    }
	   }
	}


}
