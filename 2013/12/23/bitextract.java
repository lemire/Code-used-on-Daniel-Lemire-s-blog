import java.util.*;

public class bitextract {
  
  // inspired by http://www.steike.com/code/bits/debruijn/	
  public static int bitscan1(long[] bitmaps, int[] output) {
	  int pos = 0;
      for(int k = 0; k < bitmaps.length; ++k) {
      	 long bitset = bitmaps[k];
         while (bitset != 0) {
           final long t = bitset & -bitset;
           output[pos++] = k * 64 +  Long.bitCount(t-1);
           bitset ^= t;
         }
      }
      return pos;
  }
  
  public static int bitscan2(long[] bitmaps, int[] output) {
	  int pos = 0;
      for(int k = 0; k < bitmaps.length; ++k) {  
        long data = bitmaps[k];
        while (data != 0) {
          final int ntz = Long.numberOfTrailingZeros(data);
          output[pos++] = k * 64 + ntz;
          data ^= (1l << ntz);
        }
      }
      return pos;
  }
  
  
  
  // inspired by http://www.steike.com/code/bits/debruijn/	
  public static int bitscan3(long[] bitmaps, int[] output) {
  	  final   int[] table = {0,1,2,7,3,13,8,19,4,25,14,28,9,34,20,40,5,17,26,38,15,46,29,48,10,31,35,54,21,50,41,57,63,6,12,18,24,27,33,39,16,37,45,47,30,53,49,56,62,11,23,32,36,44,52,55,61,22,43,51,60,42,59,58};
	  int pos = 0;
      for(int k = 0; k < bitmaps.length; ++k) {
      	 long bitset = bitmaps[k];
         while (bitset != 0) {
           final long t = bitset & -bitset;
           output[pos++] = table[(int)(t * 0x0218a392cd3d5dbfL) >>> 58];
           bitset ^= t;
         }
      }
      return pos;
  }
  


  public static void main(String[] args) {
  	  int N = 100000;
  	  for(int sb = 1; sb<32;++sb) {
  	    int setbitsmax = sb*N;
		long[] bitmap = new long[N];
		java.util.Random r = new java.util.Random();
		for (int k = 0; k < setbitsmax; ++k) {
			int bit = Math.abs(r.nextInt()) % (N*64);
			if(bit>=N*64) throw new RuntimeException("what?");
			bitmap[bit/64] |= (1L<<(bit%64));
		}
		int bitcount = 0;
		for(int k = 0; k < bitmap.length; ++k) {
			bitcount += Long.bitCount(bitmap[k]);
		} 

		int[] output = new int[bitcount];
		for(int t = 0; t<3; ++t) {
		  long bef1 = System.nanoTime();
		  int c1 = 0;
		  for(int t1=0;t1<100;++t1)
		    c1 = bitscan1(bitmap,output);
		  long aft1 = System.nanoTime();
		  long bef2 = System.nanoTime();
		  int c2 = 0;
		  for(int t1=0;t1<100;++t1)
		    c2 = bitscan2(bitmap,output);
		  long aft2 = System.nanoTime();
		  if(c1 != c2) throw new RuntimeException("bug2");
		  long bef3 = System.nanoTime();
		  int c3 = 0;
		  for(int t1=0;t1<100;++t1)
		    c3 = bitscan3(bitmap,output);
		  long aft3 = System.nanoTime();
		  if(c1 != c3) throw new RuntimeException("bug3");
		  if(t==2)
		    System.out.println(sb+" " +(aft1-bef1)/(1000*1000.0)+" "+(aft2-bef2)/(1000*1000.0)+" "+(aft3-bef3)/(1000*1000.0));
		}
  	  }

  }
  
}