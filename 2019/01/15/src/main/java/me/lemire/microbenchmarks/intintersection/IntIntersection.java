package me.lemire.microbenchmarks.intintersection;

import org.openjdk.jmh.annotations.*;
import org.openjdk.jmh.runner.*;
import org.openjdk.jmh.runner.options.*;

import java.util.*;
import java.util.concurrent.TimeUnit;

@State(Scope.Thread)
@BenchmarkMode(Mode.AverageTime)
@OutputTimeUnit(TimeUnit.MICROSECONDS)
public class IntIntersection {

  static int N = 100000000;
  static int Ntest = 32;

  @State(Scope.Benchmark)
  public static class BenchmarkState {

    int[] array;
    int[] answer;
    int[] queries;
    private static int MMAX = 1000000000;
    Random rand = new Random();
    public int nextQuery() { return rand.nextInt(); }

    public BenchmarkState() {
      System.out.print("Intersecting a small sorted array ("+Ntest+")");
      System.out.print(" with a large sorted array ("+N+")");
      System.out.println(" large arrays use "+N * 4 / (1024 * 1024)+ " MB.");
      array = new int[N];
      queries = new int[Ntest];
      answer = new int[Ntest];

      for (int k = 0; k < N; ++k)
        array[k] = rand.nextInt(MMAX);
      Arrays.sort(array);
      for (int k = 0; k < queries.length; ++k)
        queries[k] = array[rand.nextInt(array.length)];
      Arrays.sort(queries); 
    }
  }

  private static int BinarySearch(final int[] array, final int begin,
                                  final int end, final int k) {
    int ikey = k;
    if ((end > 0) && (array[end - 1] < ikey)) {
      return -end - 1;
    }
    int low = begin;
    int high = end - 1;
    // 32 in the next line matches the size of a cache line
    while (low <= high) {
      final int middleIndex = (low + high) >>> 1;
      final int middleValue = array[middleIndex];
      if (middleValue < ikey) {
        low = middleIndex + 1;
      } else if (middleValue > ikey) {
        high = middleIndex - 1;
      } else {
        return middleIndex;
      }
    }
    return -(low + 1);
  }

  int shotgun_search4(final int[] smallSet, final int[] largeSet, int[] answer) {
    int pos = 0, idx_l = 0, idx_s = 0;

    if (0 == smallSet.length) {
      return 0;
    }
    int index1 = 0, index2 = 0, index3 = 0, index4 = 0;
    while ((idx_s + 4 <= smallSet.length) && (idx_l < largeSet.length)) {
      int target1 = smallSet[idx_s];
      int target2 = smallSet[idx_s + 1];
      int target3 = smallSet[idx_s + 2];
      int target4 = smallSet[idx_s + 3];
      {
        int n = largeSet.length - idx_l;
        if (n == 0)
          return pos;
        int base1 = idx_l;
        int base2 = idx_l;
        int base3 = idx_l;
        int base4 = idx_l;
        while (n > 1) {
          int half = n >>> 1;
          base1 = (largeSet[base1 + half] < target1) ? base1 + half : base1;
          base2 = (largeSet[base2 + half] < target2) ? base2 + half : base2;
          base3 = (largeSet[base3 + half] < target3) ? base3 + half : base3;
          base4 = (largeSet[base4 + half] < target4) ? base4 + half : base4;
          n -= half;
        }
        index1 = (largeSet[base1] < target1) ? base1 + 1 : base1;
        index2 = (largeSet[base2] < target2) ? base2 + 1 : base2;
        index3 = (largeSet[base3] < target3) ? base3 + 1 : base3;
        index4 = (largeSet[base4] < target4) ? base4 + 1 : base4;
      }

      if ((index1 < largeSet.length) && (largeSet[index1] == target1)) {
        answer[pos++] = target1;
      }
      if ((index2 < largeSet.length) && (largeSet[index2] == target2)) {
        answer[pos++] = target2;
      }
      if ((index3 < largeSet.length) && (largeSet[index3] == target3)) {
        answer[pos++] = target3;
      }
      if ((index4 < largeSet.length) && (largeSet[index4] == target4)) {
        answer[pos++] = target4;
      }
      idx_s += 4;
      idx_l = index4;
    }
    if ((idx_s < smallSet.length) && (idx_l < largeSet.length)) {
      int val_s = smallSet[idx_s];
      int index = BinarySearch(largeSet, idx_l, largeSet.length, val_s);
      if (index >= 0)
        answer[pos++] = val_s;
    }
    return pos;
  }


  int shotgun_search1(final int[] smallSet, final int[] largeSet, int[] answer) {
    int pos = 0, idx_l = 0, idx_s = 0;

    if (0 == smallSet.length) {
      return 0;
    }
    int index1 = 0;
    while ((idx_s  < smallSet.length) && (idx_l < largeSet.length)) {
      int target1 = smallSet[idx_s];
      {
        int n = largeSet.length - idx_l;
        if (n == 0)
          return pos;
        int base1 = idx_l;
        while (n > 1) {
          int half = n >>> 1;
          base1 = (largeSet[base1 + half] < target1) ? base1 + half : base1;
          n -= half;
        }
        index1 = (largeSet[base1] < target1) ? base1 + 1 : base1;
      }
      if ((index1 < largeSet.length) && (largeSet[index1] == target1)) {
        answer[pos++] = target1;
      }
      idx_s += 1;
      idx_l = index1;
    }
    return pos;
  }






  @Benchmark
  public int shotgun1(BenchmarkState s) {
    int c = shotgun_search1(s.queries, s.array, s.answer);
    return c;
  }

  @Benchmark
  public int shotgun4(BenchmarkState s) {
    int c = shotgun_search4(s.queries, s.array, s.answer);
    return c;
  }


  public static void main(String[] args) throws RunnerException {
    Options opt = new OptionsBuilder()
                      .include(IntIntersection.class.getSimpleName())
                      .warmupIterations(4)
                      .measurementIterations(6)
                      .forks(1)
                      .build();
    new Runner(opt).run();
  }
}
