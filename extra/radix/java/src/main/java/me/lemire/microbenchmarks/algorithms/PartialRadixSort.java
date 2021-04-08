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
public class PartialRadixSort {
  
	public static void originalPartialRadixSort(int[] data) {
		final int radix = 8;

		int shift = 16;
		int mask = 0xFF0000;
		int[] copy = new int[data.length];
		int[] histogram = new int[(1 << radix) + 1];
		while (shift < 32) {
			for (int i = 0; i < data.length; ++i) {
				++histogram[((data[i] & mask) >>> shift) + 1];
			}
			for (int i = 0; i < 1 << radix; ++i) {
				histogram[i + 1] += histogram[i];
			}
			for (int i = 0; i < data.length; ++i) {
				copy[histogram[(data[i] & mask) >>> shift]++] = data[i];
			}
			System.arraycopy(copy, 0, data, 0, data.length);
			shift += radix;
			mask <<= radix;
			Arrays.fill(histogram, 0);
		}
	}

	public static void newPartialRadixSort(int[] data) {
		final int radix = 8;
		int shift = 16;
		int mask = 0xFF0000;
		int[] copy = new int[data.length];
		int[] histogram = new int[(1 << radix) + 1];
		// We want to avoid copying the data, see
		// https://github.com/RoaringBitmap/RoaringBitmap/issues/470
		int[] primary = data;
		int[] secondary = copy;
		while (shift < 32) {
			for (int i = 0; i < data.length; ++i) {
				++histogram[((primary[i] & mask) >>> shift) + 1];
			}
			for (int i = 0; i < 1 << radix; ++i) {
				histogram[i + 1] += histogram[i];
			}
			for (int i = 0; i < primary.length; ++i) {
				secondary[histogram[(primary[i] & mask) >>> shift]++] = primary[i];
			}
			// swap
			int[] tmp = primary;
			primary = secondary;
			secondary = tmp;
			//
			shift += radix;
			mask <<= radix;
			Arrays.fill(histogram, 0);
		}
	}
  public static void newnewUnrollPartialRadixSort(int[] primary) {
    final int radix = 8;
    int[] secondary = new int[primary.length];
    int[] histogram = new int[(1 << radix) + 1];
    {
      final int shift = 16;
      for (int i = 0; i < primary.length; ++i) {
        ++histogram[((primary[i] >>> shift) & 0xFF) + 1];
      }
      for (int i = 0; i < 1 << radix; ++i) {
        histogram[i + 1] += histogram[i];
      }
      for (int i = 0; i < primary.length; ++i) {
        secondary[histogram[(primary[i] >>> shift) & 0xFF]++] = primary[i];
      }
    }
    Arrays.fill(histogram, 0);
    {
      final int shift = 24;
      for (int i = 0; i < secondary.length; ++i) {
        ++histogram[((secondary[i] >>> shift) & 0xFF) + 1];
      }
      for (int i = 0; i < 1 << radix; ++i) {
        histogram[i + 1] += histogram[i];
      }
      for (int i = 0; i < secondary.length; ++i) {
        primary[histogram[(secondary[i] >>> shift) & 0xFF]++] = secondary[i];
      }
    }
  }

	public static void newnewPartialRadixSort(int[] data) {
		final int radix = 8;
		int shift = 16;
		int[] copy = new int[data.length];
		int[] histogram = new int[(1 << radix) + 1];
		// We want to avoid copying the data, see
		// https://github.com/RoaringBitmap/RoaringBitmap/issues/470
		int[] primary = data;
		int[] secondary = copy;
		while (shift < 32) {
			for (int i = 0; i < data.length; ++i) {
				++histogram[((primary[i] >>> shift) & 0xFF) + 1];
			}
			for (int i = 0; i < 1 << radix; ++i) {
				histogram[i + 1] += histogram[i];
			}
			for (int i = 0; i < primary.length; ++i) {
				secondary[histogram[(primary[i] >>> shift) & 0xFF]++] = primary[i];
			}
			// swap
			int[] tmp = primary;
			primary = secondary;
			secondary = tmp;
			//
			shift += radix;
			if(shift < 32) { Arrays.fill(histogram, 0); }
		}	
  }

	public static void unrollPartialRadixSort(int[] primary) {
		final int radix = 8;

		final int[] secondary = new int[primary.length];
		final int[] histogram = new int[(1 << radix) + 1];
		{
			final int shift = 16;
			final int mask = 0x00FF0000;
			for (int i = 0; i < primary.length; ++i) {
				++histogram[((primary[i] & mask) >>> shift) + 1];
			}
			for (int i = 0; i < 1 << radix; ++i) {
				histogram[i + 1] += histogram[i];
			}
			for (int i = 0; i < primary.length; ++i) {
				secondary[histogram[(primary[i] & mask) >>> shift]++] = primary[i];
			}
		}
		Arrays.fill(histogram, 0);
		{
			final int shift = 24;
			final int mask = 0xFF000000;
			for (int i = 0; i < secondary.length; ++i) {
				++histogram[((secondary[i] & mask) >>> shift) + 1];
			}
			for (int i = 0; i < 1 << radix; ++i) {
				histogram[i + 1] += histogram[i];
			}
			for (int i = 0; i < secondary.length; ++i) {
				primary[histogram[(secondary[i] & mask) >>> shift]++] = secondary[i];
			}
		}
	}

	public static void likeNewPartialRadixSort(int[] data) {
		final int radix = 8;
		int shift = 16;
		int mask = 0xFF0000;
		int[] copy = new int[data.length];
		int[] histogram = new int[(1 << radix) + 1];
		// We want to avoid copying the data, see
		// https://github.com/RoaringBitmap/RoaringBitmap/issues/470
		int[] primary = data;
		int[] secondary = copy;
		for (int i = 0; i < data.length; ++i) {
			++histogram[((primary[i] & mask) >>> shift) + 1];
		}
		for (int i = 0; i < 1 << radix; ++i) {
			histogram[i + 1] += histogram[i];
		}
		for (int i = 0; i < primary.length; ++i) {
			secondary[histogram[(primary[i] & mask) >>> shift]++] = primary[i];
		}
		// swap
		int[] tmp = primary;
		primary = secondary;
		secondary = tmp;
		//
		shift += radix;
		mask <<= radix;
		Arrays.fill(histogram, 0);
		for (int i = 0; i < data.length; ++i) {
			++histogram[((primary[i] & mask) >>> shift) + 1];
		}
		for (int i = 0; i < 1 << radix; ++i) {
			histogram[i + 1] += histogram[i];
		}
		for (int i = 0; i < primary.length; ++i) {
			secondary[histogram[(primary[i] & mask) >>> shift]++] = primary[i];
		}
		// swap
		tmp = primary;
		primary = secondary;
		secondary = tmp;
		//
		shift += radix;
		mask <<= radix;
		Arrays.fill(histogram, 0);
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
	public void BM_original(Blackhole blackhole, BenchmarkState state) {
		originalPartialRadixSort(state.testArr);
		blackhole.consume(state.testArr);
	}
	@Benchmark
	public void BM_new(Blackhole blackhole, BenchmarkState state) {
		newPartialRadixSort(state.testArr);
		blackhole.consume(state.testArr);
	}
	@Benchmark
	public void BM_newnew(Blackhole blackhole, BenchmarkState state) {
		newnewPartialRadixSort(state.testArr);
		blackhole.consume(state.testArr);
	}
	@Benchmark
	public void BM_newnew2(Blackhole blackhole, BenchmarkState state) {
		newnewPartialRadixSort(state.testArr);
		blackhole.consume(state.testArr);
	}

	@Benchmark
	public void BM_unroll(Blackhole blackhole, BenchmarkState state) {
		unrollPartialRadixSort(state.testArr);
		blackhole.consume(state.testArr);
	}

  @Benchmark
	public void BM_unroll2(Blackhole blackhole, BenchmarkState state) {
		unrollPartialRadixSort(state.testArr);
		blackhole.consume(state.testArr);
	}

	@Benchmark
	public void BM_likeNew(Blackhole blackhole, BenchmarkState state) {
		likeNewPartialRadixSort(state.testArr);
		blackhole.consume(state.testArr);
	}

	@Benchmark
	public void BM_fullSort(Blackhole blackhole, BenchmarkState state) {
		Arrays.sort(state.testArr);
		blackhole.consume(state.testArr);
	}

	@Benchmark
	public void BM_newnewUnroll(Blackhole blackhole, BenchmarkState state) {
		newnewUnrollPartialRadixSort(state.testArr);
		blackhole.consume(state.testArr);
	}

  @Benchmark
	public void BM_newnewUnroll2(Blackhole blackhole, BenchmarkState state) {
		newnewUnrollPartialRadixSort(state.testArr);
		blackhole.consume(state.testArr);
	}

  public static void main(String[] args) throws RunnerException {
        Options opt = new OptionsBuilder()
        .include(PartialRadixSort.class.getSimpleName()).warmupIterations(5)
        .measurementIterations(10).forks(1).build();
        new Runner(opt).run();
  }

}
