public class prefixsum {
    
    public static void delta(int[] x) {
        for(int i = x.length - 1; i - 1 > 0; --i) 
          x[i] = x[i] - x[i-1];
    }

    public static void prefixsum(int[] x) {
        for(int i = 1; i < x.length; ++i) 
          x[i] = x[i] + x[i-1];
    }
    
    
    public static void main(String[] args) {
        int N = 64000;
        int[] x = new int[N];
        for(int k = 0; k < x.length; ++k) 
          x[k] = k * k - k;
        for(int j = 0; j < 100 ; ++j) {
          delta(x);
          prefixsum(x);
        }
        for(int j = 0; j < 10 ; ++j) {   
          long t1 = System.nanoTime();
          delta(x);
          long t2 = System.nanoTime();
          prefixsum(x);
          long t3 = System.nanoTime();
          System.out.println(N*1.0/(t3-t2)+" "+N*1.0/(t2-t1));
        }
        for(int k = 0; k < x.length; ++k) 
          if(  x[k] != k * k - k) throw new RuntimeException("bug");
    }
}
