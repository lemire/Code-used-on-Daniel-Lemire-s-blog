public class Benchmark {
    
    public static void main(String[] args) {
        int N = 10000000;
        FastSummer fs = new FastSummer(N);
        BasicSummer gs = new BasicSummer(new NaiveArray(N));
        SmartSummer ss = new SmartSummer(new NaiveArray(N));
        FixedSummer ffs = new FixedSummer(N);
        int bogus = 0;
        for(int k = 0; k < 100; ++k) {
            long t1 = System.nanoTime();
            bogus += fs.compute();
            long t2 = System.nanoTime();
            bogus += gs.compute();
            long t3 = System.nanoTime();
            bogus += ss.compute();
            long t4 = System.nanoTime();
            bogus += ffs.compute();
            long t5 = System.nanoTime();
            System.out.println((t2-t1)*1.0/N+" "+(t3-t2)*1.0/N+" "+(t4-t3)*1.0/N+" "+(t5-t4)*1.0/N);
        }
    }
}
