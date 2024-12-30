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
    private static final short[] replacementsAndCompressionTables = new short[256 * 2];

    static {
        for (int i = 0; i < 256; i++) {
            replacementsAndCompressionTables[i * 2] = (short) ((i & 0xFF) << 8);
            replacementsAndCompressionTables[(i * 2) + 1] = (short) 0xFF00;
        }
        // we need to override the default values
        replacementsAndCompressionTables['\\' * 2] = (short) (0x005c | (('\\' & 0xFF) << 8));
        replacementsAndCompressionTables[('\\' * 2) + 1] = (short) 0xFFFF;
        replacementsAndCompressionTables['\n' * 2] = (short) (0x005c | (('n' & 0xFF) << 8));
        replacementsAndCompressionTables[('\n' * 2) + 1] = (short) 0xFFFF;
        replacementsAndCompressionTables['\t' * 2] = (short) (0x005c | (('t' & 0xFF) << 8));
        replacementsAndCompressionTables[('\t' * 2) + 1] = (short) 0xFFFF;

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

    public static void main(String[] args) {
        replaceBackslashRawCompressedTable3(new byte[] { 'a', '\\' , 'b', '\n', 'c', '\t', 'd', 'e', '\\', '\n', 'a'}, new byte[(11 * 2) + 16]);
    }

    public static int replaceBackslashRawCompressedTable3(byte[] original, byte[] newArray) {
        int newArrayLength = 0;
        int eightCharsBatches = original.length / 8;
        for (int b = 0; b < eightCharsBatches; b++) {
            int i = b * 8;
            long readChars =  (long) LONG_WRITER.get(original, i);
            int digits = Long.bitCount(writeReplacementPart(newArray, newArrayLength, (int) (readChars & 0xFFFF_FFFFL))) / 8;
            digits += (Long.bitCount(writeReplacementPart(newArray, newArrayLength + digits, (int)((readChars >>> 32) & 0xFFFFFFFFL))) / 8);
            newArrayLength += digits;
        }
        int tail = original.length % 8;
        if (tail > 0) {
            int i = eightCharsBatches * 8;
            long readChars = readTailChars(original, i, tail);
            // since the compression mask for 0 is 0xFF00 we need to trust tail size
            int additionalCharsRight = Long.bitCount(writeReplacementPart(newArray, newArrayLength, (int) (readChars & 0xFFFF_FFFFL)) & 0x00FF_00FF_00FF_00FFL) / 8;
            int charsWrittenRight = Math.min(tail, 4) + additionalCharsRight;
            int additionalCharsLeft = Long.bitCount(writeReplacementPart(newArray, newArrayLength + charsWrittenRight, (int)((readChars >>> 32) & 0xFFFFFFFFL)) & 0x00FF_00FF_00FF_00FFL) / 8;
            newArrayLength += (tail + additionalCharsRight + additionalCharsLeft);
        }
        return newArrayLength;
    }

    private static long writeReplacementPart(byte[] newArray, int newArrayLength, int readChars) {
        long compressionMask = 0;
        long replacements = 0;
        for (int i = 0; i < 4; i++) {
            int c = (readChars >>> (i * 8)) & 0xFF;
            long replacement = Short.toUnsignedLong(replacementsAndCompressionTables[c * 2]);
            long mask = Short.toUnsignedLong(replacementsAndCompressionTables[(c * 2) + 1]);
            replacements |= (replacement << (i * 16));
            compressionMask |= (mask << (i * 16));
        }
        LONG_WRITER.set(newArray, newArrayLength, Long.compress(replacements, compressionMask));
        return compressionMask;
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
