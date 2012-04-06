/**
* Sorting is fast and useful
* http://lemire.me/blog/archives/2010/05/20/sorting-is-fast-and-useful/
*/

import java.util.*;

public class Stupid {
	
	public static void run() {	
     for(int N = 32; N<=1048576;N*=2) {
		int[] x = new int[N];
		Random rand = new Random();
		for(int k = 0;k < N; ++k) x[k]=rand.nextInt();
		Arrays.sort(x);
		HashSet<Integer> hs = new HashSet<Integer>();
		for(int k = 0;k < N; ++k) hs.add(x[k]);
		TreeSet<Integer> ts = new TreeSet<Integer>();
		for(int k = 0;k < N; ++k) ts.add(x[k]);
		int howmanyqueries = 1000000;
		int[] whichvalues = new int[howmanyqueries];
		for(int k = 0;k<howmanyqueries; ++k)
		  whichvalues[k] = rand.nextInt();
        long aft1 = System.currentTimeMillis();
		for(int t=1;t<10;++t) for(int wv : whichvalues) Arrays.binarySearch(x,wv);
        long aft2 = System.currentTimeMillis();
		for(int t=1;t<10;++t) for(int wv : whichvalues) hs.contains(wv);
        long aft3 = System.currentTimeMillis();
		for(int t=1;t<10;++t) for(int wv : whichvalues) ts.contains(wv);
        long aft4 = System.currentTimeMillis();
	    System.out.println(N+" "+(aft2-aft1)/(1000.0*howmanyqueries)+ " "+(aft3-aft2)/(1000.0*howmanyqueries)+ " "+(aft4-aft3)/(1000.0*howmanyqueries));
     }
	}
	
	public static void main(String [] a) {
		run();
	}
}