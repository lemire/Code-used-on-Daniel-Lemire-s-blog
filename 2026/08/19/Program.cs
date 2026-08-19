using System.Diagnostics;
using System.Net;
using System.Runtime.InteropServices;
using System.Runtime.Intrinsics.X86;

const int N = 10_000;
const int Rounds = 2_000;

Console.WriteLine($".NET {Environment.Version} {RuntimeInformation.OSArchitecture} {RuntimeInformation.OSDescription}");
Console.WriteLine($"Avx512BW.VL: {Avx512BW.VL.IsSupported}  AvxVnni: {AvxVnni.IsSupported}  Bmi2: {Bmi2.IsSupported}");
Console.WriteLine($"Ipv4Parser.IsSupported: {Ipv4Parser.IsSupported}");
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
    bool aOk = Ipv4Parser.TryParse(s, out IPAddress? a);
    bool bOk = IPAddress.TryParse(s, out IPAddress? b);
    if (aOk != bOk)
    {
        Console.WriteLine($"  mismatch ok: '{s}' parser={aOk} bcl={bOk}");
        return 1;
    }
    if (aOk && !a!.Equals(b))
    {
        Console.WriteLine($"  mismatch value: '{s}' parser={a} bcl={b}");
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
            IPAddress.TryParse(inputs[i], out var b);
            warmup += a!.GetHashCode() + b!.GetHashCode();
        }
    }
    GC.KeepAlive(warmup);
    GC.Collect();
    GC.WaitForPendingFinalizers();
    GC.Collect();

    double bclNs = Time(inputs, rounds, IPAddress.TryParse);
    double fastNs = Time(inputs, rounds, Ipv4Parser.TryParse);

    Console.WriteLine($"Benchmark: {n} random IPv4 strings, {rounds} rounds ({(long)n * rounds:N0} parses)");
    Console.WriteLine($"  IPAddress.TryParse : {bclNs,8:F2} ns/op   {1000.0 / bclNs,7:F1} M/s");
    Console.WriteLine($"  Ipv4Parser.TryParse: {fastNs,8:F2} ns/op   {1000.0 / fastNs,7:F1} M/s");
    Console.WriteLine($"  speedup            : {bclNs / fastNs,8:F2}x");
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
