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
@OutputTimeUnit(TimeUnit.MILLISECONDS)
public class HashFast {


    @State(Scope.Benchmark)
    public static class BenchmarkState {
        int N = 100000000;
        HashSet<List<Integer>> hm = new HashSet<List<Integer>>();
        HashSet<BufferedList> bhm = new HashSet<BufferedList>();

        int smallN = 10000;
        HashSet<List<Integer>> small_hm = new HashSet<List<Integer>>(smallN);
        HashSet<BufferedList> small_bhm = new HashSet<BufferedList>(smallN);
        public BenchmarkState() {
        
         for(int k = 0; k < N; k++) {
        	    List<Integer> s = new ArrayList<Integer>(3);
        	    for(int z = 0; z < 3; z++) s.add(k * z - 2);
        	    hm.add(s);
            bhm.add(new BufferedList(s));
          }
          for(int k = 0; k < smallN; k++) {
        	    List<Integer> s = new ArrayList<Integer>(3);
        	    for(int z = 0; z < 3; z++) s.add(k * z + 2);
        	    small_hm.add(s);
            small_bhm.add(new BufferedList(s));
          }
        }
    }
    
    @Benchmark
    public int justhash(BenchmarkState s) {
        int count = 0;
        for(List<Integer> st : s.small_hm) {
          count+= st.hashCode();
        }
        return count;
    }

    @Benchmark
    public int test_contains(BenchmarkState s) {
        int count = 0;
        for(List<Integer> st : s.small_hm) {
          if(s.hm.contains(st)) count++;
        }
        return count;
    }

    @Benchmark
    public int test_contains_buffered(BenchmarkState s) {
        int count = 0;
        for(BufferedList st : s.small_bhm) {
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
