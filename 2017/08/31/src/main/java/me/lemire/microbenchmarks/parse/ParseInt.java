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
        int[] expected;
        Random rand = new Random();
        public int nextQuery()  {
            return  rand.nextInt();
        }

        public BenchmarkState() {
          // this won't be fast
          expected = new int[N];
          for(int k = 0 ; k < N ; k++) {
            int val = (rand.nextInt() % 1000000);
            expected[k] = val;
            myarray += val;
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

    @Benchmark
    public int[] split(BenchmarkState s) {
        String[] positions = s.myarray.split(",");
        int[] ans = new int[positions.length];
        for (int i = 0; i < positions.length; i++) {
          ans[i] = Integer.parseInt(positions[i]);
        }
        if(ans.length != s.N) throw new RuntimeException("bug");
        if(! Arrays.equals(ans,s.expected)) throw new RuntimeException("bug");
        return ans;
    }


    @Benchmark
    public int[] monolithicSplit(BenchmarkState s) {
      final String text = s.myarray;
      final int length = text.length();
      final int limit = (length / 2) + 1;
      int[] array = new int[limit];
      int pos = 0, tmp = 0;
      boolean neg = false;
      for (int i = 0; i < length; i++) {
        char c = text.charAt(i);
        if (c == ',') {
          array[pos++] = neg ? 0 - tmp : tmp;
          tmp = 0;
          neg = false;
        } else if (c == '-') {
          neg = true;
        } else {
          tmp = (tmp << 3) + (tmp << 1) + (c & 0xF);
        }
      }
      array[pos++] = neg ? 0 - tmp : tmp;
      if(pos != s.N) throw new RuntimeException("bug");
      int[] result = new int[pos];
      System.arraycopy(array, 0, result, 0, pos);
      return result;
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
