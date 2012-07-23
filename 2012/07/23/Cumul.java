public class Cumul {
	
	public static void sum(int[] data) {
		if(data.length == 0) return;
		for( int i = 1; i !=  data.length; ++i)
		  data[i] += data[i-1];
	}
	
	

    public static int[]  givemeanarray(int N) {
    	int[] bigarray = new int[N];
        for(int k = 0; k<N; ++k)
          bigarray[k] = k+k/3;
        return bigarray;
     }
     
     public static void test(int N) {
     	for(int t = 0; t <10; ++t) {
     		int[] data = givemeanarray(N);
     		long bef = System.currentTimeMillis();
     		sum(data);
     		long aft = System.currentTimeMillis();
     		System.out.println(N/(1000.0 * (aft-bef)));
     	}
     }
     
     public static void main(String[] args) {
     	test( 50 * 1000 * 1000 );
     }


}
