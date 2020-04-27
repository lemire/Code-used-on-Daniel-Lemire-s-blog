import java.util.Arrays;
import java.util.concurrent.ThreadLocalRandom;


public class sample {
    static ThreadLocalRandom r = ThreadLocalRandom.current();

    public static int naiveRandomSample(int[] histo, int[] output) {
        int[] runninghisto = Arrays.copyOf(histo, histo.length);
        int sum = 0;
        for (int z = 0; z < histo.length; z++) {
            sum += histo[z];
        }
        int pos = 0;
        while (sum > 0) {
            int y = r.nextInt(sum); // random integer in [0,sum)
            // select the key in linear time
            int runningsum = 0;
            int z = 0;
            for (; z < runninghisto.length; z++) {
                runningsum += runninghisto[z];
                if (y < runningsum) {
                    break;
                }
            }
            output[pos++] = z;
            runninghisto[z] -= 1;
            sum -= 1;
        }
        return pos;
    }

    public static int smarterRandomSample(int[] histo, int[] output) {


        int sum = 0;
        for (int z = 0; z < histo.length; z++) {
            sum += histo[z];
        }
        // build tree
        int[] runninghisto = Arrays.copyOf(histo, histo.length);
        int level = 0;
        for (;
            (1 << level) < runninghisto.length; level++) {
            for (int z = (1 << level) - 1; z + (1 << level) < runninghisto.length; z += 2 * (1 << level)) {
                runninghisto[z + (1 << level)] += runninghisto[z];
            }
        }
        int maxlevel = level;
        int pos = 0;
        while (sum > 0) {
            int y = r.nextInt(sum); // random integer in [0,sum)
            // select logarithmic time
            level = maxlevel;
            int position = (1 << level) - 1;
            int runningsum = 0;
            for (; level >= 0; level -= 1) {
                if (y > runningsum + runninghisto[position]) {
                    runningsum += runninghisto[position];
                    position += (1 << level) / 2;
                } else if (y == runningsum + runninghisto[position]) {
                    runninghisto[position] -= 1;
                    break;
                } else {
                    runninghisto[position] -= 1;
                    position -= (1 << level) / 2;
                }
            }
            output[pos++] = position;
            sum -= 1;
        }
        return pos;
    }

    public static void demo() {
        int N = 1024;
        int M = 1000000;
        int[] histo = new int[N];
        int s = 0;
        for (int z = 0; z < histo.length; z++) {
            int k = 1; //r.nextInt(M);
            s += k;
            histo[z] = k;
        }
        int[] output = new int[s];
        long t1 = System.nanoTime();
        int c1 = naiveRandomSample(histo, output);
        long t2 = System.nanoTime();
        if (c1 != s) {
            System.err.println("bug1 " + c1 + " " + s);
        }
        int c2 = smarterRandomSample(histo, output);
        long t3 = System.nanoTime();
        if (c2 != s) {
            System.err.println("bug2 " + c2 + " " + s);
        }
        System.out.println((t2 - t1) * 1.0 / s + " " + (t3 - t2) * 1.0 / s);
    }

    public static void main(String[] args) {
        demo();
    }
}
