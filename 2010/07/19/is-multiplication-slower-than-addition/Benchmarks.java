/**
* Is multiplication slower than addition?
* http://lemire.me/blog/archives/2010/07/19/is-multiplication-slower-than-addition/
*/

public class Benchmarks {

	public static int product(long[] v1, long[] v2) {
		int sum=1;
        for(int j = 0; j<v1.length;++j)
          sum += v1[j] * v2[j];
        return sum;
    }
	public static int fewerMultiplications(long[] v1, long[] v2) {
		int sum=1;
        for(int j = 0; j<v1.length;j+=2)
          sum += (v1[j] + v2[j]) * ( v1[j+1] + v2[j+1] );
        return sum;
    }
	public static void main(String[] args) {
		int N= 1024*32;
		long[] v1 = new long[N];
		long[] v2 = new long[N];
		long before,after;
		for(int k = 0;k < 5;++k) { // we repeat to be sure
		before = System.currentTimeMillis();
		for(int loop = 1; loop<100000;++loop) product(v1,v2);
		after = System.currentTimeMillis();
		System.out.println("product "+(after-before));
		//
		before = System.currentTimeMillis();
		for(int loop = 1; loop<100000;++loop) fewerMultiplications(v1,v2);
		after = System.currentTimeMillis();
		System.out.println("fewer mults "+(after-before));
		System.out.println();
		}
	}
}