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
            // the existing algorithms can write 16 bytes more
            latinOutputString = new byte[outputLength + 16];
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

    private static final VarHandle LONG_WRITER = MethodHandles.byteArrayViewVarHandle(long[].class, ByteOrder.LITTLE_ENDIAN);

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
        // There is some problem with OSR compilation!
        // if OSR is on we need to replace this with
        // transformMSBSetIntoFF(setMSBonZeroBytes(input) ^ 0x8080808080808080L)
        return ~transformMSBSetIntoFF(setMSBonZeroBytes(input));
    }

    public static int replaceBackslashRawCompressedTable3(byte[] original, byte[] newArray) {
        int newArrayLength = 0;
        int eightCharsBatches = original.length / 8;
        for (int b = 0; b < eightCharsBatches; b++) {
            int i = b * 8;
            long readChars =  (long) LONG_WRITER.get(original, i);
            long replacements = readReplacements(readChars);
            int digits = addReplacedChars(newArray, readChars, replacements, newArrayLength, 4, 8);
            newArrayLength += digits;
        }
        int tail = original.length % 8;
        if (tail > 0) {
            int i = eightCharsBatches * 8;
            long readChars = readTailChars(original, i, tail);
            long replacements = readTailReplacements(readChars, tail);
            int digits = addReplacedChars(newArray, readChars, replacements, newArrayLength, Math.min(tail, 4), tail);
            newArrayLength += digits;
        }
        return newArrayLength;
    }

    private static long readTailChars(byte[] original, int index, int count) {
        assert count < 8;
        long chars = 0;
        int idx = index;
        long ch  = original[idx] & 0xFFL;
        chars |= ch;
        // TODO this could be implemented with batch operations
        //      7 bytes = 4 + 2 + 1 or 6 bytes = 4 + 2 or 5 bytes = 4 + 1
        if (count > 1) {
            ch = original[idx + 1] & 0xFFL;
            chars |= ch << 8;
            if (count > 2) {
                ch =  original[idx + 2] & 0xFFL;
                chars |= ch << 16;
                if (count > 3) {
                    ch =  original[idx + 3] & 0xFFL;
                    chars |= ch << 24;
                    if (count > 4) {
                        ch = original[idx + 4] & 0xFFL;
                        chars |= ch << 32;
                        if (count > 5) {
                            ch = original[idx + 5] & 0xFFL;
                            chars |= ch << 40;
                            if (count > 6) {
                                ch = original[idx + 6] & 0xFFL;;
                                chars |= ch << 48;
                            }
                        }
                    }
                }
            }
        }
        return chars;
    }

    private static long readTailReplacements(long chars, int count) {
        long b0 = silly_table3[(int) chars & 0xFF] & 0xFFL;
        long replacements = b0;
        if (count > 1) {
            long b1 = silly_table3[(int) ((chars >>> 8) & 0xFF)] & 0xFFL;
            replacements |= b1 << 8;
            if (count > 2) {
                long b2 = silly_table3[(int) ((chars >>> 16) & 0xFF)] & 0xFFL;
                replacements |= b2 << 16;
                if (count > 3) {
                    long b3 = silly_table3[(int) ((chars >>> 24) & 0xFF)] & 0xFFL;
                    replacements |= b3 << 24;
                    if (count > 4) {
                        long b4 = silly_table3[(int) ((chars >>> 32) & 0xFF)] & 0xFFL;
                        replacements |= b4 << 32;
                        if (count > 5) {
                            long b5 = silly_table3[(int) ((chars >>> 40) & 0xFF)] & 0xFFL;
                            replacements |= b5 << 40;
                            if (count > 6) {
                                long b6 = silly_table3[(int) ((chars >>> 48) & 0xFF)] & 0xFFL;;
                                replacements |= b6 << 48;
                            }
                        }
                    }
                }
            }
        }
        return replacements;
    }

    private static long readReplacements(long readChars) {
        long b0 = silly_table3[(int) readChars & 0xFF] & 0xFFL;
        long b1 = (silly_table3[(int) ((readChars >>> 8) & 0xFF)] & 0xFFL) << 8;
        long b2 = (silly_table3[(int) ((readChars >>> 16) & 0xFF)] & 0xFFL) << 16;
        long b3 = (silly_table3[(int) ((readChars >>> 24) & 0xFF)] & 0xFFL) << 24;
        long b4 = (silly_table3[(int) ((readChars >>> 32) & 0xFF)] & 0xFFL) << 32;
        long b5 = (silly_table3[(int) ((readChars >>> 40) & 0xFF)] & 0xFFL) << 40;
        long b6 = (silly_table3[(int) ((readChars >>> 48) & 0xFF)] & 0xFFL) << 48;
        long b7 = (silly_table3[(int) ((readChars >>> 56) & 0xFF)] & 0xFFL) << 56;
        long replacements = b0 | b1 | b2 | b3 | b4 | b5 | b6 | b7;
        return replacements;
    }


    private static int addReplacedChars(byte[] newArray, long chars, long replacements, int newArrayLength,
                                        int rightCharsCount, int totalChars) {
        // A mask which contains 0xFF for each char to replace
        long ffIfNotZero = ffNotZeroBytes(replacements);
        long replacedChars = (((~ffIfNotZero & chars)) | ffIfNotZero & 0x5c5c_5c5c_5c5c_5c5cL);
        long rightCharsAndReplacements  = Long.expand(replacedChars, 0x00FF_00FF_00FF_00FFL);
        rightCharsAndReplacements |= Long.expand(replacements, 0xFF00_FF00_FF00_FF00L);
        long rightCompressMask = Long.expand(ffIfNotZero , 0xFF00_FF00_FF00_FF00L) | 0x00FF_00FF_00FF_00FFL;
        LONG_WRITER.set(newArray, newArrayLength, Long.compress(rightCharsAndReplacements, rightCompressMask));
        long leftCharsAndReplacements = Long.expand(replacedChars >>> 32, 0x00FF_00FF_00FF_00FFL);
        leftCharsAndReplacements |= Long.expand(replacements >>> 32, 0xFF00_FF00_FF00_FF00L);
        int rightWrittenChars = rightCharsCount + Integer.bitCount((int) (ffIfNotZero & 0xFFFFFFFFL)) / 8;
        long leftCompressMask = Long.expand(ffIfNotZero >>> 32, 0xFF00_FF00_FF00_FF00L) | 0x00FF_00FF_00FF_00FFL;
        LONG_WRITER.set(newArray, newArrayLength + rightWrittenChars, Long.compress(leftCharsAndReplacements, leftCompressMask));
        final int totalWrittenChars = totalChars + Long.bitCount(ffIfNotZero) / 8;
        return totalWrittenChars;
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
