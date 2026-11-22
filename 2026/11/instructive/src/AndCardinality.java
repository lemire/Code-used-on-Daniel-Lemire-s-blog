import instructive.agent.InstructionProfileFormatter;
import instructive.agent.Sampler;
import java.util.Random;
import org.roaringbitmap.RoaringBitmap;

public class AndCardinality {
    // a bitmap of 64 chunks of 65536 values, each value present with probability 1/2
    static RoaringBitmap dense(long seed) {
        Random r = new Random(seed);
        RoaringBitmap b = new RoaringBitmap();
        for (int i = 0; i < 64 * 65536; i++) {
            if (r.nextBoolean()) {
                b.add(i);
            }
        }
        return b;
    }

    public static void main(String[] args) {
        RoaringBitmap a = dense(1), b = dense(2);
        long sink = 0;
        for (int i = 0; i < 10_000; i++) {
            sink += RoaringBitmap.andCardinality(a, b); // warm up, so that the code is JIT compiled
        }
        int runs = 100_000;
        Sampler.start();
        long start = System.nanoTime();
        for (int i = 0; i < runs; i++) {
            sink += RoaringBitmap.andCardinality(a, b);
        }
        long elapsed = System.nanoTime() - start;
        Sampler.stop();
        System.out.printf("%.2f us per call, %.3f ns per word%n", elapsed / 1e3 / runs, elapsed / (double) runs / (64 * 1024));
        InstructionProfileFormatter formatter = new InstructionProfileFormatter();
        Sampler.forEachHottestMethod(1, profile -> formatter.format(profile.snapshot(), System.out));
        System.out.println(sink);
    }
}
