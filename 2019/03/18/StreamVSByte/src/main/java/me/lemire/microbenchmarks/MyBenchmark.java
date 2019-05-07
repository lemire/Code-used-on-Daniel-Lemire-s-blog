
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
        int N = 4000000;
        byte[] array = new byte[N];

        public BenchmarkState() {
            for(int k = 0; k  < array.length; k++)
              array[k] = (byte) k;
        }

    }

    @Benchmark
    public int[] stream(BenchmarkState s) throws IOException {
        int[] recipient = new int[s.N / 4];
        DataInput di = new DataInputStream(new ByteArrayInputStream(s.array));
        for(int k = 0; k < recipient.length; k++)
          recipient[k] = di.readInt();
        return recipient;
    }

    @Benchmark
    public int[] buffer(BenchmarkState s) throws IOException {
        int[] recipient = new int[s.N / 4];
        ByteBuffer bb = ByteBuffer.wrap(s.array);
        bb.asIntBuffer().get(recipient);
        return recipient;
    }

}
