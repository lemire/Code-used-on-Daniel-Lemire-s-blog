
package me.lemire.microbenchmarks;

import java.io.*;
import java.nio.*;
import org.openjdk.jmh.annotations.*;
import java.util.concurrent.TimeUnit;

@State(Scope.Thread)
@BenchmarkMode(Mode.AverageTime)
@OutputTimeUnit(TimeUnit.MICROSECONDS)

@Warmup(iterations = 5, time = 1000000, timeUnit = TimeUnit.MICROSECONDS)
@Measurement(iterations = 3,  time = 1000000, timeUnit = TimeUnit.MICROSECONDS)
@Threads(value = 1)
public class MyBenchmark {


    @State(Scope.Benchmark)
    public static class BenchmarkState {
        int N = 50000;
        int[] array = new int[N];
        IntBuffer buffer = IntBuffer.allocate(N);
        IntBuffer buffer_crazy = ByteBuffer.allocate(N * Integer.BYTES).order(ByteOrder.nativeOrder()).asIntBuffer();


        public BenchmarkState() {
            for(int k = 0; k  < array.length; k++) { 
                array[k] = k;
                buffer.put(k, k);
                buffer_crazy.put(k, k);
            }
            System.out.println("is backed by array: "+buffer.hasArray());
            System.out.println("is backed by array: "+buffer_crazy.hasArray());     
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

    @Benchmark
    public void buffer_crazy(BenchmarkState s) throws IOException {
        for(int k = 0; k  < s.buffer_crazy.limit(); k++) { 
            s.buffer_crazy.put(k, s.buffer_crazy.get(k) + 1);
        }
    }
}
