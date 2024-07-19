using System;
using SimdHTML;
using BenchmarkDotNet.Attributes;
using BenchmarkDotNet.Running;
using BenchmarkDotNet.Configs;
using BenchmarkDotNet.Reports;
using BenchmarkDotNet.Filters;
using BenchmarkDotNet.Jobs;
using System.Text;
using System.Runtime;
using System.Runtime.InteropServices;
using System.Buffers;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using BenchmarkDotNet.Columns;
using System.Runtime.Intrinsics;
using System.Runtime.Intrinsics.X86;
using System.Runtime.Intrinsics.Arm;
using System.Runtime.CompilerServices;
using Perfolizer.Horology;


namespace SimdHTMLBenchmarks
{


    public class Speed : IColumn
    {
        public string GetValue(Summary summary, BenchmarkCase benchmarkCase)
        {
            if (summary is null || benchmarkCase is null || benchmarkCase.Parameters is null)
            {
                return "N/A";
            }
            var ourReport = summary.Reports.First(x => x.BenchmarkCase.Equals(benchmarkCase));
            var fileName = (string)benchmarkCase.Parameters["FileName"];
            if (ourReport is null || ourReport.ResultStatistics is null)
            {
                return "N/A";
            }
            long length = new System.IO.FileInfo(fileName).Length;
            var mean = ourReport.ResultStatistics.Mean;
            return $"{(length / ourReport.ResultStatistics.Mean):#####.00}";
        }

        public string GetValue(Summary summary, BenchmarkCase benchmarkCase, SummaryStyle style) => GetValue(summary, benchmarkCase);
        public bool IsDefault(Summary summary, BenchmarkCase benchmarkCase) => false;
        public bool IsAvailable(Summary summary) => true;

        public string Id { get; } = nameof(Speed);
        public string ColumnName { get; } = "Speed (GB/s)";
        public bool AlwaysShow { get; } = true;
        public ColumnCategory Category { get; } = ColumnCategory.Custom;
        public int PriorityInCategory { get; }
        public bool IsNumeric { get; }
        public UnitType UnitType { get; } = UnitType.Dimensionless;
        public string Legend { get; } = "The speed in gigabytes per second";
    }


    [SimpleJob(launchCount: 1, warmupCount: 3, iterationCount: 3)]
    [Config(typeof(Config))]
    public class RealDataBenchmark
    {
        // We only informs the user once about the SIMD support of the system.
        private static bool printed;
#pragma warning disable CA1812
        private sealed class Config : ManualConfig
        {
            public Config()
            {
                AddColumn(new Speed());

                if (RuntimeInformation.ProcessArchitecture == Architecture.Arm64)
                {
                    if (!printed)
                    {
#pragma warning disable CA1303
                        Console.WriteLine("ARM64 system detected.");
                        printed = true;
                    }
                }
                else if (RuntimeInformation.ProcessArchitecture == Architecture.X64)
                {
                    if (Vector512.IsHardwareAccelerated && System.Runtime.Intrinsics.X86.Avx512Vbmi.IsSupported)
                    {
                        if (!printed)
                        {
#pragma warning disable CA1303
                            Console.WriteLine("X64 system detected (Intel, AMD,...) with AVX-512 support.");
                            printed = true;
                        }
                    }
                    else if (Avx2.IsSupported)
                    {
                        if (!printed)
                        {
#pragma warning disable CA1303
                            Console.WriteLine("X64 system detected (Intel, AMD,...) with AVX2 support.");
                            printed = true;
                        }
                    }
                    else if (Ssse3.IsSupported)
                    {
                        if (!printed)
                        {
#pragma warning disable CA1303
                            Console.WriteLine("X64 system detected (Intel, AMD,...) with Sse4.2 support.");
                            printed = true;
                        }
                    }
                    else
                    {
                        if (!printed)
                        {
#pragma warning disable CA1303
                            Console.WriteLine("X64 system detected (Intel, AMD,...) without relevant SIMD support.");
                            printed = true;
                        }
                    }
                }
                AddFilter(new AnyCategoriesFilter(["default"]));

            }
        }
        // Parameters and variables for real data
        [Params(@"data/data.html")]
#pragma warning disable CA1051
        public string? FileName;
        private byte[] allLinesUtf8 = Array.Empty<byte>();


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
        [BenchmarkCategory("default")]

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
        [BenchmarkCategory("default")]

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
        [BenchmarkCategory("default")]

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
        private static readonly SearchValues<byte> searchValues = SearchValues.Create(stackalloc byte[] { 0, 13, 38, 60 });

        [Benchmark]
        [BenchmarkCategory("default")]

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



    public class Program
    {
        static void Main(string[] args)
        {
            if (args.Length == 0)
            {
                args = new string[] { "--filter", "*" };
            }
            var job = Job.Default
                .WithWarmupCount(1)
                .WithMinIterationCount(2)
                .WithMaxIterationCount(10)
                .AsDefault();

            BenchmarkSwitcher.FromAssembly(typeof(Program).Assembly).Run(args, DefaultConfig.Instance.AddJob(job).WithSummaryStyle(SummaryStyle.Default.WithMaxParameterColumnWidth(100)));
        }
    }


    //  }

}
