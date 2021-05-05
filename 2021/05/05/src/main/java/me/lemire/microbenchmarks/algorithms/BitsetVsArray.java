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
public class BitsetVsArray {
	@State(Scope.Benchmark)
	public static class BenchmarkState {
		@Param({"65536"})
		public int listSize;


		@Setup(Level.Trial)
		public void setUp() {
		}

		@Setup(Level.Invocation)
		public void perInvocation(){
		}
	}

	@Benchmark
	public void constructStringArray(Blackhole blackhole, BenchmarkState state) {
		String[] array = new String[state.listSize];
		for(int k = 0; k < state.listSize; k++) {
			array[k] = ((k & 1) == 0) ? "Found" : "NotFound";
		}
		blackhole.consume(array);
	}

	@Benchmark
	public void constructBooleanArray(Blackhole blackhole, BenchmarkState state) {
		boolean[] array = new boolean[state.listSize];
		for(int k = 0; k < state.listSize; k++) {
			array[k] = ((k & 1) == 0) ? true: false;
		}
		blackhole.consume(array);
	}

	@Benchmark
	public void constructByteArray(Blackhole blackhole, BenchmarkState state) {
		byte[] array = new byte[state.listSize];
		for(int k = 0; k < state.listSize; k++) {
			array[k] = ((k & 1) == 0) ? (byte)1 : (byte)0;
		}
		blackhole.consume(array);
	}

	@Benchmark
	public void constructBitset(Blackhole blackhole, BenchmarkState state) {
		BitSet bitset = new BitSet(state.listSize);
		for(int k = 0; k < state.listSize; k++) {
			if((k & 1) == 0) { bitset.set(k); }
		}
		blackhole.consume(bitset);
	}
  

  public static void main(String[] args) throws RunnerException {
        Options opt = new OptionsBuilder()
        .include(BitsetVsArray.class.getSimpleName()).warmupIterations(5)
        .measurementIterations(10).forks(1).build();
        new Runner(opt).run();
  }

}
