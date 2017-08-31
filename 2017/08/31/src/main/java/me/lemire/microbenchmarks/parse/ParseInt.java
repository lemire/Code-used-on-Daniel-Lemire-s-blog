package me.lemire.microbenchmarks.parse;

import java.util.*;
import java.io.*;



import org.openjdk.jmh.annotations.*;
import org.openjdk.jmh.runner.*;
import org.openjdk.jmh.runner.options.*;

import java.util.*;
import java.util.concurrent.TimeUnit;

@State(Scope.Thread)
@BenchmarkMode(Mode.Throughput)
@OutputTimeUnit(TimeUnit.SECONDS)
public class ParseInt {


    @State(Scope.Benchmark)
    public static class BenchmarkState {
        public static int N = 2048;

        String myarray = new String();
        Random rand = new Random();
        public int nextQuery()  {
            return  rand.nextInt();
        }

        public BenchmarkState() {
          // this won't be fast
          for(int k = 0 ; k < N ; k++) {
            myarray += (rand.nextInt() % 1000000);
            if(k + 1 < N) myarray += ",";
          }
        }

    }

    @Benchmark
    public int[] scanner(BenchmarkState s) {
        Scanner sc = new Scanner(s.myarray).useDelimiter(",");
        ArrayList<Integer> al = new ArrayList<Integer>();
        while (sc.hasNextInt()) {
          al.add(sc.nextInt());
        }
        int[] ans = new int[al.size()];
        Iterator<Integer> iterator = al.iterator();
        for (int i = 0; i < ans.length; i++) {
          ans[i] = iterator.next().intValue();
        }
        if(ans.length != s.N) throw new RuntimeException("bug");
        return ans;
    }

    public static int countChar(String s, char c) {
      int l = s.length();
      int count = 0;
      for(int k = 0; k < l; k++)
        if(s.charAt(k) == c) count ++;
      return count;
    }

    @Benchmark
    public int[] split(BenchmarkState s) {
        String[] positions = s.myarray.split(",");
        int[] ans = new int[positions.length];
        for (int i = 0; i < positions.length; i++) {
          ans[i] = Integer.parseInt(positions[i]);
        }
        if(ans.length != s.N) throw new RuntimeException("bug");
        return ans;
    }

    // assume positive numbers, return -1 if not found
    private static int parseSubstring(String s, int begin, int len) {
      if(len == 0) return -1;
      char c = s.charAt(begin);
      int i = 0;
      while(!Character.isDigit(c) ) {
        i++;
        if(i == len) return -1;
        c = s.charAt(begin + i);
      }
      int ans = Character.digit(c,10);
      i++;
      for(; i < len; ++i) {
        c = s.charAt(i + begin);
        if(!Character.isDigit(c) ) {
          break;
        }
        ans = 10 * ans +  Character.digit(c,10);
      }
      return ans;
    }

    @Benchmark
    public int[] manualSplit(BenchmarkState s) {
      int maxsize = countChar(s.myarray, ',') + 1;
      int[] ans = new int[maxsize];
      int pos = 0;
      int l = s.myarray.length();
      for(int begin = 0; begin + 1 < l; ) {
        int next = begin + 1;
        while(next < l && s.myarray.charAt(next) != ',') next++;
        int v = parseSubstring(s.myarray, begin, next - begin);
        if(v >= 0) ans[pos++] = v;
        begin = next;
      }
      if(pos < maxsize) ans = Arrays.copyOf(ans,pos);
      if(ans.length != s.N) throw new RuntimeException("bug");
      return ans;
    }
    public static void main(String[] args) throws RunnerException {
       Options opt = new OptionsBuilder()
                .include(ParseInt.class.getSimpleName())
                .warmupIterations(5)
                .measurementIterations(5)
                .forks(1)
                .build();

        new Runner(opt).run();
    }
}
