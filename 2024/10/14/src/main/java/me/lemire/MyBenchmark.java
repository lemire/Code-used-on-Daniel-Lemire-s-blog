package me.lemire;

import org.openjdk.jmh.annotations.Benchmark;

import org.openjdk.jmh.annotations.*;
import org.openjdk.jmh.infra.BenchmarkParams;
import org.openjdk.jmh.infra.Blackhole;

import java.lang.invoke.MethodHandles;
import java.lang.invoke.VarHandle;
import java.nio.ByteOrder;
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
        public byte[] latinInputString;
        public char[] outputstring;
        public byte[] latinOutputString;


        @Setup(Level.Trial)
        public void setUp(BenchmarkParams params) {
            inputstring = new char[size];
            int outputLength = populateChars(params, (index, latinChar) -> inputstring[index] = (char) latinChar, size, specialCharPercentage);
            outputstring = new char[outputLength];
            latinInputString = new byte[size];
            for (int i = 0; i < size; i++) {
                latinInputString[i] = (byte) inputstring[i];
            }
            // the existing algorithms can write 7 bytes more
            latinOutputString = new byte[outputLength + 7];
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

    private static final VarHandle LONG_COMPRESS_WRITER = MethodHandles.byteArrayViewVarHandle(long[].class, ByteOrder.LITTLE_ENDIAN);
    private static final VarHandle INT_READER = MethodHandles.byteArrayViewVarHandle(int[].class, ByteOrder.LITTLE_ENDIAN);


    public static long transformMSBSetIntoFF(long input) {
        // the JIT compiler won't use imul for this :P
        return ((input >>> 7) * 0xFF);
    }

    // set the MSB of the zero bytes, zero otherwise
    private static long setMSBonZeroBytes(long word) {
        long tmp = (word & 0x7F7F7F7F7F7F7F7FL) + 0x7F7F7F7F7F7F7F7FL;
        // this is necessary since we can have negative ones, which already set the MSB (!)
        tmp = ~(tmp | word | 0x7F7F7F7F7F7F7F7FL);
        return tmp;
    }

    private static long ffNotZeroBytes(long input) {
        return transformMSBSetIntoFF(setMSBonZeroBytes(input) ^ 0x8080808080808080L);
    }

    public static int replaceBackslashRawCompressedTable3(byte[] original, byte[] newArray) {
        int newArrayLength = 0;
        int fourCharsBatches = original.length / 4;
        for (int b = 0; b < fourCharsBatches; b++) {
            int i = b * 4;
            int readChars = (int) INT_READER.get(original, i);
            long replacementWithOriginalChars = readCharsWithReplacements(readChars);
            int digits = addReplacedChars(newArray, replacementWithOriginalChars, newArrayLength);
            newArrayLength += digits;
        }
        int tail = original.length % 4;
        if (tail > 0) {
            long latinChars = readTailCharsWithReplacements(original, fourCharsBatches, tail);
            int digits = addReplacedChars(newArray, latinChars, newArrayLength);
            newArrayLength += digits;
        }
        return newArrayLength;
    }

    // it returns 0xRRSS_RRSS_RRSS_RRSS with RR the replacement char found on silly_table3[SS] and SS the original char
    private static long readCharsWithReplacements(int readChars) {
        long latinChars = Long.expand(Integer.toUnsignedLong(readChars), 0x00FF_00FF_00FF_00FFL);
        byte b0 = silly_table3[(int) (latinChars & 0xFF)];
        latinChars |= (long) b0 << 8;
        byte b1 = silly_table3[(int) ((latinChars >>> 16) & 0xFF)];
        latinChars |= (long) b1 << 24;
        byte b2 = silly_table3[(int) ((latinChars >>> 32) & 0xFF)];
        latinChars |= (long) b2 << 40;
        byte b3 = silly_table3[(int) ((latinChars >>> 48) & 0xFF)];
        latinChars |= (long) b3 << 56;
        return latinChars;
    }

    private static long readTailCharsWithReplacements(byte[] original, int fourCharsBatches, int tail) {
        long latinChars = 0;
        int idx = fourCharsBatches * 4;
        byte ch  = original[idx];
        byte b0 = silly_table3[(ch & 0xFF)];
        // place this near to the latinChars it refer to
        latinChars |= ch;
        latinChars |= (long) b0 << 8;
        if (tail > 1) {
            ch = original[idx + 1];
            byte b1 = silly_table3[(ch & 0xFF)];
            latinChars |= (long) ch << 16;
            latinChars |= (long) b1 << 24;
            if (tail > 2) {
                ch =  original[idx + 2];
                byte b2 = silly_table3[(ch & 0xFF)];
                latinChars |= (long) ch << 32;
                latinChars |= (long) b2 << 40;
            }
        }
        return latinChars;
    }

    private static int addReplacedChars(byte[] newArray, long replacementsWithChars, int newArrayLength) {
        // A mask which contains 0xFF for each original char to replace and 0xFF for each replacement char
        // eg:  0x5c5c_0061_0061_0061 -> ffIfNotZero := 0xFFFF_FF00_FF00_FF00
        long ffIfNotZero = ((ffNotZeroBytes(replacementsWithChars) & 0xFF00_FF00_FF00_FF00L) >>> 8) | 0xFF00_FF00_FF00_FF00L;
        // uses the mask to replace each original char with '\' (0x5c)
        // if the mask obtained at the previous step contains 0xFF
        // otherwise we keep the original char
        long replacedChars = (((~ffIfNotZero & (replacementsWithChars & 0x00FF_00FF_00FF_00FFL)) |
                ffIfNotZero & 0x005c_005c_005c_005cL) | (replacementsWithChars & 0xFF00_FF00_FF00_FF00L));
        // uses the mask to compress each pair if the replacement hasn't happened
        // i.e. we move it to the right by 8 bits since we care only about valid replacement chars
        long compressedChars = Long.compress(replacedChars, (ffIfNotZero << 8) | 0x00FF_00FF_00FF_00FFL);
        LONG_COMPRESS_WRITER.set(newArray, newArrayLength, compressedChars);
        // it's fine to use popcnt on ffIfNotZero i.e. the number of 0xFF := 4 + number of replacements
        int digits = Long.bitCount(ffIfNotZero) / 8;
        return digits;
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

    @Benchmark
    public void benchReplaceBackslashRawCompressedTable3(Blackhole blackhole, BenchmarkState state) {
        blackhole.consume(replaceBackslashRawCompressedTable3(state.latinInputString, state.latinOutputString));
    }

}
