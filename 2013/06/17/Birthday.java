import java.util.Arrays;
import java.util.Random;
public class Birthday {
	
	public static boolean hasCollision(String[] source, StronglyUniversalHasher hasher) {
		int[] hashvalues = new int[source.length];
		for(int k = 0; k < hashvalues.length; ++k) {
			hashvalues[k] = hasher.hash(source[k]);
			}
		Arrays.sort(hashvalues);
		for(int k = 0; k < hashvalues.length - 1; ++k) {
			if(hashvalues[k] == hashvalues[k+1]) return true;
		}
		return false;
	}
	
	public static boolean hasCollision(String[] source, JavaHasher hasher) {
		int[] hashvalues = new int[source.length];
		for(int k = 0; k < hashvalues.length; ++k) {
			hashvalues[k] = hasher.hash(source[k]);
		}
		Arrays.sort(hashvalues);
		for(int k = 0; k < hashvalues.length - 1; ++k) {
			if(hashvalues[k] == hashvalues[k+1]) return true;
		}
		return false;
	}

	public static boolean idealhasCollisionGivenNoDuplicate(String[] source) {
		int[] hashvalues = new int[source.length];
	    Random r = new Random();
		for(int k = 0; k < hashvalues.length; ++k) {
			hashvalues[k] =r.nextInt();
		}
		Arrays.sort(hashvalues);
		for(int k = 0; k < hashvalues.length - 1; ++k) {
			if(hashvalues[k] == hashvalues[k+1]) return true;
		}
		return false;
	}
	
	
	public static String[] fancylistStrings(int N) {
		String[] values = { "Ace","BDe","AdF","BEF"};
		String[] answer = new String[N];
		for(int k = 0;k<4;++k) answer[k] = values[k];
		for(int k = 4;k<N;++k) 
		answer[k]=values[k%4]+toString(k);
		return answer;
	}
	
	public static String[] listStrings(int N) {
		String[] answer = new String[N];
		for(int k = 0;k<N;++k) 
		answer[k]=toString(k);
		return answer;
	}
	
	public static String toString(int z) {
		byte[] x = new byte[4];
		x[0] = (byte) (z>>>24);
		x[1] = (byte) (z>>>16);
		x[2] = (byte) (z>>>8);
		x[3] = (byte) (z);
		return new String(x);
	}
    public static void test(int N) {
    	System.out.println("N="+N);
    	String[] values = listStrings(N);
		Arrays.sort(values);
		for(int k = 0; k< values.length-1;++k)
		  if(values[k]==values[k+1]) throw new RuntimeException("duplicate found");
		int collisioncount = 0;
		Random r = new Random();
		int totalcount = 100;
		for(int k = 0; k < totalcount; ++k) {
			JavaHasher hasher = new JavaHasher();
			if(hasCollision(values,hasher)) ++collisioncount;
		}
		System.out.println("proba = "+collisioncount * 100.0 / totalcount);
		collisioncount = 0;
		long[] key = new long[4];
		for(int k = 0; k < totalcount; ++k) {
		    for(int z = 0; z<key.length;++z) key[z] = r.nextLong();
			StronglyUniversalHasher hasher = new StronglyUniversalHasher(key);
			if(hasCollision(values,hasher)) ++collisioncount;
		}
		System.out.println("proba = "+collisioncount * 100.0 / totalcount);
		collisioncount = 0;
		for(int k = 0; k < totalcount; ++k) {
			if(idealhasCollisionGivenNoDuplicate(values)) ++collisioncount;
		}
		System.out.println("proba = "+collisioncount * 100.0 / totalcount);

    }

	
	public static void main(String[] args) {
		test(30000);
		test(30500);
		test(31000);
		test(32000);
		test(33000);
		test(34000);
		test(34500);
		test(35000);
		
		test(40000);
		test(77163);
		test(100000);
	}
}


class StronglyUniversalHasher {
	long A[];
	public StronglyUniversalHasher (long[] a) {
		A = a;
	}
	
	public int hash(String s) {
			
		long hashvalue = A[0];
		int pos = 1;
		byte[] x = s.getBytes();
		int l = x.length / 4 * 4;

		int k = 0;
		for(; k + 4 <= l;) {
			int c =  x[k++] | (x[k++]<<8) | (x[k++]<<16) | (x[k++]<<24);
			hashvalue += A[pos++] * (0xFFFFFFFFl & c);

		}
		int c = 0;
		while(k<x.length) {
		  c |= x[k] << (8 * (k-l));
		  ++k;
		}
		c |= 1 <<(8 * (x.length-l));
	    hashvalue += A[pos++] * (0xFFFFFFFFl & c);
		return (int)(hashvalue>>>32);
	}

	
	public int hash(int x) {
		return  (int) ((A[1] * (0xFFFFFFFFl & x) + A[0]) >>> 32);
	}
}


class JavaHasher {
	public JavaHasher () {
	}
	
	public int hash(String s) {
		return s.hashCode();
	}

	
}