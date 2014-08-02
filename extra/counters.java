import java.util.*;

public class counters {
    
    public static int[] merge1(int N, int T, int[][] arrays) {
        int[] C= new int[N];
        HashSet<Integer> s = new HashSet<Integer>();
        for(int k = 0; k < arrays.length; ++k) {
            for(int v : arrays[k]) {
                C[v]++;
                if(C[v] == T) s.add(v);
            }
        }
        int[] answer = new int[s.size()];
        int pos = 0;
        for(int v: s)
        answer[pos++] = v;
        return answer;
    }

    public static int[] merge2(int N, int T, int[][] arrays) {
        int[] C= new int[N];
        for(int k = 0; k < arrays.length; ++k) {
            for(int v : arrays[k]) {
                C[v]++;
            }
        }
        ArrayList<Integer> s = new ArrayList<Integer>();
        for(int k = 0; k<N;++k)
          if(C[k]>T) s.add(k);
        int[] answer = new int[s.size()];
        int pos = 0;
        for(int v: s)
        answer[pos++] = v;
        return answer;
    }
    
    
    public static int[] merge3(int N, int T, int[][] arrays) {
        char[] C= new char[N];
        for(int k = 0; k < arrays.length; ++k) {
            for(int v : arrays[k]) {
                C[v]++;
            }
        }
        ArrayList<Integer> s = new ArrayList<Integer>();
        for(int k = 0; k<N;++k)
          if(C[k]>T) s.add(k);
        int[] answer = new int[s.size()];
        int pos = 0;
        for(int v: s)
        answer[pos++] = v;
        return answer;
    }
    
    
    public static int[] merge4(long[] C, int index, int T, int[][] arrays) {
        HashSet<Integer> s = new HashSet<Integer>();
        for(int k = 0; k < arrays.length; ++k) {
            for(int v : arrays[k]) {
                if((C[v]>> 32) != index) 
                  C[v] = ((long) index << 32) + 1;
                else 
                  C[v] = ((int) C[v] + 1) + ((long) index << 32);
                if((int) C[v] == T) s.add(v);
            }
        }
        int[] answer = new int[s.size()];
        int pos = 0;
        for(int v: s)
        answer[pos++] = v;
        return answer;
    }
    
    public static void main(String[] args) {
        int N = 1000*1000*100;
        Random r = new Random();
        int[][] arrays = new int[100][10];
        for(int k = 0; k < arrays.length; ++k) {
            for(int l = 0; l<arrays[k].length; ++l) 
              arrays[k][l] = r.nextInt(N);
        }
        long[] C= new long[N];
        long t1 = System.currentTimeMillis();
        for(int k = 0; k<5; ++k)
        System.out.println(merge1(N,2,arrays).length);
        long t2 = System.currentTimeMillis();
        for(int k = 0; k<5; ++k)
        System.out.println(merge2(N,2,arrays).length);
        long t3 = System.currentTimeMillis();
        for(int k = 0; k<5; ++k)
        System.out.println(merge3(N,2,arrays).length);
        long t4 = System.currentTimeMillis();
        for(int k = 0; k<5; ++k)
        System.out.println(merge4(C,k,2,arrays).length);
        long t5 = System.currentTimeMillis();

        System.out.println("timings using hash "+(t2-t1));
        System.out.println("timings using naive "+(t3-t2));
        System.out.println("timings using packed naive "+(t4-t3));
        System.out.println("timings using packed naive "+(t5-t4));

    }
}