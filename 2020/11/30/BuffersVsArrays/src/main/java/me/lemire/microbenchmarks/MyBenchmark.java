
package me.lemire.microbenchmarks;

import java.io.*;
import java.nio.*;
import org.openjdk.jmh.annotations.*;
import java.util.concurrent.TimeUnit;

@State(Scope.Thread)
@BenchmarkMode(Mode.AverageTime)
@OutputTimeUnit(TimeUnit.MILLISECONDS)

@Warmup(iterations = 2, time = 200, timeUnit = TimeUnit.MILLISECONDS)
@Measurement(iterations = 3,  time = 200, timeUnit = TimeUnit.MILLISECONDS)
@Threads(value = 1)
public class MyBenchmark {


    @State(Scope.Benchmark)
    public static class BenchmarkState {
        int N = 50000;
        int[] array = new int[N];
        IntBuffer buffer = IntBuffer.allocate(N);


        public BenchmarkState() {
            for(int k = 0; k  < array.length; k++) { 
                array[k] = k;
                buffer.put(k, k);
            }
              
        }

    }

    @Benchmark
    public void array(BenchmarkState s) {
        for(int k = 0; k  < s.array.length; k++) { 
            s.array[k] += 1;
        }
    }

    @Benchmark
    public void buffer(BenchmarkState s) throws IOException {
        for(int k = 0; k  < s.buffer.limit(); k++) { 
            s.buffer.put(k, s.buffer.get(k) + 1);
        }
    }

}
