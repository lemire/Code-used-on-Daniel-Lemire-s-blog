package me.lemire.microbenchmarks.algorithms;

import java.util.*;
import java.util.concurrent.TimeUnit;
import java.util.stream.Collectors;

import org.openjdk.jmh.annotations.*;
import org.openjdk.jmh.runner.Runner;
import org.openjdk.jmh.runner.RunnerException;
import org.openjdk.jmh.runner.options.Options;
import org.openjdk.jmh.runner.options.OptionsBuilder;

@State(Scope.Benchmark)
@BenchmarkMode(Mode.AverageTime)
@OutputTimeUnit(TimeUnit.MICROSECONDS)
public class HashFast {
   int smallN = 100;
   HashSet<List<Integer>> small_hm = new HashSet<List<Integer>>(smallN);
   HashSet<BufferedList> small_bhm = new HashSet<BufferedList>(smallN);
   int counter = 0;

    @State(Scope.Benchmark)
    public static class BenchmarkState {
        int N = 10000000;
        int sN = 100000;


        HashSet<List<Integer>> hm = new HashSet<List<Integer>>();
        HashSet<BufferedList> bhm = new HashSet<BufferedList>();
        HashSet<List<Integer>> shm = new HashSet<List<Integer>>();
        HashSet<BufferedList> sbhm = new HashSet<BufferedList>();


       public BenchmarkState() {
         for(int k = 0; k < sN; k++) {
            List<Integer> s = new ArrayList<Integer>(3);
            for(int z = 0; z < 3; z++) s.add(k * z - 2);
            shm.add(s);
            sbhm.add(new BufferedList(s));
          }
          for(int k = 0; k < N; k++) {
            List<Integer> s = new ArrayList<Integer>(3);
            for(int z = 0; z < 3; z++) s.add(k * z - 2);
            hm.add(s);
            bhm.add(new BufferedList(s));
          }
         }
    }
    @Setup(Level.Iteration)
    public void start() {
          small_hm.clear();
          small_bhm.clear();
          counter++;
          for(int k = 0; k < smallN; k++) {
            List<Integer> s = new ArrayList<Integer>(3);
            for(int z = 0; z < 3; z++) s.add(k * z * counter + 2);
            small_hm.add(s);
            small_bhm.add(new BufferedList(s));
          }
    }
    
    @Benchmark
    public int small_test_contains(BenchmarkState s) {
        int count = 0;
        for(List<Integer> st : small_hm) {
          if(s.shm.contains(st)) count++;
        }
        return count;
    }

    @Benchmark
    public int small_test_contains_buffered(BenchmarkState s) {
        int count = 0;
        for(BufferedList st : small_bhm) {
          if(s.sbhm.contains(st)) count++;
        }
        return count;
    }

    @Benchmark
    public int justhash(BenchmarkState s) {
        int count = 0;
        for(List<Integer> st : small_hm) {
          count+= st.hashCode();
        }
        return count;
    }

    @Benchmark
    public int test_contains(BenchmarkState s) {
        int count = 0;
        for(List<Integer> st : small_hm) {
          if(s.hm.contains(st)) count++;
        }
        return count;
    }

    @Benchmark
    public int test_contains_buffered(BenchmarkState s) {
        int count = 0;
        for(BufferedList st : small_bhm) {
          if(s.bhm.contains(st)) count++;
        }
        return count;
    }
    public static void main(String[] args) throws RunnerException {
        Options opt = new OptionsBuilder()
        .include(HashFast.class.getSimpleName()).warmupIterations(5)
        .measurementIterations(5).forks(1).build();
        new Runner(opt).run();
    }

}


class BufferedList {
  private List<Integer> list;
  private int hash;
  public BufferedList(List<Integer> s) {
    list = s;
    hash = s.hashCode();
  }
  public int hashCode() {
    return hash;
  }
  public boolean equals(Object o) {
    if(o instanceof BufferedList)
      return ((BufferedList)o).list.equals(list);
    return false;
  }
}
