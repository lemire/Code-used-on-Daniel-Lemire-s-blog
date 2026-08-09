// Adversarial input for .NET's ordinal string.IndexOf.
//
// .NET filters candidate positions on TWO characters, not one: SpanHelpers.IndexOf
// takes ch1 = needle[0], then walks backwards from the end of the needle to find
// ch2, the last character that differs from ch1, at distance d. It then uses SIMD
// to find positions where haystack[i] == ch1 and haystack[i+d-1] == ch2, and only
// there does it run a (vectorized) SequenceEqual over the whole needle.
//
// That second character defeats the textbook adversary. With haystack "a"*n and
// needle "a"*(m-1) + "b", the filter looks for 'a' followed by 'b' at distance m-1;
// 'b' never occurs, so .NET rejects the whole haystack in one linear SIMD pass.
// This is the CONTROL below and it stays flat.
//
// To make .NET quadratic the haystack must be dense in BOTH filter characters
// while still disagreeing with the needle only at the very end. An alternating
// haystack does it:
//
//     haystack = "ab" repeated n/2 times          -> "abababab..."
//     needle   = "ab" repeated (m/2 - 1) times, then "bb"      (m even)
//
//   * ch1 = 'a'. The needle's last character is 'b' != 'a', so d = m: the filter
//     is the first and last character of the needle.
//   * haystack[i] == 'a' exactly when i is even; with m even, haystack[i+m-1] is
//     at an odd index and so equals 'b' exactly when i is even. Both filter
//     characters therefore agree at EVERY even position: half the haystack
//     survives the filter, instead of none.
//   * Each surviving position runs SequenceEqual, which matches the alternating
//     prefix for m-2 characters and fails only at index m-2, where the needle has
//     'b' and the haystack has 'a'.
//   * "bb" never occurs in "abab...", so there is no match and the scan runs to
//     the end.
//
// Result: n/2 verifications of m-2 characters each. Theta(n*m).
//
// Run: dotnet run -c Release
using System;
using System.Diagnostics;
using System.Text;

class Adversarial
{
    static long sink;

    // Nanoseconds per haystack character, best of several repetitions.
    static double Measure(string haystack, string needle, double minSeconds)
    {
        double best = double.MaxValue;
        double spent = 0.0;
        int rounds = 0;
        while (spent < minSeconds || rounds < 3)
        {
            long t0 = Stopwatch.GetTimestamp();
            int r = haystack.IndexOf(needle, StringComparison.Ordinal);
            double elapsed = (double)(Stopwatch.GetTimestamp() - t0) / Stopwatch.Frequency;
            sink += r;
            if (elapsed < best) best = elapsed;
            spent += elapsed;
            rounds++;
            if (rounds > 1000) break;
        }
        return best * 1e9 / haystack.Length;
    }

    static string Repeat(string unit, int times) => new StringBuilder().Insert(0, unit, times).ToString();

    static void Main(string[] args)
    {
        int n = args.Length > 0 ? int.Parse(args[0]) : (1 << 20);

        string altHaystack = Repeat("ab", n / 2);   // "abababab..."
        string flatHaystack = new string('a', n);   // "aaaaaaaa..."

        // Warm up the JIT.
        {
            string w = Repeat("ab", 1 << 15);
            string wn = Repeat("ab", 199) + "bb";
            for (int i = 0; i < 2000; i++) sink += w.IndexOf(wn, StringComparison.Ordinal);
        }

        Console.WriteLine($".NET {Environment.Version} on {System.Runtime.InteropServices.RuntimeInformation.OSArchitecture}, string.IndexOf (Ordinal)");
        Console.WriteLine($"adversary: haystack = 'ab' x {n / 2}, needle = 'ab'x(m/2-1) + 'bb'");
        Console.WriteLine($"control:   haystack = 'a'  x {n},     needle = 'a'x(m-1) + 'b'");
        Console.WriteLine();
        Console.WriteLine($"{"m",8}{"adversary ns/char",20}{"control ns/char",20}");

        for (int m = 8; m <= 4096; m *= 2)
        {
            string adv = Repeat("ab", m / 2 - 1) + "bb";
            string ctl = new string('a', m - 1) + "b";
            double a = Measure(altHaystack, adv, 0.05);
            double c = Measure(flatHaystack, ctl, 0.05);
            Console.WriteLine($"{m,8}{a,20:F3}{c,20:F3}");
        }

        // Does .NET really walk backwards for the last DISTINCT character, or does
        // it just use the literal last character? Discriminating probe: a needle
        // that ENDS in a run of ch1, with the only 'b' buried in the interior.
        //
        //     haystack = "a"*n,  needle = "a"*(m/2) + "b" + "a"*(m/2 - 1)
        //
        // The literal last character is 'a', which matches everywhere; if that were
        // the filter, this would be quadratic. The last character DISTINCT from
        // ch1='a' is the interior 'b', which never occurs in the haystack, so the
        // backward walk makes this linear. Fast here == the backward walk is real.
        Console.WriteLine();
        Console.WriteLine("Probe: needle ending in a run of ch1, only 'b' buried in the interior.");
        Console.WriteLine("Fast => .NET walks back to the last DISTINCT character (paper's claim).");
        Console.WriteLine("Slow => .NET would be using the literal last character.");
        Console.WriteLine();
        Console.WriteLine($"{"m",8}{"interior-b ns/char",22}");
        for (int m = 64; m <= 4096; m *= 4)
        {
            string probe = new string('a', m / 2) + "b" + new string('a', m / 2 - 1);
            Console.WriteLine($"{m,8}{Measure(flatHaystack, probe, 0.05),22:F3}");
        }

        // The sweep above varies m at fixed n, establishing the m factor. Vary n at
        // fixed m to establish the other one: ns/char flat in n means time ~ n, so
        // together the cost is Theta(n*m) rather than, say, Theta(n^2) or Theta(m^2).
        Console.WriteLine();
        Console.WriteLine("Scaling n at fixed m = 1024 (flat ns/char means time is linear in n):");
        Console.WriteLine();
        Console.WriteLine($"{"n",10}{"adversary ns/char",20}");
        string advFixed = Repeat("ab", 511) + "bb";
        for (int k = 16; k <= 22; k++)
        {
            string h = Repeat("ab", (1 << k) / 2);
            Console.WriteLine($"{1 << k,10}{Measure(h, advFixed, 0.05),20:F3}");
        }

        Console.WriteLine();
        Console.WriteLine("The control shows the two-character filter working as intended: it rejects");
        Console.WriteLine("the classic adversary in one linear pass. The adversary shows that feeding");
        Console.WriteLine("both filter characters restores the Theta(n*m) worst case, since .NET has");
        Console.WriteLine("no linear-time fall-back behind the filter.");
        if (sink == 42) Console.WriteLine(sink);
    }
}
