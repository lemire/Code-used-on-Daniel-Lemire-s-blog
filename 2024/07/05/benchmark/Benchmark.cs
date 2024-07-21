using SimdHTML;
using BenchmarkDotNet.Attributes;
using BenchmarkDotNet.Running;
using BenchmarkDotNet.Configs;
using BenchmarkDotNet.Reports;
using Utils.Benchmark;
using System.Buffers;
using System.Runtime.Intrinsics.Arm;

if (args.Length == 0)
{
    args = ["--filter", "*"];
}

BenchmarkSwitcher
    .FromAssembly(typeof(Program).Assembly)
    .Run(args, DefaultConfig.Instance.WithSummaryStyle(SummaryStyle.Default.WithMaxParameterColumnWidth(100)));

[SpeedColumn]
[SimpleJob(launchCount: 1, warmupCount: 3, iterationCount: 6)]
public class RealDataBenchmark
{
    // Parameters and variables for real data
    [Params(@"data/data.html")]
#pragma warning disable CA1051
    public string? FileName;
    byte[] allLinesUtf8 = [];

    public unsafe delegate void HTMLScanFunction(ref byte* pUtf8, byte* end);

    private void RunHTMLScanBenchmark(byte[] data, HTMLScanFunction HTMLScanFunction)
    {
        unsafe
        {
            fixed (byte* pUtf8 = data)
            {
                byte* start = pUtf8;
                byte* end = pUtf8 + data.Length;
                while (start != end)
                {
                    HTMLScanFunction(ref start, end);
                    if (start == end)
                    {
                        break;
                    }
                    start++;
                }
            }
        }
    }

    [GlobalSetup]
    public void Setup()
    {
        allLinesUtf8 = FileName == null ? allLinesUtf8 : File.ReadAllBytes(FileName);
    }

    [Benchmark]
    public unsafe int SIMDHTMLScan()
    {
        int count = 0;
        if (allLinesUtf8 != null)
        {
            unsafe
            {
                fixed (byte* pUtf8 = allLinesUtf8)
                {
                    byte* start = pUtf8;
                    byte* end = pUtf8 + allLinesUtf8.Length;
                    while (start != end)
                    {
                        SimdHTML.FastScan.SIMDAdvanceString(ref start, end);
                        if (start == end)
                        {
                            break;
                        }
                        count += *start;
                        start++;
                    }
                }
            }
        }
        return count;
    }

    [Benchmark]
    public unsafe int NEONHTMLScan()
    {
        int count = 0;
        if (allLinesUtf8 != null && AdvSimd.Arm64.IsSupported)
        {

            unsafe
            {
                fixed (byte* pUtf8 = allLinesUtf8)
                {
                    byte* start = pUtf8;
                    byte* end = pUtf8 + allLinesUtf8.Length;
                    SimdHTML.NeonMatch match = new SimdHTML.NeonMatch(start, end);
                    while (match.Advance())
                    {
                        count += *match.Get();
                        match.Consume();
                    }

                }
            }
        }
        return count;
    }

    [Benchmark]
    public unsafe int NEON64HTMLScan()
    {
        int count = 0;
        if (allLinesUtf8 != null && AdvSimd.Arm64.IsSupported)
        {
            fixed (byte* pUtf8 = allLinesUtf8)
            {
                var start = pUtf8;
                var end = pUtf8 + allLinesUtf8.Length;
                var scanner = new Neon64Scanner(start, end);
                foreach (var match in scanner)
                {
                    count += *match;
                }
            }
        }
        return count;
    }

    [Benchmark]
    public unsafe int HTMLScan()
    {
        int count = 0;

        if (allLinesUtf8 != null)
        {
            unsafe
            {
                fixed (byte* pUtf8 = allLinesUtf8)
                {
                    byte* start = pUtf8;
                    byte* end = pUtf8 + allLinesUtf8.Length;
                    while (start != end)
                    {
                        SimdHTML.FastScan.NaiveAdvanceString(ref start, end);
                        if (start == end)
                        {
                            break;
                        }
                        count += *start;
                        start++;
                    }
                }
            }
        }
        return count;
    }

    static readonly SearchValues<byte> searchValues = SearchValues.Create([0, 13, 38, 60]);

    [Benchmark]
    public unsafe int SearchValuesBench()
    {
        int count = 0;
        if (allLinesUtf8 != null)
        {
            unsafe
            {
                ReadOnlySpan<byte> data = allLinesUtf8;
                while (!data.IsEmpty)
                {
                    int first = data.IndexOfAny(searchValues);
                    data = data.Slice(first >= 0 ? first + 1 : 1);
                    if (first >= 0) count += data[0];
                }

            }
        }
        return count;
    }

}
