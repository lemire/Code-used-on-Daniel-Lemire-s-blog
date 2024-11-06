package me.lemire;

import org.openjdk.jmh.annotations.Benchmark;

import org.openjdk.jmh.annotations.*;
import org.openjdk.jmh.infra.Blackhole;

import java.lang.invoke.MethodHandles;
import java.lang.invoke.VarHandle;
import java.nio.ByteOrder;
import java.util.Arrays;
import java.util.random.RandomGenerator;
import java.util.random.RandomGeneratorFactory;

@Measurement(iterations = 10, time = 1)
@Warmup(iterations = 5, time = 1)
// the default size of char[] is enough to trigger OSR compilation which is not as effective as C2
// as most of the time, in the real world, is likely the method will be compiled with way less iterations of the loop
// making the compiler to perform better decisions i.e. silly_tableX reference shouldn't be brought into a register
// in the hot path but hoisted before the loop begins, given that's a constant value, from the JVM pov
@Fork(value = 2, jvmArgsPrepend = {"-XX:-UseOnStackReplacement"})
public class MyBenchmark {
    private static final byte[] silly_table1;

    static {
        silly_table1 = new byte[256];
        silly_table1['\\'] = '\\';
    }
    private static final byte[] silly_table2;

    static {
        silly_table2 = new byte[256];
        silly_table2['\\'] = '\\';
        silly_table2['\n'] = 'n';
    }

    private static final byte[] silly_table3;

    static {
        silly_table3 = new byte[256];
        silly_table3['\\'] = '\\';
        silly_table3['\n'] = 'n';
        silly_table3['\t'] = 't';
    }

    public static int replaceBackslash1(char[] original, char[] newArray) {
        int index = 0;
        for (char c : original) {
            if (c == '\\') {
                newArray[index++] = '\\';
                newArray[index++] = '\\';
            } else {
                newArray[index++] = c;
            }
        }
        return index;
    }

    public static int replaceBackslashTable1(char[] original, char[] newArray) {
        int newArrayLength = 0;
        for (char c : original) {
            // copy regardless into output; this allow the loop to be unrolled ;)
            newArray[newArrayLength] = c;
            byte b = silly_table1[c % 256];
            if (c < 256 && b != 0) {
                // we need to copy from the last known index to the current one, excluded
                newArray[newArrayLength] = '\\';
                newArray[newArrayLength + 1] = (char) b;
                newArrayLength += 2;
            } else {
                newArrayLength++;
            }
        }
        return newArrayLength;
    }


    public static int replaceBackslash2(char[] original, char[] newArray) {
        int index = 0;
        for (char c : original) {
            if (c == '\\') {
                newArray[index++] = '\\';
                newArray[index++] = '\\';
            } else if (c == '\n') {
                newArray[index++] = '\\';
                newArray[index++] = 'n';
            } else {
                newArray[index++] = c;
            }
        }
        return index;
    }

    public static int replaceBackslashTable2(char[] original, char[] newArray) {
        int newArrayLength = 0;
        for (char c : original) {
            // copy regardless into output; this allow the loop to be unrolled ;)
            newArray[newArrayLength] = c;
            byte b = silly_table2[c % 256];
            if (c < 256 && b != 0) {
                // we need to copy from the last known index to the current one, excluded
                newArray[newArrayLength] = '\\';
                newArray[newArrayLength + 1] = (char) b;
                newArrayLength += 2;
            } else {
                newArrayLength++;
            }
        }
        return newArrayLength;
    }

    public static int replaceBackslash3(char[] original, char[] newArray) {
        int index = 0;
        for (char c : original) {
            if (c == '\\') {
                newArray[index++] = '\\';
                newArray[index++] = '\\';
            } else if (c == '\n') {
                newArray[index++] = '\\';
                newArray[index++] = 'n';
            } else if (c == '\t') {
                newArray[index++] = '\\';
                newArray[index++] = 't';
            } else {
                newArray[index++] = c;
            }
        }
        return index;
    }

    public static int replaceBackslashTable3(char[] original, char[] newArray) {
        int newArrayLength = 0;
        for (char c : original) {
            // copy regardless into output; this allow the loop to be unrolled ;)
            newArray[newArrayLength] = c;
            byte b = silly_table3[c % 256];
            if (c < 256 && b != 0) {
                // we need to copy from the last known index to the current one, excluded
                newArray[newArrayLength] = '\\';
                newArray[newArrayLength + 1] = (char) b;
                newArrayLength += 2;
            } else {
                newArrayLength++;
            }
        }
        return newArrayLength;
    }

    @State(Scope.Benchmark)
    public static class BenchmarkState {

        // better be safe and keep it the same to have reproducible results
        private static final int SEED = 42;

        @Param({"65536"})
        public int size;
        public char[] inputstring;
        public char[] outputstring;


        @Setup(Level.Trial)
        public void setUp() {
            RandomGenerator random = RandomGeneratorFactory.getDefault().create(SEED);
            inputstring = new char[size];
            // we want to size the output array with the worst case scenario
            int count = size;
            for(int k = 0; k < size; k++) {
                int value = random.nextInt(0, 256);
                inputstring[k] = (char)value;
                if(silly_table3[inputstring[k]] > 0) {
                    count++;
                }
            }
            outputstring = new char[count];
        }
    }

    @State(Scope.Benchmark)
    public static class RawBenchmarkState {

        // better be safe and keep it the same to have reproducible results
        private static final int SEED = 42;

        @Param({"65536"})
        public int size;
        public byte[] inputstring;
        public byte[] outputstring;


        @Setup(Level.Trial)
        public void setUp() {
            RandomGenerator random = RandomGeneratorFactory.getDefault().create(SEED);
            inputstring = new byte[size];
            // we want to size the output array with the worst case scenario
            int count = size;
            for(int k = 0; k < size; k++) {
                int value = random.nextInt(0, 256);
                inputstring[k] = (byte) value;
                if(silly_table3[inputstring[k] & 0xFF] > 0) {
                    count++;
                }
            }
            // always make this at least size + 4 to allow the algorithm to be smarter
            outputstring = new byte[count + 4];
        }
    }

    @Benchmark
    public void benchReplaceBackslashRawTable3(Blackhole blackhole, RawBenchmarkState state) {
        blackhole.consume(replaceBackslashRawTable3(state.inputstring, state.outputstring));
    }

    @Benchmark
    public void benchReplaceBackslashRawCompressedTable3(Blackhole blackhole, RawBenchmarkState state) {
        blackhole.consume(replaceBackslashRawCompressedTable3(state.inputstring, state.outputstring));
    }

    /**
     * this version looks appealing as it doesn't have branches, but it's a trap!
     * Looks at the data dependency to compute the index in the output array!<br>
     * It would be better instead to accumulate 4 output chars in a single long, while keeping track
     * of the empty slots in another mask and using {@link Long#compress(long, long)} and {@link Long#bitCount(long)}
     * to compute the index in the output array, while using a single batched store to write the compressed long.
     * Due to this batched store, we always need to have 7 more additional bytes in the output array.
     *
     * We still have a data dependency to know the next index, but in the best case scenario we could advance by 4 at time.
     */
    public static int replaceBackslashRawTable3(byte[] original, byte[] newArray) {
        int newArrayLength = 0;
        for (byte c : original) {
            newArrayLength = writeToOutput(newArray, c, newArrayLength);
        }
        return newArrayLength;
    }

    // This can be used to verify that we're not doing something terribly wrong!
    // It can become just a test, really
    public static void main(String[] args) {
        var state = new RawBenchmarkState();
        state.size = 65536;
        state.setUp();
        int out = replaceBackslashRawCompressedTable3(state.inputstring, state.outputstring);
        byte[] copy = Arrays.copyOf(state.outputstring, out);
        out = replaceBackslashRaw3(state.inputstring, state.outputstring);
        // check if the two arrays are the same and print the values and positions which are not
        byte[] good = Arrays.copyOf(state.outputstring, out);
        for (int i = 0; i < good.length; i++) {
            if (good[i] != copy[i]) {
                System.out.println("Position: " + i + " " + good[i] + " != " + copy[i]);
            }
        }
        System.out.println(Arrays.equals(copy, good));
    }

    private static final VarHandle LONG_COMPRESS_WRITER = MethodHandles.byteArrayViewVarHandle(long[].class, ByteOrder.LITTLE_ENDIAN);
    private static final VarHandle INT_READER = MethodHandles.byteArrayViewVarHandle(int[].class, ByteOrder.LITTLE_ENDIAN);

    public static int replaceBackslashRawCompressedTable3(byte[] original, byte[] newArray) {
        int newArrayLength = 0;
        int fourCharsBatches = original.length / 4;
        for (int b = 0; b < fourCharsBatches; b++) {
            int i = b * 4;
            long fourChars = 0;
            long compressMask = 0;
            int fourReadChars = (int) INT_READER.get(original, i);
            for (int j = 0; j < 4; j++) {
                int ch0 = (fourReadChars >>> (j * 8)) & 0xFF;
                byte b0 = silly_table3[ch0];
                int zeroIfEqualsZeroOrMinusOneIfNot = ((b0 | -b0) >> 31);
                // put the jth 2 bytes chars in the right fourChars slots
                int firstChar = ((~zeroIfEqualsZeroOrMinusOneIfNot & ch0) | (zeroIfEqualsZeroOrMinusOneIfNot & '\\'));
                int twoChars = (b0 << 8) | firstChar;
                fourChars |= (long) twoChars << (j * 16);
                // put the jth 2 bytes mask in the right compressMask slots
                compressMask |= (long) (~zeroIfEqualsZeroOrMinusOneIfNot & 0xFF00) << (j * 16);
            }
            compressMask = ~compressMask;
            // compress the fourChars into the newArray; this could be the same too!
            long compressedChars = Long.compress(fourChars, compressMask);
            LONG_COMPRESS_WRITER.set(newArray, newArrayLength, compressedChars);
            int chars = Long.bitCount(compressMask) / 8;
            newArrayLength += chars;
        }
        int tail = original.length % 4;
        for (int t = 0; t < tail; t++) {
            int i = fourCharsBatches * 4 + t;
            newArrayLength = writeToOutput(newArray, original[i], newArrayLength);
        }
        return newArrayLength;
    }

    private static final VarHandle SHORT_WRITER = MethodHandles.byteArrayViewVarHandle(short[].class, ByteOrder.LITTLE_ENDIAN);

    private static int writeToOutput(byte[] newArray, byte c, int newArrayLength) {
        int ch0 = c & 0xFF;
        byte b0 = silly_table3[ch0];
        int zeroIfEqualsZeroOrMinusOneIfNot = ((b0 | -b0) >> 31);
        // branch-less assign \\ if b != 0 or c if b == 0
        int firstChar = ((~zeroIfEqualsZeroOrMinusOneIfNot & ch0) | (zeroIfEqualsZeroOrMinusOneIfNot & '\\'));
        short twoChars = (short) ((b0 << 8) | firstChar);
        SHORT_WRITER.set(newArray, newArrayLength, twoChars);
        // branch-less increment by 2 if b != 0 or 1 if b == 0
        newArrayLength += 1 + (zeroIfEqualsZeroOrMinusOneIfNot & 1);
        return newArrayLength;
    }

    @Benchmark
    public void benchReplaceBackslashRaw3(Blackhole blackhole, RawBenchmarkState state) {
        blackhole.consume(replaceBackslashRaw3(state.inputstring, state.outputstring));
    }

    public static int replaceBackslashRaw3(byte[] original, byte[] newArray) {
        int index = 0;
        for (byte c : original) {
            if (c == '\\') {
                newArray[index] = '\\';
                newArray[index + 1] = '\\';
                index += 2;
            } else if (c == '\n') {
                newArray[index] = '\\';
                newArray[index + 1] = 'n';
                index += 2;
            } else if (c == '\t') {
                newArray[index] = '\\';
                newArray[index + 1] = 't';
                index += 2;
            } else {
                newArray[index] = c;
                index++;
            }
        }
        return index;
    }

    @Benchmark
    public void benchReplaceBackslash1(Blackhole blackhole, BenchmarkState state) {
        blackhole.consume(replaceBackslash1(state.inputstring, state.outputstring));
    }

    @Benchmark
    public void benchReplaceBackslashTable1(Blackhole blackhole, BenchmarkState state) {
        blackhole.consume(replaceBackslashTable1(state.inputstring, state.outputstring));
    }
    @Benchmark
    public void benchReplaceBackslash2(Blackhole blackhole, BenchmarkState state) {
        blackhole.consume(replaceBackslash2(state.inputstring, state.outputstring));
    }

    @Benchmark
    public void benchReplaceBackslashTable2(Blackhole blackhole, BenchmarkState state) {
        blackhole.consume(replaceBackslashTable2(state.inputstring, state.outputstring));
    }

    @Benchmark
    public void benchReplaceBackslash3(Blackhole blackhole, BenchmarkState state) {
        blackhole.consume(replaceBackslash3(state.inputstring, state.outputstring));
    }

    @Benchmark
    public void benchReplaceBackslashTable3(Blackhole blackhole, BenchmarkState state) {
        blackhole.consume(replaceBackslashTable3(state.inputstring, state.outputstring));
    }

}
