package me.lemire.microbenchmarks.hash;

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
public class OrderHash {



    @State(Scope.Benchmark)
    public static class BenchmarkState {
          @Param({"1000", "1000000", "10000000", "100000000"})
        public int N ;

        LinkedHashSet<Integer> lhs = new LinkedHashSet<Integer>();
        HashSet<Integer> hs = new HashSet<Integer>();
        TreeSet<Integer> ts = new TreeSet<Integer>();

        int[] array = new int[N];


        public BenchmarkState() {
          for(int k = 0 ; k < N ; k++) {
            lhs.add(k);
            hs.add(k);
            ts.add(k);
            array[k] = k;
          }
        }

    }

    @Benchmark
    public int scanLinkedHashSet(BenchmarkState s) {
      int sum = 0;
      for(int i : s.lhs) sum += i; // I know you could use lambdas, but why?
      return sum;
    }

    @Benchmark
    public int scanHashSet(BenchmarkState s) {
      int sum = 0;
      for(int i : s.hs) sum += i; // I know you could use lambdas, but why?
      return sum;
    }

    @Benchmark
    public int scanTreeSet(BenchmarkState s) {
      int sum = 0;
      for(int i : s.ts) sum += i; // I know you could use lambdas, but why?
      return sum;
    }

    @Benchmark
    public int scanArray(BenchmarkState s) {
      int sum = 0;
      for(int i : s.array) sum += i; // I know you could use lambdas, but why?
      return sum;
    }

    public static void main(String[] args) throws RunnerException {
       Options opt = new OptionsBuilder()
                .include(OrderHash.class.getSimpleName())
                .warmupIterations(5)
                .measurementIterations(5)
                .forks(1)
                .build();

        new Runner(opt).run();
    }
}
