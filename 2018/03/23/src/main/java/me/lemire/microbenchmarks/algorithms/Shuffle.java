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

    private static void shuffle_java_blocked(int arr[]) {
        ThreadLocalRandom tlc = ThreadLocalRandom.current();
        int size = arr.length;
        final int block = 8;
        final int[] buffer = new int[block];
        int i = size;
        for (; i > block +  1; i-=block) {
            for(int k = 0; k < block; k++)
              buffer[k] = tlc.nextInt(i - k);
            for(int k = 0; k < block; k++)
              swap(arr, i - 1 -k, buffer[k]);
        }
        for (; i > 1; i--) {
            swap(arr, i - 1, tlc.nextInt(i));
        }
    }
    private static void shuffle_java_blocked_inlined(int arr[]) {
        ThreadLocalRandom tlc = ThreadLocalRandom.current();
        int size = arr.length;
        int i = size;
        for (; i > 8 +  1; i -= 8) {
            final int buffer0 = tlc.nextInt(i    );
            final int buffer1 = tlc.nextInt(i - 1);
            final int buffer2 = tlc.nextInt(i - 2);
            final int buffer3 = tlc.nextInt(i - 3);
            final int buffer4 = tlc.nextInt(i - 4);
            final int buffer5 = tlc.nextInt(i - 5);
            final int buffer6 = tlc.nextInt(i - 6);
            final int buffer7 = tlc.nextInt(i - 7);
            swap(arr, i - 1    , buffer0);
            swap(arr, i - 1 - 1, buffer1);
            swap(arr, i - 1 - 2, buffer2);
            swap(arr, i - 1 - 3, buffer3);
            swap(arr, i - 1 - 4, buffer4);
            swap(arr, i - 1 - 5, buffer5);
            swap(arr, i - 1 - 6, buffer6);
            swap(arr, i - 1 - 7, buffer7);

        }
        for (; i > 1; i--) {
            swap(arr, i - 1, tlc.nextInt(i));
        }
    }
    private static void shuffle_java_rev(int arr[]) {
        ThreadLocalRandom tlc = ThreadLocalRandom.current();
        int size = arr.length;
        for (int i = size; i > 1; i--)
            swap(arr, size - i, tlc.nextInt(i));
    }

    private static void shuffle_precomp(int arr[], int indexes[]) {
        int size = arr.length;
        for (int i = size; i > 1; i--)
            swap(arr, i - 1, indexes[i]);
    }

    private static void shuffle_precomp_rev(int arr[], int indexes[]) {
        int size = arr.length;
        for (int i = size; i > 1; i--)
             swap(arr, size - i, indexes[i]);
    }

    @State(Scope.Benchmark)
    public static class BenchmarkState {
        int N = 100000000;
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
    public void test_shuffle_java_blocked_inlined(BenchmarkState s) {
        shuffle_java_blocked_inlined(s.array);
    }

    @Benchmark
    public void test_shuffle_java_blocked(BenchmarkState s) {
        shuffle_java_blocked(s.array);
    }

    @Benchmark
    public void test_shuffle_java(BenchmarkState s) {
        shuffle_java(s.array);
    }

    @Benchmark
    public void test_shuffle_precomp(BenchmarkState s) {
        shuffle_precomp(s.array, s.indexes);
    }

    @Benchmark
    public void test_shuffle_java_rev(BenchmarkState s) {
        shuffle_java_rev(s.array);
    }

    @Benchmark
    public void test_shuffle_precomp_rev(BenchmarkState s) {
        shuffle_precomp_rev(s.array, s.indexes);
    }

    public static void main(String[] args) throws RunnerException {
        Options opt = new OptionsBuilder()
        .include(Shuffle.class.getSimpleName()).warmupIterations(5)
        .measurementIterations(5).forks(1).build();
        new Runner(opt).run();
    }

}
