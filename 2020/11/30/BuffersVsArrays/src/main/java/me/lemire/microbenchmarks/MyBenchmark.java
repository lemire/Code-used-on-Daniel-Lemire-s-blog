
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
        IntBuffer buffer_direct = ByteBuffer.allocateDirect(N * Integer.BYTES).order(ByteOrder.nativeOrder()).asIntBuffer();;

        public BenchmarkState() {
            for(int k = 0; k  < array.length; k++) { 
                array[k] = k;
                buffer.put(k, k);
                buffer_crazy.put(k, k);
                buffer_direct.put(k, k);
            }
            System.out.println("buffer.hasArray(): "+buffer.hasArray());
            System.out.println("buffer_crazy.hasArray(): "+buffer_crazy.hasArray());     
            System.out.println("buffer_direct.hasArray(): "+buffer_direct.hasArray());     
            System.out.println("buffer.order: "+buffer.order());
            System.out.println("buffer_crazy.order: "+buffer_crazy.order());   
            System.out.println("buffer_direct.order: "+buffer_direct.order());   
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

    @Benchmark
    public void buffer_direct(BenchmarkState s) throws IOException {
        for(int k = 0; k  < s.buffer_direct.limit(); k++) { 
            s.buffer_direct.put(k, s.buffer_direct.get(k) + 1);
        }
    }
}
