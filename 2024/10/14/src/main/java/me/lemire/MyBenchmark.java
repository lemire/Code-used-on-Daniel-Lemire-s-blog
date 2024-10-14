/*
 * Copyright (c) 2014, Oracle America, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 *  * Neither the name of Oracle nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

package me.lemire;

import org.openjdk.jmh.annotations.Benchmark;
import java.util.*;
import java.util.concurrent.TimeUnit;
import java.util.stream.Collectors;

import org.openjdk.jmh.annotations.*;
import org.openjdk.jmh.runner.Runner;
import org.openjdk.jmh.runner.RunnerException;
import org.openjdk.jmh.runner.options.Options;
import org.openjdk.jmh.runner.options.OptionsBuilder;
import org.openjdk.jmh.infra.Blackhole;
import java.util.concurrent.ThreadLocalRandom;

@Measurement(iterations = 10, time = 1)
@Warmup(iterations = 5, time = 1)
@Fork(value = 2)
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
        int index = 0;
        for (char c : original) {
            byte b = silly_table1[c%256];
			if (c < 256 &&  b != 0) {
				newArray[index++] = '\\';
				newArray[index++] = (char)b;
			} else {
				newArray[index++] = c;
			}
        }
		return index;
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
        int index = 0;
        for (char c : original) {
            byte b = silly_table2[c%256];
			if (c < 256 &&  b > 0) {
				newArray[index++] = '\\';
				newArray[index++] = (char)b;
			} else {
				newArray[index++] = c;
			}
        }
		return index;
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
        int index = 0;
        for (char c : original) {
            byte b = silly_table2[c%256];
			if (c < 256 && b > 0) {
				newArray[index++] = '\\';
				newArray[index++] = (char)b;
			} else {
				newArray[index++] = c;
			}
        }
		return index;
    }

	@State(Scope.Benchmark)
	public static class BenchmarkState {
		@Param({"65536"})
		public int size;
		public char[] inputstring;
		public char[] outputstring;


		@Setup(Level.Trial)
		public void setUp() {
			ThreadLocalRandom random = ThreadLocalRandom.current();
			inputstring = new char[size];
			int count = size;
			for(int k = 0; k < size; k++) {
				inputstring[k] = (char)random.nextInt(0, 256);
				if(silly_table3[inputstring[k]] > 0) {
					count++;
				}
			}
			outputstring = new char[count];
		}

		@Setup(Level.Invocation)
		public void perInvocation(){
		}
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
