import java.text.*;
public class bytevsintcounters {
	
	
  static int counter8(int S) {
     byte[]  counter = new byte[S];	    
     int gap = 3 * S/(64*1024);
     for(int init = 1; init < 1024; ++init) 
        for (int i=init; i<S*3; i+=gap) {
        	counter[i%S] += 1;// ignore overflows
        }
     int c = 0;
     for(int i = 0; i < S; ++i)
        if(counter[i]>2) ++c;
     return c;

   }
   
  static int counter16(int S) {
     short[]  counter = new short[S];	    
     int gap = 3 * S/(64*1024);
     for(int init = 1; init < 1024; ++init) 
        for (int i=init; i<S*3; i+=gap) {
        	counter[i%S] += 1;// ignore overflows
        }
     int c = 0;
     for(int i = 0; i < S; ++i)
        if(counter[i]>2) ++c;
     return c;

   }
   
  static int counter32(int S) {
     int[]  counter = new int[S];	    
     int gap = 3 * S/(64*1024);
     for(int init = 1; init < 1024; ++init) 
        for (int i=init; i<S*3; i+=gap) {
        	counter[i%S] += 1;// ignore overflows
        }
     int c = 0;
     for(int i = 0; i < S; ++i)
        if(counter[i]>2) ++c;
     return c;

   }
   
  static int counter64(int S) {
     long[]  counter = new long[S];	    
     int gap = 3 * S/(64*1024);
     for(int init = 1; init < 1024; ++init) 
        for (int i=init; i<S*3; i+=gap) {
        	counter[i%S] += 1;// ignore overflows
        }
     int c = 0;
     for(int i = 0; i < S; ++i)
        if(counter[i]>2) ++c;
     return c;

   }
   public static void test(int S) {
   	    System.out.println("Number of counters = "+S);
        int bogus = 0;
		long bef,aft;
		bef = System.currentTimeMillis();
		bogus += counter8(S);
		aft = System.currentTimeMillis();
		long t1 = aft-bef;
		bef = System.currentTimeMillis();
		bogus += counter16(S);
		aft = System.currentTimeMillis();
		aft = System.currentTimeMillis();
		long t2 = aft-bef;
		bef = System.currentTimeMillis();
		bogus += counter32(S);
		aft = System.currentTimeMillis();
		long t3  = aft-bef;
		bef = System.currentTimeMillis();
		bogus += counter32(S);
		aft = System.currentTimeMillis();
		long t4  = aft-bef;
			
        int gap = 3 * S/(64*1024);
		
        double number = (1024.0 * S)/gap;
		DecimalFormat df = new DecimalFormat( "#.#" );

		System.out.println(df.format(number*0.001/t1)+" "+df.format(number*0.001/t2)+" "+df.format(number*0.001/t3)+" "+df.format(number*0.001/t4));
		System.out.println("ignore :"+bogus);
   }
	
	public static void main(String[] args) {
	   System.out.println(" speeds for 8-bit, 16-bit, 32-bit and 64-bit counters");
	   test(1024*64);	
	   test(1024*1024*8);	
	   test(1024*1024*32);	
	   test(1024*1024*64);	
	}
}
