package me.lemire.microbenchmarks.algorithms;

import java.util.*;
import java.util.concurrent.TimeUnit;
import java.util.stream.Collectors;

import org.openjdk.jmh.annotations.*;
import org.openjdk.jmh.runner.Runner;
import org.openjdk.jmh.runner.RunnerException;
import org.openjdk.jmh.runner.options.Options;
import org.openjdk.jmh.runner.options.OptionsBuilder;
import org.openjdk.jmh.infra.Blackhole;
import java.util.concurrent.ThreadLocalRandom;

@State(Scope.Benchmark)
@BenchmarkMode(Mode.AverageTime)
@OutputTimeUnit(TimeUnit.MICROSECONDS)
public class RadixSort {
  
  public static void radixSort(int[] data) {
    int[] copy = new int[data.length];
    int[] level0 = new int[257];
    int[] level1 = new int[257];
    int[] level2 = new int[257];
    int[] level3 = new int[257];
    for (int value : data) {
	  value -= Integer.MIN_VALUE;
	  level0[(value & 0xFF) + 1] ++;
	  level1[((value >>> 8) & 0xFF) + 1] ++;
	  level2[((value >>> 16) & 0xFF) + 1] ++;
	  level3[((value >>> 24) & 0xFF) + 1] ++;
    }
    for (int i = 1; i < level0.length; ++i) {
	  level0[i] += level0[i - 1];
	  level1[i] += level1[i - 1];
	  level2[i] += level2[i - 1];
	  level3[i] += level3[i - 1];
	}
    for (int value : data) {
      copy[level0[(value - Integer.MIN_VALUE) & 0xFF]++] = value;
    }
    for (int value : copy) {
	  data[level1[((value - Integer.MIN_VALUE)>>>8) & 0xFF]++] = value;
	}
    for (int value : data) {
		copy[level2[((value - Integer.MIN_VALUE)>>>16) & 0xFF]++] = value;
	}
    for (int value : copy) {
	  data[level3[((value - Integer.MIN_VALUE)>>>24) & 0xFF]++] = value;
	}
  }

	@State(Scope.Benchmark)
	public static class BenchmarkState {
		@Param({"1000000"})
		public int listSize;

		private int[] origArr;
		public int[] testArr;

		@Setup(Level.Trial)
		public void setUp() {
			origArr = ThreadLocalRandom.current()
					.ints()
					.limit(listSize)
					.toArray();
			testArr = new int[origArr.length];
		}

		@Setup(Level.Invocation)
		public void perInvocation(){
			System.arraycopy(origArr, 0, testArr, 0, origArr.length);
		}
	}

	@Benchmark
	public void BM_radixSort(Blackhole blackhole, BenchmarkState state) {
		radixSort(state.testArr);
		blackhole.consume(state.testArr);
	}
	@Benchmark
	public void BM_sort(Blackhole blackhole, BenchmarkState state) {
		Arrays.sort(state.testArr);
		blackhole.consume(state.testArr);
	}

  

  public static void main(String[] args) throws RunnerException {
        Options opt = new OptionsBuilder()
        .include(RadixSort.class.getSimpleName()).warmupIterations(5)
        .measurementIterations(10).forks(1).build();
        new Runner(opt).run();
  }

}
