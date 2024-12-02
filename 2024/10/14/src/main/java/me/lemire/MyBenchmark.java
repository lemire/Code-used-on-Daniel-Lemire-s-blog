package me.lemire;

import org.openjdk.jmh.annotations.Benchmark;

import org.openjdk.jmh.annotations.*;
import org.openjdk.jmh.infra.BenchmarkParams;
import org.openjdk.jmh.infra.Blackhole;

import java.util.Arrays;
import java.util.BitSet;
import java.util.Random;

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


        @Param({"3", "50"})
        public int specialCharPercentage;

        @Param({"65536"})
        public int size;
        public char[] inputstring;
        public char[] outputstring;


        @Setup(Level.Trial)
        public void setUp(BenchmarkParams params) {
            inputstring = new char[size];
            int outputLength = populateChars(params, (index, latinChar) -> inputstring[index] = (char) latinChar, size, specialCharPercentage);
            outputstring = new char[outputLength];
        }
    }

    // better be safe and keep it the same to have reproducible results
    private static final int SEED = 42;

    @FunctionalInterface
    private interface IntIntBiConsumer {
        void accept(int index, int latinChar);
    }

    /**
     * Populates the input string with latin characters and special characters, based on the specialCharsProbability:
     */
    private static int populateChars(BenchmarkParams params, IntIntBiConsumer latinCharProducer, int count,
                                     int specialCharsProbability) {
        byte[] specialChars = specialCharsFor(params);
        byte[] latinChars = latinCharsExcluding(specialChars);
        Random random = new Random(SEED);
        int outputLength = count;
        for (int i = 0; i < count; i++) {
            if (random.nextInt( 100) <= specialCharsProbability) {
                int specialCharIndex = random.nextInt(specialChars.length);
                latinCharProducer.accept(i, Byte.toUnsignedInt(specialChars[specialCharIndex]));
                outputLength++;
            } else {
                int latinCharIndex = random.nextInt(latinChars.length);
                latinCharProducer.accept(i, Byte.toUnsignedInt(latinChars[latinCharIndex]));
            }
        }
         return outputLength;
    }

    private static byte[] specialCharsFor(BenchmarkParams params) {
        final byte[] specialChars;
        if (params.getBenchmark().contains("1")) {
            specialChars = new byte[1];
            specialChars[0] = '\\';
        } else if (params.getBenchmark().contains("2")) {
            specialChars = new byte[2];
            specialChars[0] = '\\';
            specialChars[1] = '\n';
        } else if (params.getBenchmark().contains("3")) {
            specialChars = new byte[3];
            specialChars[0] = '\\';
            specialChars[1] = '\n';
            specialChars[2] = '\t';
        } else {
            throw new IllegalArgumentException("Unknown benchmark: " + params.getBenchmark());
        }
        return specialChars;
    }

    private static byte[] latinCharsExcluding(byte[] specialChars) {
        BitSet specialCharsSet = new BitSet(256);
        for (byte specialChar : specialChars) {
            specialCharsSet.set(specialChar);
        }
        BitSet latinChars = new BitSet(256);
        latinChars.set(0, 255);
        latinChars.andNot(specialCharsSet);
        byte[] nonSpecialLatinChars = new byte[latinChars.cardinality()];
        for (int i = latinChars.nextSetBit(0), j = 0; i >= 0; i = latinChars.nextSetBit(i + 1), j++) {
            nonSpecialLatinChars[j] = (byte) i;
        }
        return nonSpecialLatinChars;
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
