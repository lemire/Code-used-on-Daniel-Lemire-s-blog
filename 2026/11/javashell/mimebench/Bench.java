import java.nio.file.*;
import java.util.Base64;

public class Bench {
    static volatile Object sink;
    public static void main(String[] args) throws Exception {
        String[] exp = Files.readString(Path.of("expected.txt")).trim().split(" ");
        int expLen = Integer.parseInt(exp[0]); long expSum = Long.parseLong(exp[1]);
        run("mime", Files.readAllBytes(Path.of("mime.txt")), Base64.getMimeDecoder(), expLen, expSum);
    }
    static void run(String name, byte[] in, Base64.Decoder dec, int expLen, long expSum) {
        byte[] out = dec.decode(in);
        long s = 0; for (byte b : out) s += b & 0xff;
        if (out.length != expLen || s != expSum) throw new RuntimeException("bad decode " + name);
        int iters = 200;
        for (int w = 0; w < 30; w++) for (int i = 0; i < iters; i++) sink = dec.decode(in); // warm-up (JIT)
        double best = Double.MAX_VALUE;
        for (int t = 0; t < 10; t++) {
            long st = System.nanoTime();
            for (int i = 0; i < iters; i++) sink = dec.decode(in);
            best = Math.min(best, (System.nanoTime() - st) / (double) iters);
        }
        System.out.printf(java.util.Locale.ROOT, "java %s %s Base64.decode(byte[]): %.2f GB/s%n", Runtime.version(), name, in.length / best);
    }
}
