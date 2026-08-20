using System.Diagnostics;
using System.Net;
using System.Runtime.InteropServices;
using System.Runtime.Intrinsics.X86;

const int N = 10_000;
const int Rounds = 2_000;

Console.WriteLine($".NET {Environment.Version} {RuntimeInformation.OSArchitecture} {RuntimeInformation.OSDescription}");
Console.WriteLine($"Avx512BW.VL: {Avx512BW.VL.IsSupported}  AvxVnni: {AvxVnni.IsSupported}  Bmi2: {Bmi2.IsSupported}");
Console.WriteLine($"Avx512Vbmi.VL: {Avx512Vbmi.VL.IsSupported}  Avx512Vbmi2.VL: {Avx512Vbmi2.VL.IsSupported}");
Console.WriteLine($"Ipv4Parser.IsSupported: {Ipv4Parser.IsSupported}  IsSupportedNoTable: {Ipv4Parser.IsSupportedNoTable}  IsSupportedNoTable2: {Ipv4Parser.IsSupportedNoTable2}  IsSupportedNoTable3: {Ipv4Parser.IsSupportedNoTable3}");
Console.WriteLine();

int failures = RunCorrectnessTests(N);
if (failures != 0)
{
    Console.WriteLine($"FAILED: {failures} mismatch(es)");
    return 1;
}
Console.WriteLine("All correctness tests passed.");
Console.WriteLine();

RunBenchmark(N, Rounds);
Console.WriteLine();
RunKernelBenchmark(N, Rounds);
return 0;

static int RunCorrectnessTests(int n)
{
    int failures = 0;

    failures += Check("0.0.0.0");
    failures += Check("1.2.3.4");
    failures += Check("127.0.0.1");
    failures += Check("192.168.0.1");
    failures += Check("255.255.255.255");
    failures += Check("10.0.0.1");
    failures += Check("8.8.8.8");
    failures += Check("1.11.111.2");
    failures += Check("111.1.11.2");

    // Fast path must reject these; the wrapper should still match IPAddress.TryParse.
    failures += Check("192.168.0.01");
    failures += Check("127.1");
    failures += Check("1.2.3");
    failures += Check("1");
    failures += Check("256.0.0.1");
    failures += Check("1.2.3.4.5");
    failures += Check("");
    failures += Check("not an ip");
    failures += Check("::1");
    failures += Check("2001:db8::1");
    failures += Check(" 192.168.0.1");
    failures += Check("192.168.0.1 ");

    // Every 1..3-digit layout of a strict dotted-quad (81 combinations): the
    // permute is driven by octet lengths, so this is the completeness check.
    int[] vals = { 0, 1, 12, 123 };
    for (int l0 = 1; l0 <= 3; l0++)
    for (int l1 = 1; l1 <= 3; l1++)
    for (int l2 = 1; l2 <= 3; l2++)
    for (int l3 = 1; l3 <= 3; l3++)
    {
        string s = $"{vals[l0]}.{vals[l1]}.{vals[l2]}.{vals[l3]}";
        failures += Check(s);
        if (Ipv4Parser.IsSupportedNoTable && !Ipv4Parser.TryParseAvx512NoTable(s, out _))
        {
            Console.WriteLine($"  no-table fast path declined valid layout '{s}'");
            failures++;
        }
        if (Ipv4Parser.IsSupportedNoTable2 && !Ipv4Parser.TryParseAvx512NoTable2(s, out _))
        {
            Console.WriteLine($"  no-table2 fast path declined valid layout '{s}'");
            failures++;
        }
        if (Ipv4Parser.IsSupportedNoTable3 && !Ipv4Parser.TryParseAvx512NoTable3(s, out _))
        {
            Console.WriteLine($"  no-table3 fast path declined valid layout '{s}'");
            failures++;
        }
        if (Ipv4Parser.IsSupportedNoTable3 && !Ipv4Parser.TryParseAvx512NoTable4(s, out _))
        {
            Console.WriteLine($"  no-table4 fast path declined valid layout '{s}'");
            failures++;
        }
    }

    foreach (string s in new[] { "1.2.3.\u00e9", "1.2.3.\u0100", "\u2024.2.3.4", "1.2.3.4\uff0e", "\u00ff.1.1.1",
                                 "1.2.3.\u0034", "256.1.1.1", "1.256.1.1", "1.1.256.1", "1.1.1.256", "999.0.0.1", "1.2.3.999",
                                 "1.2.3.", ".1.2.3", "1..2.3", "1.2.3.4.", "1234.5.6.7", "1.2.3.4:", "1.2.3.:" })
    {
        failures += Check(s);
    }

    var rng = new Random(42);
    Span<byte> bytes = stackalloc byte[4];
    int fastHits = 0;
    for (int i = 0; i < n; i++)
    {
        rng.NextBytes(bytes);
        var expected = new IPAddress(bytes);
        string s = expected.ToString();
        failures += Check(s);
        if (Ipv4Parser.TryParseAvx512(s, out _))
        {
            fastHits++;
        }
    }

    // Differential fuzz: short random strings over a dot/digit/junk alphabet.
    // This is what actually proves the "exactly three dots" check is live --
    // e.g. "1.2.3.4.5" must not be accepted as 1.2.3.4.
    {
        const string Alphabet = "0123456789..:a 9";
        var fuzz = new Random(2026);
        var sb = new System.Text.StringBuilder(16);
        int fuzzFail = 0;
        for (int i = 0; i < 3_000_000 && fuzzFail < 10; i++)
        {
            sb.Clear();
            int flen = fuzz.Next(0, 16);
            for (int j = 0; j < flen; j++) { sb.Append(Alphabet[fuzz.Next(Alphabet.Length)]); }
            string s = sb.ToString();
            fuzzFail += Check(s);
        }
        failures += fuzzFail;
        Console.WriteLine($"Differential fuzz: 3,000,000 random strings, {fuzzFail} failure(s)");
    }

    Console.WriteLine($"Random dotted-quads: {n}, AVX-512 fast-path hits: {fastHits}");
    if (Ipv4Parser.IsSupported && fastHits != n)
    {
        Console.WriteLine($"WARNING: expected every canonical dotted-quad to hit the fast path, got {fastHits}/{n}");
        failures++;
    }

    return failures;
}

static int Check(string s)
{
    int failures = 0;
    failures += CheckOne("table", s, Ipv4Parser.TryParse);
    failures += CheckOne("table-sat", s, Ipv4Parser.TryParseSat);
    failures += CheckOne("no-table", s, Ipv4Parser.TryParseNoTable);
    failures += CheckOne("no-table2", s, Ipv4Parser.TryParseNoTable2);
    failures += CheckOne("no-table3", s, Ipv4Parser.TryParseNoTable3);
    failures += CheckOne("no-table4", s, Ipv4Parser.TryParseNoTable4);

    // The two fast paths must accept exactly the same set of strings, with the
    // same value -- the table-free path is a drop-in replacement, not a variant.
    bool f1 = Ipv4Parser.TryParseAvx512(s, out uint ip1);
    bool f2 = Ipv4Parser.TryParseAvx512NoTable(s, out uint ip2);
    bool f3 = Ipv4Parser.TryParseAvx512NoTable2(s, out uint ip3);
    if (Ipv4Parser.IsSupported && Ipv4Parser.IsSupportedNoTable && (f1 != f2 || (f1 && ip1 != ip2)))
    {
        Console.WriteLine($"  fast-path divergence: '{s}' table={f1}/{ip1:X8} notable={f2}/{ip2:X8}");
        failures++;
    }
    if (Ipv4Parser.IsSupported && Ipv4Parser.IsSupportedNoTable2 && (f1 != f3 || (f1 && ip1 != ip3)))
    {
        Console.WriteLine($"  fast-path divergence: '{s}' table={f1}/{ip1:X8} notable2={f3}/{ip3:X8}");
        failures++;
    }
    bool f0 = Ipv4Parser.TryParseAvx512Sat(s, out uint ip0);
    if (Ipv4Parser.IsSupported && (f1 != f0 || (f1 && ip1 != ip0)))
    {
        Console.WriteLine($"  fast-path divergence: '{s}' table={f1}/{ip1:X8} table-sat={f0}/{ip0:X8}");
        failures++;
    }
    bool f4 = Ipv4Parser.TryParseAvx512NoTable3(s, out uint ip4);
    bool f5 = Ipv4Parser.TryParseAvx512NoTable4(s, out uint ip5);
    if (Ipv4Parser.IsSupported && Ipv4Parser.IsSupportedNoTable3 && (f1 != f4 || (f1 && ip1 != ip4)))
    {
        Console.WriteLine($"  fast-path divergence: '{s}' table={f1}/{ip1:X8} notable3={f4}/{ip4:X8}");
        failures++;
    }
    if (Ipv4Parser.IsSupported && Ipv4Parser.IsSupportedNoTable3 && (f1 != f5 || (f1 && ip1 != ip5)))
    {
        Console.WriteLine($"  fast-path divergence: '{s}' table={f1}/{ip1:X8} notable4={f5}/{ip5:X8}");
        failures++;
    }
    return failures;
}

static int CheckOne(string name, string s, TryParseFn fn)
{
    bool aOk = fn(s, out IPAddress? a);
    bool bOk = IPAddress.TryParse(s, out IPAddress? b);
    if (aOk != bOk)
    {
        Console.WriteLine($"  [{name}] mismatch ok: '{s}' parser={aOk} bcl={bOk}");
        return 1;
    }
    if (aOk && !a!.Equals(b))
    {
        Console.WriteLine($"  [{name}] mismatch value: '{s}' parser={a} bcl={b}");
        return 1;
    }
    return 0;
}

static void RunBenchmark(int n, int rounds)
{
    var rng = new Random(123);
    var inputs = new string[n];
    Span<byte> bytes = stackalloc byte[4];
    for (int i = 0; i < n; i++)
    {
        rng.NextBytes(bytes);
        inputs[i] = new IPAddress(bytes).ToString();
    }

    // Warm up JIT and tables.
    long warmup = 0;
    for (int w = 0; w < 200; w++)
    {
        for (int i = 0; i < n; i++)
        {
            Ipv4Parser.TryParse(inputs[i], out var a);
            Ipv4Parser.TryParseNoTable(inputs[i], out var c);
            Ipv4Parser.TryParseNoTable2(inputs[i], out var d);
            Ipv4Parser.TryParseNoTable3(inputs[i], out var e);
            Ipv4Parser.TryParseNoTable4(inputs[i], out var f);
            IPAddress.TryParse(inputs[i], out var b);
            warmup += a!.GetHashCode() + b!.GetHashCode() + c!.GetHashCode() + d!.GetHashCode() + e!.GetHashCode() + f!.GetHashCode();
        }
    }
    GC.KeepAlive(warmup);
    GC.Collect();
    GC.WaitForPendingFinalizers();
    GC.Collect();

    Console.WriteLine($"Benchmark: {n} random IPv4 strings, {rounds} rounds ({(long)n * rounds:N0} parses)");

    // Three interleaved passes; report the best of each to blunt frequency drift.
    double bclNs = double.MaxValue, tableNs = double.MaxValue, noTableNs = double.MaxValue, noTable2Ns = double.MaxValue, noTable3Ns = double.MaxValue, noTable4Ns = double.MaxValue;
    for (int pass = 0; pass < 3; pass++)
    {
        bclNs = Math.Min(bclNs, Time(inputs, rounds, IPAddress.TryParse));
        tableNs = Math.Min(tableNs, Time(inputs, rounds, Ipv4Parser.TryParse));
        noTableNs = Math.Min(noTableNs, Time(inputs, rounds, Ipv4Parser.TryParseNoTable));
        noTable2Ns = Math.Min(noTable2Ns, Time(inputs, rounds, Ipv4Parser.TryParseNoTable2));
        noTable3Ns = Math.Min(noTable3Ns, Time(inputs, rounds, Ipv4Parser.TryParseNoTable3));
        noTable4Ns = Math.Min(noTable4Ns, Time(inputs, rounds, Ipv4Parser.TryParseNoTable4));
    }

    Console.WriteLine($"  IPAddress.TryParse        : {bclNs,8:F2} ns/op   {1000.0 / bclNs,7:F1} M/s");
    Console.WriteLine($"  Ipv4Parser.TryParse       : {tableNs,8:F2} ns/op   {1000.0 / tableNs,7:F1} M/s   {bclNs / tableNs,5:F2}x vs BCL");
    Console.WriteLine($"  Ipv4Parser.TryParseNoTable: {noTableNs,8:F2} ns/op   {1000.0 / noTableNs,7:F1} M/s   {bclNs / noTableNs,5:F2}x vs BCL");
    Console.WriteLine($"  Ipv4Parser.TryParseNoTbl2 : {noTable2Ns,8:F2} ns/op   {1000.0 / noTable2Ns,7:F1} M/s   {bclNs / noTable2Ns,5:F2}x vs BCL");
    Console.WriteLine($"  Ipv4Parser.TryParseNoTbl3 : {noTable3Ns,8:F2} ns/op   {1000.0 / noTable3Ns,7:F1} M/s   {bclNs / noTable3Ns,5:F2}x vs BCL");
    Console.WriteLine($"  Ipv4Parser.TryParseNoTbl4 : {noTable4Ns,8:F2} ns/op   {1000.0 / noTable4Ns,7:F1} M/s   {bclNs / noTable4Ns,5:F2}x vs BCL");
}

// Times the SIMD kernels only: no IPAddress allocation, no delegate, so the
// table lookup / table-free permute chain is what actually shows up.
static void RunKernelBenchmark(int n, int rounds)
{
    var rng = new Random(123);
    var inputs = new string[n];
    Span<byte> bytes = stackalloc byte[4];
    for (int i = 0; i < n; i++)
    {
        rng.NextBytes(bytes);
        inputs[i] = new IPAddress(bytes).ToString();
    }

    for (int w = 0; w < 500; w++)
    {
        KernelTable(inputs, 1);
        KernelTableSat(inputs, 1);
        KernelNoTable(inputs, 1);
        KernelNoTable2(inputs, 1);
        KernelNoTable3(inputs, 1);
        KernelNoTable4(inputs, 1);
    }

    const int Passes = 7;
    var tableAll = new double[Passes];
    var tableSatAll = new double[Passes];
    var noTableAll = new double[Passes];
    var noTable2All = new double[Passes];
    var noTable3All = new double[Passes];
    var noTable4All = new double[Passes];
    for (int pass = 0; pass < Passes; pass++)
    {
        long t0 = Stopwatch.GetTimestamp();
        uint s1 = KernelTable(inputs, rounds);
        long t1 = Stopwatch.GetTimestamp();
        uint s1b = KernelTableSat(inputs, rounds);
        long t1b = Stopwatch.GetTimestamp();
        uint s2 = KernelNoTable(inputs, rounds);
        long t2 = Stopwatch.GetTimestamp();
        uint s3 = KernelNoTable2(inputs, rounds);
        long t3 = Stopwatch.GetTimestamp();
        uint s4 = KernelNoTable3(inputs, rounds);
        long t4 = Stopwatch.GetTimestamp();
        uint s5 = KernelNoTable4(inputs, rounds);
        long t5 = Stopwatch.GetTimestamp();
        if (s1 != s1b || s1 != s2 || s1 != s3 || s1 != s4 || s1 != s5) { Console.WriteLine("  kernel checksum divergence"); }
        double per = 1e9 / (Stopwatch.Frequency * (double)rounds * n);
        tableAll[pass] = (t1 - t0) * per;
        tableSatAll[pass] = (t1b - t1) * per;
        noTableAll[pass] = (t2 - t1b) * per;
        noTable2All[pass] = (t3 - t2) * per;
        noTable3All[pass] = (t4 - t3) * per;
        noTable4All[pass] = (t5 - t4) * per;
    }
    Array.Sort(tableAll);
    Array.Sort(tableSatAll);
    Array.Sort(noTableAll);
    Array.Sort(noTable2All);
    Array.Sort(noTable3All);
    Array.Sort(noTable4All);

    Console.WriteLine($"Kernel only (uint out, no allocation), {n} strings, {rounds} rounds, {Passes} passes");
    Console.WriteLine($"  TryParseAvx512       : min {tableAll[0],6:F2}  med {tableAll[Passes / 2],6:F2} ns/op   {1000.0 / tableAll[0],7:F1} M/s");
    Console.WriteLine($"  ...Avx512Sat (table)  : min {tableSatAll[0],6:F2}  med {tableSatAll[Passes / 2],6:F2} ns/op   {1000.0 / tableSatAll[0],7:F1} M/s");
    Console.WriteLine($"  TryParseAvx512NoTable: min {noTableAll[0],6:F2}  med {noTableAll[Passes / 2],6:F2} ns/op   {1000.0 / noTableAll[0],7:F1} M/s");
    Console.WriteLine($"  ...NoTable2 (vpshufb): min {noTable2All[0],6:F2}  med {noTable2All[Passes / 2],6:F2} ns/op   {1000.0 / noTable2All[0],7:F1} M/s");
    Console.WriteLine($"  ...NoTable3 (mask dom): min {noTable3All[0],6:F2}  med {noTable3All[Passes / 2],6:F2} ns/op   {1000.0 / noTable3All[0],7:F1} M/s");
    Console.WriteLine($"  ...NoTable4 (+saturate): min {noTable4All[0],6:F2}  med {noTable4All[Passes / 2],6:F2} ns/op   {1000.0 / noTable4All[0],7:F1} M/s");
    Console.WriteLine($"  no-table  vs table   : {tableAll[0] / noTableAll[0],8:F2}x (min)");
    Console.WriteLine($"  no-table2 vs table   : {tableAll[0] / noTable2All[0],8:F2}x (min)");
    Console.WriteLine($"  no-table3 vs table   : {tableAll[0] / noTable3All[0],8:F2}x (min)");
    Console.WriteLine($"  no-table4 vs table   : {tableAll[0] / noTable4All[0],8:F2}x (min)");
    Console.WriteLine($"  no-table4 vs table-sat:{tableSatAll[0] / noTable4All[0],8:F2}x (min)  <- like for like");
}

static uint KernelTable(string[] inputs, int rounds)
{
    uint sum = 0;
    for (int r = 0; r < rounds; r++)
    {
        for (int i = 0; i < inputs.Length; i++)
        {
            Ipv4Parser.TryParseAvx512(inputs[i], out uint ip);
            sum += ip;
        }
    }
    return sum;
}

static uint KernelNoTable(string[] inputs, int rounds)
{
    uint sum = 0;
    for (int r = 0; r < rounds; r++)
    {
        for (int i = 0; i < inputs.Length; i++)
        {
            Ipv4Parser.TryParseAvx512NoTable(inputs[i], out uint ip);
            sum += ip;
        }
    }
    return sum;
}

static uint KernelTableSat(string[] inputs, int rounds)
{
    uint sum = 0;
    for (int r = 0; r < rounds; r++)
    {
        for (int i = 0; i < inputs.Length; i++)
        {
            Ipv4Parser.TryParseAvx512Sat(inputs[i], out uint ip);
            sum += ip;
        }
    }
    return sum;
}

static uint KernelNoTable3(string[] inputs, int rounds)
{
    uint sum = 0;
    for (int r = 0; r < rounds; r++)
    {
        for (int i = 0; i < inputs.Length; i++)
        {
            Ipv4Parser.TryParseAvx512NoTable3(inputs[i], out uint ip);
            sum += ip;
        }
    }
    return sum;
}

static uint KernelNoTable4(string[] inputs, int rounds)
{
    uint sum = 0;
    for (int r = 0; r < rounds; r++)
    {
        for (int i = 0; i < inputs.Length; i++)
        {
            Ipv4Parser.TryParseAvx512NoTable4(inputs[i], out uint ip);
            sum += ip;
        }
    }
    return sum;
}

static uint KernelNoTable2(string[] inputs, int rounds)
{
    uint sum = 0;
    for (int r = 0; r < rounds; r++)
    {
        for (int i = 0; i < inputs.Length; i++)
        {
            Ipv4Parser.TryParseAvx512NoTable2(inputs[i], out uint ip);
            sum += ip;
        }
    }
    return sum;
}

static double Time(string[] inputs, int rounds, TryParseFn fn)
{
    long hash = 0;
    long t0 = Stopwatch.GetTimestamp();
    for (int r = 0; r < rounds; r++)
    {
        for (int i = 0; i < inputs.Length; i++)
        {
            fn(inputs[i], out IPAddress? addr);
            hash += addr!.GetHashCode();
        }
    }
    double sec = (Stopwatch.GetTimestamp() - t0) / (double)Stopwatch.Frequency;
    GC.KeepAlive(hash);
    return sec * 1e9 / ((long)rounds * inputs.Length);
}

delegate bool TryParseFn(string s, out IPAddress? address);
