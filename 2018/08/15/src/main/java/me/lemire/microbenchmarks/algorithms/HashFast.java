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
  static long a1 = 0x65d200ce55b19ad8L;
  static long b1 = 0x4f2162926e40c299L;
  static long c1 = 0x162dd799029970f8L;
  static long a2 = 0x68b665e6872bd1f4L;
  static long b2 = 0xb6cfcf9d79b51db2L;
  static long c2 = 0x7a2b92ae912898c2L;

  static int hash32_1(long x) {
    int low = (int)x;
    int high = (int)(x >>> 32);
    return (int)((a1 * low + b1 * high + c1) >>> 32);
  }
  static int hash32_2(long x) {
    int low = (int)x;
    int high = (int)(x >>> 32);
    return (int)((a2 * low + b2 * high + c2) >>> 32);
  }

  static long hash64(long x) {
      int low = (int)x;
      int high = (int)(x >>> 32);
      return ((a1 * low + b1 * high + c1) >>> 32)
        | ((a2 * low + b2 * high + c2) & 0xFFFFFFFF00000000L);
  }

  static long murmur64(long h) {
    h ^= h >>> 33;
    h *= 0xff51afd7ed558ccdL;
    h ^= h >>> 33;
    h *= 0xc4ceb9fe1a85ec53L;
    h ^= h >>> 33;
    return h;
  }

  @Benchmark
  public long murmur()   {
    long answer = 0;
    for(long x = 0; x < 100000; x++) {
      answer += murmur64(x);
    }
    return answer;
  }

  @Benchmark
  public int murmur_32()   {
    int answer1 = 0;
    int answer2 = 0;
    for(long x = 0; x < 100000; x++) {
      long h = murmur64(x);
      answer1 += (int) h;
      answer2 += (int) (h >>> 32);
    }
    return answer1 + answer2;
  }

  @Benchmark
  public int fast2_32()   {
    int answer1 = 0;
    int answer2 = 0;

    for(long x = 0; x < 100000; x++) {
      answer1 += hash32_1(x);
      answer2 += hash32_2(x);

    }
    return answer1 + answer2;
  }
  @Benchmark
  public long fast64()   {
    long answer = 0;
    for(long x = 0; x < 100000; x++) {
      answer += hash64(x);
    }
    return answer;
  }

  public static void main(String[] args) throws RunnerException {
        Options opt = new OptionsBuilder()
        .include(HashFast.class.getSimpleName()).warmupIterations(5)
        .measurementIterations(10).forks(1).build();
        new Runner(opt).run();
  }

}
