package me.lemire.microbenchmarks.algorithms;

import java.util.*;
import java.util.concurrent.*;
import java.util.stream.Collectors;
import java.nio.charset.*;
import java.nio.*;

import org.openjdk.jmh.annotations.*;
import org.openjdk.jmh.runner.Runner;
import org.openjdk.jmh.runner.RunnerException;
import org.openjdk.jmh.runner.options.Options;
import org.openjdk.jmh.runner.options.OptionsBuilder;

@State(Scope.Benchmark)
@BenchmarkMode(Mode.AverageTime)
@OutputTimeUnit(TimeUnit.MICROSECONDS)
@Fork(1)
@Warmup(iterations = 2, time = 500, timeUnit = TimeUnit.MILLISECONDS)
@Measurement(iterations = 3, time = 500, timeUnit = TimeUnit.MILLISECONDS)
public class Utf8Validate {
  

  @Param({"ASCII", "UTF8"})
  static String mode;
    
  @Param({"191"})
  static int N;


  @State(Scope.Benchmark)
  public static class MyState {
      public ArrayList<byte[]> datautf8;// = getUTF8String();
      static Random r = new Random(1234);

      public MyState() {
        datautf8 = new ArrayList<byte[]>();
        int total = 0;
        do {
          byte[] c = getUTF8String();
          total += c.length;
          datautf8.add(c);
        } while(total < 10000000);
        System.out.println("==Size of the input string in  bytes "+total);
      }
      private static byte[] getUTF8String() {
        String vals[] = null;
        String asci[] = {"a "," working ", "potato", "hmm?", "in the garange!", "the dog", "Daniel", "elephant", "cylon "};
        String utf[] = {"a "," working ", "potato", "«hmm?»", "in the garange!", "look!","...","" ,  "the ", "dog ", "Daniel ", "éléphant ","®", "↧", "⿐" ,"😨","😧","😦","😱","😫","😩"};
        
        if(mode.equals("ASCII")) 
         vals = asci;
        else if(mode .equals("UTF8"))
         vals = utf;
        else System.err.println("mode = "+mode);
        StringBuilder builder = new StringBuilder();
        for(int i = 0; i < N; i++) {
          builder.append(vals[r.nextInt(vals.length)]);
        }
        String answer = builder.toString();
        // we are going to make sure that the UTF-8 is proper
        byte[] byteanswer = answer.getBytes(StandardCharsets.UTF_8);
        if(! new String(byteanswer,StandardCharsets.UTF_8).equals(answer) )
          throw new RuntimeException("utf8 mismatch.");
        return byteanswer;
      }
  }

/** 
BEGIN COPY-PASTE FROM Guava
 **/

  /**
   * Returns {@code true} if {@code bytes} is a <i>well-formed</i> UTF-8 byte sequence according to
   * Unicode 6.0. Note that this is a stronger criterion than simply whether the bytes can be
   * decoded. For example, some versions of the JDK decoder will accept "non-shortest form" byte
   * sequences, but encoding never reproduces these. Such byte sequences are <i>not</i> considered
   * well-formed.
   *
   * <p>This method returns {@code true} if and only if {@code Arrays.equals(bytes, new
   * String(bytes, UTF_8).getBytes(UTF_8))} does, but is more efficient in both time and space.
   */
  public static boolean isWellFormed(byte[] bytes) {
    return isWellFormed(bytes, 0, bytes.length);
  }

  /**
   * Returns whether the given byte array slice is a well-formed UTF-8 byte sequence, as defined by
   * {@link #isWellFormed(byte[])}. Note that this can be false even when {@code
   * isWellFormed(bytes)} is true.
   *
   * @param bytes the input buffer
   * @param off the offset in the buffer of the first byte to read
   * @param len the number of bytes to read from the buffer
   */
  public static boolean isWellFormed(byte[] bytes, int off, int len) {
    int end = off + len;

    // Look for the first non-ASCII character.
    for (int i = off; i < end; i++) {
      if (bytes[i] < 0) {
        return isWellFormedSlowPath(bytes, i, end);
      }
    }
    return true;
  }

  private static boolean isWellFormedSlowPath(byte[] bytes, int off, int end) {
    int index = off;
    while (true) {
      int byte1;

      // Optimize for interior runs of ASCII bytes.
      do {
        if (index >= end) {
          return true;
        }
      } while ((byte1 = bytes[index++]) >= 0);
      if (byte1 < (byte) 0xE0) {
        // Two-byte form.
        if (index == end) {
          return false;
        }
        // Simultaneously check for illegal trailing-byte in leading position
        // and overlong 2-byte form.
        if (byte1 < (byte) 0xC2 || bytes[index++] > (byte) 0xBF) {
          return false;
        }
      } else if (byte1 < (byte) 0xF0) {
        // Three-byte form.
        if (index + 1 >= end) {
          return false;
        }
        int byte2 = bytes[index++];
        if (byte2 > (byte) 0xBF
            // Overlong? 5 most significant bits must not all be zero.
            || (byte1 == (byte) 0xE0 && byte2 < (byte) 0xA0)
            // Check for illegal surrogate codepoints.
            || (byte1 == (byte) 0xED && (byte) 0xA0 <= byte2)
            // Third byte trailing-byte test.
            || bytes[index++] > (byte) 0xBF) {
          return false;
        }
      } else {
        // Four-byte form.
        if (index + 2 >= end) {
          return false;
        }
        int byte2 = bytes[index++];
        if (byte2 > (byte) 0xBF
            // Check that 1 <= plane <= 16. Tricky optimized form of:
            // if (byte1 > (byte) 0xF4
            //     || byte1 == (byte) 0xF0 && byte2 < (byte) 0x90
            //     || byte1 == (byte) 0xF4 && byte2 > (byte) 0x8F)
            || (((byte1 << 28) + (byte2 - (byte) 0x90)) >> 30) != 0
            // Third byte trailing-byte test
            || bytes[index++] > (byte) 0xBF
            // Fourth byte trailing-byte test
            || bytes[index++] > (byte) 0xBF) {
          return false;
        }
      }
    }
  }
  public static boolean isWellFormedSimpler(byte[] bytes) {
    return isWellFormedSimpler(bytes, 0, bytes.length);
  }
  public static boolean isWellFormedSimpler(byte[] bytes, int off, int len) {
    int end = off + len;

    // Look for the first non-ASCII character.
    for (int i = off; i < end; i++) {
      if (bytes[i] < 0) {
        return isWellFormedSlowPath(bytes, i, end);
      }
    }
    return true;
  }

  private static boolean isWellFormedSlowPathSimpler(byte[] bytes, int off, int end) {
    int index = off;
    while (index < end) {
      int byte1 = bytes[index++];
      if(byte1 >= 0) {
        // ASCII!!!
      } else if (byte1 < (byte) 0xE0) {
        // Two-byte form.
        if (index == end) {
          return false;
        }
        // Simultaneously check for illegal trailing-byte in leading position
        // and overlong 2-byte form.
        if (byte1 < (byte) 0xC2 || bytes[index++] > (byte) 0xBF) {
          return false;
        }
      } else if (byte1 < (byte) 0xF0) {
        // Three-byte form.
        if (index + 1 >= end) {
          return false;
        }
        int byte2 = bytes[index++];
        if (byte2 > (byte) 0xBF
            // Overlong? 5 most significant bits must not all be zero.
            || (byte1 == (byte) 0xE0 && byte2 < (byte) 0xA0)
            // Check for illegal surrogate codepoints.
            || (byte1 == (byte) 0xED && (byte) 0xA0 <= byte2)
            // Third byte trailing-byte test.
            || bytes[index++] > (byte) 0xBF) {
          return false;
        }
      } else {
        // Four-byte form.
        if (index + 2 >= end) {
          return false;
        }
        int byte2 = bytes[index++];
        if (byte2 > (byte) 0xBF
            // Check that 1 <= plane <= 16. Tricky optimized form of:
            // if (byte1 > (byte) 0xF4
            //     || byte1 == (byte) 0xF0 && byte2 < (byte) 0x90
            //     || byte1 == (byte) 0xF4 && byte2 > (byte) 0x8F)
            || (((byte1 << 28) + (byte2 - (byte) 0x90)) >> 30) != 0
            // Third byte trailing-byte test
            || bytes[index++] > (byte) 0xBF
            // Fourth byte trailing-byte test
            || bytes[index++] > (byte) 0xBF) {
          return false;
        }
      }
    }
    return true;
  }



/** 
   END COPY-PASTE FROM Guava
 **/

static final byte utf8d_toclass[] = {
  // The first part of the table maps bytes to character classes that
  // to reduce the size of the transition table and create bitmasks.
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
  8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
  10,3,3,3,3,3,3,3,3,3,3,3,3,4,3,3, 11,6,6,6,5,8,8,8,8,8,8,8,8,8,8,8
};

static final byte utf8d_transition[] = {
 // The second part is a transition table that maps a combination
 // of a state of the automaton and a character class to a state.
  0,12,24,36,60,96,84,12,12,12,48,72, 12,12,12,12,12,12,12,12,12,12,12,12,
  12, 0,12,12,12,12,12, 0,12, 0,12,12, 12,24,12,12,12,12,12,24,12,24,12,12,
  12,12,12,12,12,12,12,24,12,12,12,12, 12,24,12,12,12,12,12,12,12,24,12,12,
  12,12,12,12,12,12,12,36,12,36,12,12, 12,36,12,12,12,12,12,36,12,36,12,12,
  12,36,12,12,12,12,12,12,12,12,12,12, 
  // padding so we get at 512 and the compiler can determine
  // that there is no overflow
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  
};

  public static boolean isASCII(byte[] b) {
    int length = b.length;
    int s = 0;
    for (int i = 0; i < length; i++) {
      s |= b[i] & 0x80;
    }
    return s == 0;
  }

  public static boolean isASCIIbranch(byte[] b) {
    int length = b.length;
    for (int i = 0; i < length; i++) {
      if (b[i] < 0) return false;
    }
    return true;
  }

  // http://bjoern.hoehrmann.de/utf-8/decoder/dfa/
  public static boolean isUTF8(byte[] b) {
    int length = b.length;
    int s = 0;
    for (int i = 0; i < length; i++) {
      s = utf8d_transition[(s + (utf8d_toclass[b[i] & 0xFF])) & 0xFF];
    }
    return s == 0;
  }
  
  public static final boolean isTrailingByte(byte b) {
	  // check that the byte is a trailing byte of the form 10xxxxxx
	  return b <= (byte)0xBF; 
  }
  
  // two stream version of 
  // http://bjoern.hoehrmann.de/utf-8/decoder/dfa/
  public static boolean isUTF8_double(byte[] b) {
    int length = b.length, half = length / 2;
    while (b[half] <= (byte)0xBF && b[half] >= (byte)0x80 && half > 0) {
    	half--;
    }
    int s1 = 0, s2 = 0;
    for (int i = 0, j = half; i < half; i++, j++) {
      s1 = utf8d_transition[(s1 + (utf8d_toclass[b[i] & 0xFF])) & 0xFF];
      s2 = utf8d_transition[(s2 + (utf8d_toclass[b[j] & 0xFF])) & 0xFF];
    }
    for (int j = half * 2; j < b.length; j++) {
        s2 = utf8d_transition[(s2 + (utf8d_toclass[b[j] & 0xFF])) & 0xFF];
    }
    return s1 == 0 && s2 == 0;
  }
  
  //
  // The three and the four stream versions are incorrect deliberately
  // Travis wrote:
  // Note that these functions don't actually return the right result much of the time: 
  // I left out the code to actually align each stream to a code point boundary and also
  // the code to stop on a code point boundary. You could do it like I did in the double case, 
  // but it also occurs to me you can do it for free using the existing DFA: start each stream 
  // in an special "start" state whose transitions are such that the initial bytes are ignored 
  // (i.e., they transition to the normal state graph as soon as they see a starting character), 
  // and as the final check just check at the state is not invalid (s1 != 12) rather than 
  // "valid end of character" (s0 == 0). Or something like that. I didn't worry about it because 
  // this is a small one-time cost which doesn't affect the main loop, except if it somehow makes the JIT produce worse code.
  
  // three stream version of 
  // http://bjoern.hoehrmann.de/utf-8/decoder/dfa/
  public static boolean isUTF8_triple(byte[] b) {
    int length = b.length, chunk = length / 3;
    int s1 = 0, s2 = 0, s3 = 0;
    for (int i = 0, j = chunk, k = 2 * chunk; i < chunk; i++, j++, k++) {
      s1 = utf8d_transition[(s1 + (utf8d_toclass[b[i] & 0xFF])) & 0xFF];
      s2 = utf8d_transition[(s2 + (utf8d_toclass[b[j] & 0xFF])) & 0xFF];
      s3 = utf8d_transition[(s3 + (utf8d_toclass[b[k] & 0xFF])) & 0xFF];
    }
    return s1 == 0 && s2 == 0 && s3 == 0;
  }
  
 // four stream version of 
 // http://bjoern.hoehrmann.de/utf-8/decoder/dfa/
 public static boolean isUTF8_quad(byte[] b) {
   int length = b.length, chunk = length / 4;
   int s1 = 0, s2 = 0, s3 = 0, s4 = 0;
   for (int i = 0, j = chunk, k = 2 * chunk, l = 3 * chunk; i < chunk; i++, j++, k++, l++) {
     s1 = utf8d_transition[(s1 + (utf8d_toclass[b[i] & 0xFF])) & 0xFF];
     s2 = utf8d_transition[(s2 + (utf8d_toclass[b[j] & 0xFF])) & 0xFF];
     s3 = utf8d_transition[(s3 + (utf8d_toclass[b[k] & 0xFF])) & 0xFF];
     s4 = utf8d_transition[(s4 + (utf8d_toclass[b[l] & 0xFF])) & 0xFF];
   }
   return s1 == 0 && s2 == 0 && s3 == 0 && s4 == 0;
 }

  @Benchmark @BenchmarkMode(Mode.AverageTime) 
  @OutputTimeUnit(TimeUnit.NANOSECONDS)
  public boolean testFSM_utf8(MyState state) {
    for(byte[] x : state.datautf8)
      if(!isUTF8(x)) throw new RuntimeException("not UTF8?");
    return true;
  }
  
  @Benchmark @BenchmarkMode(Mode.AverageTime) 
  @OutputTimeUnit(TimeUnit.NANOSECONDS)
  public boolean testFSM2_utf8(MyState state) {
    for(byte[] x : state.datautf8)
      if(!isUTF8_double(x)) throw new RuntimeException("not UTF8?");
    return true;
  }
  
  @Benchmark @BenchmarkMode(Mode.AverageTime) 
  @OutputTimeUnit(TimeUnit.NANOSECONDS)
  public boolean testFSM3_utf8(MyState state) {
    for(byte[] x : state.datautf8)
      if(!isUTF8_triple(x)) throw new RuntimeException("not UTF8?");
    return true;
  }
  
  @Benchmark @BenchmarkMode(Mode.AverageTime) 
  @OutputTimeUnit(TimeUnit.NANOSECONDS)
  public boolean testFSM4_utf8(MyState state) {
    for(byte[] x : state.datautf8)
      if(!isUTF8_quad(x)) throw new RuntimeException("not UTF8?");
    return true;
  }

  @Benchmark @BenchmarkMode(Mode.AverageTime) 
  @OutputTimeUnit(TimeUnit.NANOSECONDS)
  public boolean testGuava_utf8(MyState state) {
    for(byte[] x : state.datautf8)
      if(!isWellFormed(x)) throw new RuntimeException("not UTF8?");
    return true;

  }


  @Benchmark @BenchmarkMode(Mode.AverageTime) 
  @OutputTimeUnit(TimeUnit.NANOSECONDS)
  public boolean testGuavaSimpler_utf8(MyState state) {
    for(byte[] x : state.datautf8) {
      if(!isWellFormedSimpler(x)) throw new RuntimeException("not UTF8?");
    }
    return true;

  }

  @Benchmark @BenchmarkMode(Mode.AverageTime) 
  @OutputTimeUnit(TimeUnit.NANOSECONDS)
  public boolean testJava_utf8(MyState state) { 
    CharsetDecoder decoder = StandardCharsets.UTF_8.newDecoder();
    for(byte[] x : state.datautf8) {
      try {
        //CharBuffer b = 
         decoder.decode(ByteBuffer.wrap(x));		           
      } catch (CharacterCodingException ex) {		        
        //return false;
         throw new RuntimeException("not UTF8?");
      } 
    }
    return true;
  }




//  public static void main(String[] args) throws RunnerException {
//        Options opt = new OptionsBuilder()
//        .include(Utf8Validate.class.getSimpleName()).warmupIterations(3)
//        .measurementIterations(3).forks(1).build();
//        new Runner(opt).run();
//
//  }

}
