public class Benchmark {
    
    public static void main(String[] args) {
        int N = 10000000;
        ReverseFastSummer refs = new ReverseFastSummer(N);
        FastSummer fs = new FastSummer(N);
        BasicSummer gs = new BasicSummer(new NaiveArray(N));
        SmartSummer ss = new SmartSummer(new NaiveArray(N));
        ReverseSmartSummer ress = new ReverseSmartSummer(new NaiveArray(N));
        SillySummer sis = new SillySummer(new NaiveArray(N));
        FixedSummer ffs = new FixedSummer(N);
        ReverseFixedSummer reffs = new ReverseFixedSummer(N);
        int bogus = 0;
        for(int k = 0; k < 100; ++k) {
            long t0 = System.nanoTime();
            bogus += refs.compute();
            long t1 = System.nanoTime();
            bogus += fs.compute();
            long t2 = System.nanoTime();
            bogus += gs.compute();
            long t3 = System.nanoTime();
            bogus += ss.compute();
            long t4 = System.nanoTime();
            bogus += ress.compute();
            long t5 = System.nanoTime();
            bogus += sis.compute();
            long t6 = System.nanoTime();
            bogus += ffs.compute();
            long t7 = System.nanoTime();
            bogus += reffs.compute();
            long t8 = System.nanoTime();
             System.out.println("refast fast basic smart resmart silly fixed ");
            System.out.println((t1-t0)*1.0/N+" "+(t2-t1)*1.0/N+" "+(t3-t2)*1.0/N+" "+(t4-t3)*1.0/N+" "+(t5-t4)*1.0/N+" "+(t6-t5)*1.0/N+" "+(t7-t6)*1.0/N+" "+(t8-t7)*1.0/N);
        }
    }
}
