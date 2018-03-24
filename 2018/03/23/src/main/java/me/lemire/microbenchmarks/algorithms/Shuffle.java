package me.lemire.microbenchmarks.algorithms;

import java.util.Arrays;
import java.util.Random;
import java.util.concurrent.ThreadLocalRandom;
import java.util.concurrent.TimeUnit;

import org.openjdk.jmh.annotations.*;
import org.openjdk.jmh.runner.Runner;
import org.openjdk.jmh.runner.RunnerException;
import org.openjdk.jmh.runner.options.Options;
import org.openjdk.jmh.runner.options.OptionsBuilder;

@State(Scope.Benchmark)
@BenchmarkMode(Mode.AverageTime)
@OutputTimeUnit(TimeUnit.SECONDS)
public class Shuffle {

    private static void swap(int[] arr, int i, int j) {
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }

    private static void shuffle_java(int arr[]) {
        ThreadLocalRandom tlc = ThreadLocalRandom.current();
        int size = arr.length;
        for (int i = size; i > 1; i--)
            swap(arr, i - 1, tlc.nextInt(i));
    }
    private static void shuffle_precomp(int arr[], int indexes[]) {
        int size = arr.length;
        for (int i = size; i > 1; i--)
            swap(arr, i - 1, indexes[i]);
    }

    @State(Scope.Benchmark)
    public static class BenchmarkState {
        int N = 10000000;
        int[] array = new int[N];
        int[] indexes = new int[N+1];
        public BenchmarkState() {
            ThreadLocalRandom tlc = ThreadLocalRandom.current();
            for (int k = 0; k < N; k++)
              array[k] = k;
            for (int k = N; k > 0; k--)
              indexes[k] = tlc.nextInt(k);
        }
    }



    @Benchmark
    public void test_shuffle_java(BenchmarkState s) {
        shuffle_java(s.array);
    }

    @Benchmark
    public void test_shuffle_precomp(BenchmarkState s) {
        shuffle_precomp(s.array, s.indexes);
    }

    public static void main(String[] args) throws RunnerException {
        Options opt = new OptionsBuilder()
        .include(Shuffle.class.getSimpleName()).warmupIterations(5)
        .measurementIterations(5).forks(1).build();
        new Runner(opt).run();
    }

}
