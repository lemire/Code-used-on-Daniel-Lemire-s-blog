using System;
using System.Buffers;
using System.Buffers.Text;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
System.Globalization.CultureInfo.DefaultThreadCurrentCulture = System.Globalization.CultureInfo.InvariantCulture;
System.Globalization.CultureInfo.CurrentCulture = System.Globalization.CultureInfo.InvariantCulture;

var exp = File.ReadAllText("expected.txt").Trim().Split(' ');
int expLen = int.Parse(exp[0]); long expSum = long.Parse(exp[1]);
string ver = Environment.Version.ToString();
foreach (var name in new[] { "mime" }) {
    byte[] bytes = File.ReadAllBytes(name + ".txt");
    string str = Encoding.ASCII.GetString(bytes);
    byte[] dst = new byte[expLen + 64];
    Run(name, "Convert.FromBase64String(string)", str.Length, () => Convert.FromBase64String(str));
    Run(name, "Base64.DecodeFromUtf8(bytes)", bytes.Length, () => {
        var st = Base64.DecodeFromUtf8(bytes, dst, out _, out int written);
        if (st != OperationStatus.Done) throw new Exception("status " + st);
        return dst.AsSpan(0, written).ToArray();
    }, () => { Base64.DecodeFromUtf8(bytes, dst, out _, out int w); return w; });
}

void Run(string name, string label, int inLen, Func<byte[]> f, Func<int> fast = null) {
    var o = f();
    long s = 0; foreach (var b in o) s += b;
    if (o.Length != expLen || s != expSum) throw new Exception("bad decode " + name + " " + label);
    Func<object> g = fast != null ? () => fast() : () => f();
    int iters = 200; object sink = null;
    for (int w = 0; w < 30; w++) for (int i = 0; i < iters; i++) sink = g();
    double best = double.MaxValue;
    for (int t = 0; t < 10; t++) {
        long st = Stopwatch.GetTimestamp();
        for (int i = 0; i < iters; i++) sink = g();
        best = Math.Min(best, Stopwatch.GetElapsedTime(st).TotalNanoseconds / iters);
    }
    GC.KeepAlive(sink);
    Console.WriteLine($".NET {ver} {name} {label}: {inLen / best:F2} GB/s");
}
