import java.util.*;
import java.text.DecimalFormat;

public class UniformDistinct {
	
	
	public static void main(String[] args) {
		int M = 20;
		int Max = 1<<M;
		DecimalFormat df = new DecimalFormat("0.###");

		for(int times = 0; times < 3; ++times) 
		
		for(int k = M-3; k< M; ++k) {
			int[] x;
			
			long bef,aft;
			
			long time1=0, time2=0, time3=0, time4=0;
			
			
			bef = System.nanoTime();
			for(int T = 0 ; T < 10; ++T) {
			x = generateUniform1(1<<k,Max);
			if(x.length!= 1<<k) throw new RuntimeException("bug");
			}
			aft = System.nanoTime();			
			time1 = aft-bef;
			
			bef = System.nanoTime();
			for(int T = 0 ; T < 10; ++T) {
			x = generateUniform2(1<<k,Max);
			if(x.length!= 1<<k) throw new RuntimeException("bug");
			}
		    aft = System.nanoTime();			
			time2 = aft-bef;
			
			bef = System.nanoTime();
			for(int T = 0 ; T < 10; ++T) {
		    x = generateUniform3(1<<k,Max,2048);
			if(x.length!= 1<<k) throw new RuntimeException("bug");
			}
			aft = System.nanoTime();			
			time3 = aft-bef;

			bef = System.nanoTime();
			for(int T = 0 ; T < 10; ++T) {
		    x = generateUniformReservoirSampling(1<<k,Max);
			if(x.length!= 1<<k) throw new RuntimeException("bug");
			}
			aft = System.nanoTime();			
			time4 = aft-bef;

			System.out.println(df.format(Max*1.0/(1<<k)*1.0)+" "+df.format((1<<k)*1000.0/time1)+" "+df.format((1<<k)*1000.0/time2)+" "+df.format((1<<k)*1000.0/time3)+" "+df.format((1<<k)*1000.0/time4));
		}
	}

  /**
  * output all integers from the range [0,Max) that are not
  * in the array
  */
  static int[] negate(int[] x, int Max) {
	int[] ans = new int[Max - x.length];
	int i = 0;
    int c = 0;
    for(int j = 0; j < x.length ; ++j) {
            	int v = x[j];
            	for(; i<v; ++i)
            	  ans[c++] = i;
            	++i;
     }
     while(c< ans.length)
       ans[c++] = i++;
     return ans;
  }
  /**
  * Generate N random integers from 0 to Max
  */
 static int[] generateUniform1(int N, int Max) {
        if (N > Max)
            throw new RuntimeException("not possible");
        int[] ans = new int[N];
        if (N == Max) {
            for (int k = 0; k < N; ++k)
                ans[k]=k;
            return ans;
        }
        HashSet<Integer> s = new HashSet<Integer>();
        while (s.size() < N)
            s.add(new Integer(rand.nextInt(Max)));
        Iterator<Integer> i = s.iterator();
        for (int k = 0; k < N; ++k)
            ans[k]=i.next().intValue();
        Arrays.sort(ans);
        return ans;
    }
    
    
   static int[] generateUniformReservoirSampling(int N, int Max) {
        if (N > Max)
            throw new RuntimeException("not possible");
        int[] ans = new int[N];
        for (int k = 0; k < N; ++k)
                ans[k]=k;
        for(int k = N ; k < Max; ++k) {
        	int v = rand.nextInt(k+1);
        	if(v < N) {
        		ans[v] = k;
        	}
        }
        return ans;
    }
    
/**
  * Generate N random integers from 0 to Max
  */
 static int[] generateUniform2(int N, int Max) {
 	if(2*N > Max) 
 	  return negate(generateUniform1(Max-N, Max),Max);
 	return generateUniform1(N, Max);
 }

/**
  * Generate N random integers from 0 to Max
  */
 static int[] generateUniform3(int N, int Max, int T) {
 	if(T*N > Max) 
 	  return generateUniformBitmap(N, Max);
 	return generateUniform1(N, Max);
 }
static int[] generateUniformBitmap(int N, int Max) {
        if (N > Max)
            throw new RuntimeException("not possible");
        int[] ans = new int[N];
        if (N == Max) {
            for (int k = 0; k < N; ++k)
                ans[k]=k;
            return ans;
        }
        BitSet bs = new BitSet(Max);
        int cardinality = 0;
        while(cardinality < N) {
        	int v = rand.nextInt(Max);
        	if(!bs.get(v)) {
        		bs.set(v);
        		cardinality++;
        	}
        }
        int pos = 0;
        for (int i = bs.nextSetBit(0); i >= 0; i = bs.nextSetBit(i+1)) {
           ans[pos++] = i;
        }
        return ans;
    }


static Random rand = new Random();
 
 
 
}
