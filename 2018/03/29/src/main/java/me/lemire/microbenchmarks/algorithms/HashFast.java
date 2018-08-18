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
   ArrayList<Triple> small_hm = new ArrayList<Triple>(smallN);
   ArrayList<BufferedTriple> small_bhm = new ArrayList<BufferedTriple>(smallN);
   int counter = 0;

    @State(Scope.Benchmark)
    public static class BenchmarkState {
        int N = 10000000;


        HashSet<Triple> hm = new HashSet<Triple>();
        HashSet<BufferedTriple> bhm = new HashSet<BufferedTriple>();

       public BenchmarkState() {
          for(int k = 0; k < N; k++) {
            hm.add(new Triple( - 2, k - 2, 2 * k - 2 ));
            bhm.add(new BufferedTriple( - 2, k - 2, 2 * k - 2));
          }
         }
    }
    @Setup(Level.Iteration)
    public void start() {
          small_hm.clear();
          small_bhm.clear();
          counter++;
          for(int k = 0; k < smallN; k++) {
            small_hm.add(new Triple( - 2 * counter, k * counter - 2, 2 * k * counter - 2 ));
            small_bhm.add(new BufferedTriple( - 2 * counter, k * counter - 2, 2 * k * counter - 2));
          }
    }
    @Benchmark
    public int test_contains(BenchmarkState s) {
        int count = 0;
        for(Triple st : small_hm) {
          if(s.hm.contains(st)) count++;
        }
        return count;
    }

    @Benchmark
    public int test_contains_buffered(BenchmarkState s) {
        int count = 0;
        for(BufferedTriple st : small_bhm) {
          if(s.bhm.contains(st)) count++;
        }
        return count;
    }
    public static void main(String[] args) throws RunnerException {
        Options opt = new OptionsBuilder()
        .include(HashFast.class.getSimpleName()).warmupIterations(5)
        .measurementIterations(10).forks(1).build();
        new Runner(opt).run();
    }

}


final class Triple {
  private int x;
  private int y;
  private int z;

  public Triple(int mx, int my, int mz) {
    x = mx;
    y = my;
    z = my;
  }

  public int hashCode() {
    return x + 31 * y + 31 * 31 * z;
  }

  public boolean equals(Object o) {
    if(o instanceof Triple) {
      Triple t = (Triple) o;
      return (t.x == x) && (t.y == y) && (t.z == z);
    }
    return false;
  }
}

final class BufferedTriple {
  private int x;
  private int y;
  private int z;
  private int hash;

  public BufferedTriple(int mx, int my, int mz) {
    x = mx;
    y = my;
    z = my;
    hash = x + 31 * y + 31 * 31 * z;
  }

  public int hashCode() {
    return hash;
  }

  public boolean equals(Object o) {
    if(o instanceof BufferedTriple) {
      BufferedTriple t = (BufferedTriple) o;
      return (t.x == x) && (t.y == y) && (t.z == z);
    }
    return false;
  }
}
