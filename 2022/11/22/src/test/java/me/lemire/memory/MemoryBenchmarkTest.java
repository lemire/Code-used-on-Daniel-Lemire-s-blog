package me.lemire.memory;

import org.github.jamm.*;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.Parameterized;
import org.junit.runners.Parameterized.Parameter;
import org.junit.runners.Parameterized.Parameters;

import java.util.*;
import java.util.concurrent.*;
import java.text.DecimalFormat;


public class MemoryBenchmarkTest {


    @Test
    public void benchmarkSpec() throws Exception {
        MemoryMeter.Guess guess = MemoryMeter.Guess.valueOf("ALWAYS_SPEC");
        MemoryMeter meter = meter = MemoryMeter.builder()
                                .withGuessing(guess)
                                .build();

       for(int k = 0; k < 16; ++k) {
          System.out.println(" memory usage of a byte["+k+"] "+meter.measureDeep(new byte[k]));
        }
    }




}
