using BenchmarkDotNet.Attributes;
using BenchmarkDotNet.Columns;
using BenchmarkDotNet.Reports;
using BenchmarkDotNet.Running;

namespace Utils.Benchmark;

public class SpeedColumnAttribute : ColumnConfigBaseAttribute
{
    public SpeedColumnAttribute() : base(new Speed()) { } 
}

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
        var length = new FileInfo(fileName).Length;
        var mean = ourReport.ResultStatistics.Mean;
        return $"{length / ourReport.ResultStatistics.Mean:#####.00}";
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