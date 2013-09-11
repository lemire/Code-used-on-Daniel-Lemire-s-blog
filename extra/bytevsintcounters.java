public class bytevsintcounters {
	
	public static void main(String[] args) {
		int S = 1024;
		
		byte[] counter8 = new byte[S];
		int[] counter32 = new int[S];
		long[] counter64 = new long[S];
			
		
		long bef,aft;
		bef = System.currentTimeMillis();
		for(int init = 0; init < 1024; ++init) 
		for(int i = init; i < 3*S; ++i)
		  counter8[i%S] += 1;
		aft = System.currentTimeMillis();
		long t1 = aft-bef;
		bef = System.currentTimeMillis();
		for(int init = 0; init < 1024; ++init) 
		for(int i = init; i < 3*S; ++i)
		  counter32[i%S] += 1;
		aft = System.currentTimeMillis();
		long t2 = aft-bef;
		bef = System.currentTimeMillis();
		for(int init = 0; init < 1024; ++init) 
		for(int i = init; i < 3*S; ++i)
		  counter64[i%S] += 1;
		aft = System.currentTimeMillis();
		long t3  = aft-bef;
			
		System.out.println(t1+" "+t2+" "+t3);
	}
}
