// AVX-512-VL IPv4 parser, ported from
// https://github.com/lemire/simdip/blob/main/include/avx512ip.h
//
// The SIMD path is strict dotted-decimal (four octets of 1..3 digits, no
// leading zeros). Anything it rejects is handed to IPAddress.TryParse.

using System.Diagnostics.CodeAnalysis;
using System.Numerics;
using System.Net;
using System.Runtime.CompilerServices;
using System.Runtime.Intrinsics;
using System.Runtime.Intrinsics.X86;

public static class Ipv4Parser
{
    private const uint HashMul = 0x00CF7800u;

    // Per-key 16-byte shuffle: octet i occupies bytes [4i..4i+3] as
    // [0x80 (->0), hundreds, tens, ones].
    private static readonly byte[] Pat = new byte[256 * 16];
    // Low 16 bits: canonical delimiter partition; high 16 bits: leading-digit lanes.
    private static readonly uint[] Aux = new uint[256];

    private static readonly Vector256<ushort> CharLaneIndex = Vector256.Create((ushort)0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
    private static readonly Vector128<byte> PackShuffle = Vector128.Create((byte)0, 4, 8, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    private static readonly Vector128<sbyte> Weights = Vector128.Create(unchecked((int)0x010A6400)).AsSByte();

    // Table-free path: constants only, no lookup tables.
    private static readonly Vector128<byte> Iota = Vector128.Create((byte)0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
    // Broadcast the i-th compressed marker position to the four bytes of octet i.
    private static readonly Vector128<byte> RepeatOctet = Vector128.Create((byte)0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3);
    // Per-octet byte offsets from the marker that terminates the octet.
    private static readonly Vector128<sbyte> OctetOffsets = Vector128.Create((sbyte)-4, -3, -2, -1, -4, -3, -2, -1, -4, -3, -2, -1, -4, -3, -2, -1);

    static Ipv4Parser()
    {
        for (int k = 0; k < 256; k++)
        {
            for (int b = 0; b < 16; b++)
            {
                Pat[k * 16 + b] = 0x80;
            }
        }

        for (int l0 = 1; l0 <= 3; l0++)
        for (int l1 = 1; l1 <= 3; l1++)
        for (int l2 = 1; l2 <= 3; l2++)
        for (int l3 = 1; l3 <= 3; l3++)
        {
            int s1 = l0 + 1;
            int s2 = l0 + l1 + 2;
            int s3 = l0 + l1 + l2 + 3;
            int len = s3 + l3;
            uint partition = (1u << l0) |
                             (1u << (l0 + l1 + 1)) |
                             (1u << (l0 + l1 + l2 + 2)) |
                             (1u << len);
            uint hk = (partition * HashMul) >> 24;
            if (Aux[hk] != 0)
            {
                throw new InvalidOperationException("avx512vl ipv4: perfect-hash collision");
            }

            int[] starts = { 0, s1, s2, s3 };
            int[] lens = { l0, l1, l2, l3 };
            ushort lead = 0;
            for (int oc = 0; oc < 4; oc++)
            {
                int baseOff = 4 * oc;
                int s = starts[oc];
                int l = lens[oc];
                Pat[hk * 16 + baseOff + 3] = (byte)(s + l - 1);
                if (l >= 2)
                {
                    Pat[hk * 16 + baseOff + 2] = (byte)(s + l - 2);
                }
                if (l == 3)
                {
                    Pat[hk * 16 + baseOff + 1] = (byte)s;
                    lead |= (ushort)(1u << (baseOff + 1));
                }
                else if (l == 2)
                {
                    lead |= (ushort)(1u << (baseOff + 2));
                }
            }
            Aux[hk] = partition | ((uint)lead << 16);
        }
    }

    public static bool IsSupported => Avx512BW.VL.IsSupported && Ssse3.IsSupported;

    public static bool TryParse(string? s, [NotNullWhen(true)] out IPAddress? address)
    {
        if (s is null)
        {
            address = null;
            return false;
        }
        return TryParse(s.AsSpan(), out address);
    }

    public static bool TryParse(ReadOnlySpan<char> s, [NotNullWhen(true)] out IPAddress? address)
    {
        if (IsSupported && TryParseAvx512(s, out uint ip))
        {
            address = new IPAddress(ip);
            return true;
        }
        return IPAddress.TryParse(s, out address);
    }

    /// <summary>
    /// Strict AVX-512 dotted-quad parse. Returns false for anything the SIMD
    /// path does not accept (including valid-but-unusual forms).
    /// </summary>
    public static unsafe bool TryParseAvx512(ReadOnlySpan<char> s, out uint ip)
    {
        ip = 0;
        int len = s.Length;
        if ((uint)len > 15u || len == 0)
        {
            return false;
        }

        fixed (char* cp = s)
        {
            Vector256<ushort> charMask = Vector256.LessThan(CharLaneIndex, Vector256.Create((ushort)len));
            Vector256<ushort> chars = Avx512BW.VL.MaskLoad((ushort*)cp, charMask, Vector256.Create((ushort)'0'));
            if (Avx512BW.VL.CompareGreaterThan(chars, Vector256.Create((ushort)0x7F)).ExtractMostSignificantBits() != 0)
            {
                return false;
            }

            Vector128<byte> str = Avx512BW.VL.ConvertToVector128Byte(chars);
            return TryParseAvx512Loaded(str, (uint)len, out ip);
        }
    }

    private static unsafe bool TryParseAvx512Loaded(Vector128<byte> str, uint len, out uint ip)
    {
        ip = 0;
        uint lenMask = Bmi2.IsSupported
            ? Bmi2.ZeroHighBits(0xFFFFFFFFu, len)
            : (1u << (int)len) - 1u;

        Vector128<byte> zeroDigit = Vector128.Create((byte)'0');
        Vector128<byte> dotsVec = Avx512BW.VL.CompareEqual(str, Vector128.Create((byte)'.'));
        uint dots = dotsVec.ExtractMostSignificantBits();

        uint partition = dots | (lenMask + 1u);
        uint hashKey = (partition * HashMul) >> 24;
        uint aux = Aux[hashKey];
        int error = partition != (aux & 0xFFFFu) ? 1 : 0;

        Vector128<byte> digits = str - zeroDigit;
        Vector128<byte> bad = Avx512BW.VL.CompareGreaterThan(digits, Vector128.Create((byte)9));
        uint hole = (~dots) & bad.ExtractMostSignificantBits();

        Vector128<byte> shuf = Vector128.LoadUnsafe(ref Pat[hashKey * 16]);
        Vector128<byte> padded = Ssse3.Shuffle(digits, shuf);

        Vector128<int> res = Dpbusd(padded);
        uint over = Avx512F.VL.CompareGreaterThan(res.AsUInt32(), Vector128.Create(0xFFu)).ExtractMostSignificantBits();
        error |= (hole | over) != 0 ? 1 : 0;

        uint lz = Vector128.Equals(padded, Vector128<byte>.Zero).ExtractMostSignificantBits();
        error |= (lz & (aux >> 16)) != 0 ? 1 : 0;

        if (error != 0)
        {
            return false;
        }

        Vector128<byte> packed = Ssse3.Shuffle(res.AsByte(), PackShuffle);
        ip = packed.AsUInt32().ToScalar();
        return true;
    }

    /// <summary>
    /// True when the table-free path can run (needs VBMI for vpermb and VBMI2 for vpcompressb).
    /// </summary>
    public static bool IsSupportedNoTable =>
        Avx512Vbmi.VL.IsSupported && Avx512Vbmi2.VL.IsSupported && Avx512BW.VL.IsSupported && Sse41.IsSupported;

    public static bool TryParseNoTable(string? s, [NotNullWhen(true)] out IPAddress? address)
    {
        if (s is null)
        {
            address = null;
            return false;
        }
        return TryParseNoTable(s.AsSpan(), out address);
    }

    public static bool TryParseNoTable(ReadOnlySpan<char> s, [NotNullWhen(true)] out IPAddress? address)
    {
        if (IsSupportedNoTable && TryParseAvx512NoTable(s, out uint ip))
        {
            address = new IPAddress(ip);
            return true;
        }
        return IPAddress.TryParse(s, out address);
    }

    /// <summary>
    /// Strict AVX-512 dotted-quad parse with no shuffle table. Digit placement is
    /// computed from the delimiter positions: vpcompressb of [0..15] by the marker
    /// mask (three dots plus a terminator at len) yields the four marker positions
    /// (q0,q1,q2,q3); broadcasting qi, adding [-4,-3,-2,-1] and a signed max with
    /// q(i-1) builds a vpermb index that points digit bytes at their source and pad
    /// bytes at a byte known to be zero.
    /// </summary>
    public static unsafe bool TryParseAvx512NoTable(ReadOnlySpan<char> s, out uint ip)
    {
        ip = 0;
        int len = s.Length;
        if ((uint)len > 15u || len == 0)
        {
            return false;
        }

        fixed (char* cp = s)
        {
            Vector256<ushort> charMask = Vector256.LessThan(CharLaneIndex, Vector256.Create((ushort)len));
            // Out-of-range lanes read as 0, which is neither a digit nor a dot.
            Vector256<ushort> chars = Avx512BW.VL.MaskLoad((ushort*)cp, charMask, Vector256<ushort>.Zero);
            if (Avx512BW.VL.CompareGreaterThan(chars, Vector256.Create((ushort)0x7F)).ExtractMostSignificantBits() != 0)
            {
                return false;
            }

            Vector128<byte> v = Avx512BW.VL.ConvertToVector128Byte(chars);
            return TryParseAvx512NoTableLoaded(v, (uint)len, out ip);
        }
    }

    private static bool TryParseAvx512NoTableLoaded(Vector128<byte> v, uint len, out uint ip)
    {
        ip = 0;
        uint lenMask = Bmi2.IsSupported
            ? Bmi2.ZeroHighBits(0xFFFFFFFFu, len)
            : (1u << (int)len) - 1u;

        Vector128<byte> zeroDigit = Vector128.Create((byte)'0');
        Vector128<byte> dotsVec = Avx512BW.VL.CompareEqual(v, Vector128.Create((byte)'.'));
        uint dots = dotsVec.ExtractMostSignificantBits();
        uint keep = lenMask & ~dots;  // in-range lanes that are not dots

        // Digit values; every non-digit (dots, junk, the zeroed tail) maps above 9.
        Vector128<byte> digits = v - zeroDigit;
        Vector128<byte> notDigit = Avx512BW.VL.CompareGreaterThan(digits, Vector128.Create((byte)9));
        // Junk in a digit slot, e.g. "1.2.3.:".
        uint hole = notDigit.ExtractMostSignificantBits() & keep;
        // Zero every non-digit lane: pad fetches of a dot (or of the tail) return 0.
        Vector128<byte> v0 = Sse2.AndNot(notDigit, digits);

        // Markers: the three dots plus a virtual terminator at `len`.
        Vector128<byte> delim = dotsVec | Avx512BW.VL.CompareEqual(Iota, Vector128.Create((byte)len));
        // Bytes 0..3 of c are (q0,q1,q2,q3): compress does the rank scan.
        Vector128<byte> c = Avx512Vbmi2.VL.Compress(Vector128<byte>.Zero, delim, Iota);

        // qi broadcast to octet i's four lanes; prev is q(i-1), with -1 for octet 0.
        Vector128<sbyte> qi = Ssse3.Shuffle(c, RepeatOctet).AsSByte();
        Vector128<sbyte> prev = Ssse3.Shuffle(
            Ssse3.AlignRight(c, Vector128.Create((byte)0xFF), 15), RepeatOctet).AsSByte();
        // Digits keep their index (>= prev+1); pads clamp onto the previous dot, and
        // octet 0's pads clamp to -1 == 15 (mod 16), the zeroed tail byte.
        Vector128<sbyte> idx = Sse41.Max(Sse2.Add(qi, OctetOffsets), prev);
        // One permute expands each octet to [0, hundreds, tens, ones].
        Vector128<byte> padded = Avx512Vbmi.VL.PermuteVar16x8(v0, idx.AsByte());

        Vector128<int> res = Dpbusd(padded);
        uint over = Avx512F.VL.CompareGreaterThan(res.AsUInt32(), Vector128.Create(0xFFu)).ExtractMostSignificantBits();

        // Layout, all in general registers: exactly three dots, no empty octet
        // (leading / trailing / consecutive dots), no four-digit octet.
        uint error = (uint)(BitOperations.PopCount(dots) ^ 3);
        error |= dots & 1u;
        error |= dots & (dots << 1);
        error |= dots & (lenMask ^ (lenMask >> 1));   // trailing dot
        error |= keep & (keep << 1) & (keep << 2) & (keep << 3);

        // Leading zero: a '0' starting an octet whose next character is a digit.
        // Bare "0" is fine -- the next character is a dot or the end of the string.
        uint zeroBits = Avx512BW.VL.CompareEqual(v, zeroDigit).ExtractMostSignificantBits();
        error |= zeroBits & ((dots << 1) | 1u) & (keep >> 1);
        error |= hole | over;

        if (error != 0)
        {
            return false;
        }

        Vector128<byte> packed = Ssse3.Shuffle(res.AsByte(), PackShuffle);
        ip = packed.AsUInt32().ToScalar();
        return true;
    }

    /// <summary>
    /// Table-free path, second cut. Needs VBMI2 (vpcompressb) but not VBMI.
    /// </summary>
    public static bool IsSupportedNoTable2 =>
        Avx512Vbmi2.VL.IsSupported && Avx512BW.VL.IsSupported && Sse41.IsSupported;

    public static bool TryParseNoTable2(string? s, [NotNullWhen(true)] out IPAddress? address)
    {
        if (s is null)
        {
            address = null;
            return false;
        }
        return TryParseNoTable2(s.AsSpan(), out address);
    }

    public static bool TryParseNoTable2(ReadOnlySpan<char> s, [NotNullWhen(true)] out IPAddress? address)
    {
        if (IsSupportedNoTable2 && TryParseAvx512NoTable2(s, out uint ip))
        {
            address = new IPAddress(ip);
            return true;
        }
        return IPAddress.TryParse(s, out address);
    }

    /// <summary>
    /// Table-free, with two changes over <see cref="TryParseAvx512NoTable"/>:
    ///
    ///  - The tail of the masked load is filled with '.' rather than 0, so the
    ///    terminator at `len` is just another dot. The marker mask is then the
    ///    dot compare itself -- the JIT leaves it in a k register, dropping the
    ///    extra compare, the or, and the vpmovb2m from the critical path. Dots
    ///    past `len` compress to bytes 4.., which nothing reads.
    ///  - vpshufb replaces vpermb. A vpshufb control byte with bit 7 set yields
    ///    zero, so octet 0's pads (which clamp to -1) zero themselves instead of
    ///    wrapping to byte 15 -- one cycle instead of three, and no VBMI.
    /// </summary>
    public static unsafe bool TryParseAvx512NoTable2(ReadOnlySpan<char> s, out uint ip)
    {
        ip = 0;
        int len = s.Length;
        if ((uint)len > 15u || len == 0)
        {
            return false;
        }

        fixed (char* cp = s)
        {
            Vector256<ushort> charMask = Vector256.LessThan(CharLaneIndex, Vector256.Create((ushort)len));
            Vector256<ushort> chars = Avx512BW.VL.MaskLoad((ushort*)cp, charMask, Vector256.Create((ushort)'.'));
            if (Avx512BW.VL.CompareGreaterThan(chars, Vector256.Create((ushort)0x7F)).ExtractMostSignificantBits() != 0)
            {
                return false;
            }

            Vector128<byte> v = Avx512BW.VL.ConvertToVector128Byte(chars);
            return TryParseAvx512NoTable2Loaded(v, (uint)len, out ip);
        }
    }

    private static bool TryParseAvx512NoTable2Loaded(Vector128<byte> v, uint len, out uint ip)
    {
        ip = 0;
        uint lenMask = Bmi2.IsSupported
            ? Bmi2.ZeroHighBits(0xFFFFFFFFu, len)
            : (1u << (int)len) - 1u;

        // Markers: real dots plus the '.'-filled tail. The first four are the
        // three dots and the terminator at `len`.
        Vector128<byte> markers = Avx512BW.VL.CompareEqual(v, Vector128.Create((byte)'.'));
        Vector128<byte> c = Avx512Vbmi2.VL.Compress(Vector128<byte>.Zero, markers, Iota);

        Vector128<sbyte> qi = Ssse3.Shuffle(c, RepeatOctet).AsSByte();
        Vector128<sbyte> prev = Ssse3.Shuffle(
            Ssse3.AlignRight(c, Vector128.Create((byte)0xFF), 15), RepeatOctet).AsSByte();
        Vector128<sbyte> idx = Sse41.Max(Sse2.Add(qi, OctetOffsets), prev);

        Vector128<byte> zeroDigit = Vector128.Create((byte)'0');
        Vector128<byte> digits = v - zeroDigit;
        Vector128<byte> notDigit = Avx512BW.VL.CompareGreaterThan(digits, Vector128.Create((byte)9));
        Vector128<byte> v0 = Sse2.AndNot(notDigit, digits);
        // Pads inside octets 1..3 clamp onto the previous dot (zero in v0);
        // octet 0's pads carry bit 7, which vpshufb turns into zero by itself.
        Vector128<byte> padded = Ssse3.Shuffle(v0, idx.AsByte());

        Vector128<int> res = Dpbusd(padded);
        uint over = Avx512F.VL.CompareGreaterThan(res.AsUInt32(), Vector128.Create(0xFFu)).ExtractMostSignificantBits();

        uint dots = markers.ExtractMostSignificantBits() & lenMask;
        uint keep = lenMask & ~dots;
        uint error = (uint)(BitOperations.PopCount(dots) ^ 3);
        error |= dots & 1u;
        error |= dots & (dots << 1);
        error |= dots & (lenMask ^ (lenMask >> 1));   // trailing dot
        error |= keep & (keep << 1) & (keep << 2) & (keep << 3);
        error |= notDigit.ExtractMostSignificantBits() & keep;

        uint zeroBits = Avx512BW.VL.CompareEqual(v, zeroDigit).ExtractMostSignificantBits();
        error |= zeroBits & ((dots << 1) | 1u) & (keep >> 1);
        error |= over;

        if (error != 0)
        {
            return false;
        }

        Vector128<byte> packed = Ssse3.Shuffle(res.AsByte(), PackShuffle);
        ip = packed.AsUInt32().ToScalar();
        return true;
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    private static Vector128<int> Dpbusd(Vector128<byte> padded)
    {
        if (AvxVnni.IsSupported)
        {
            return AvxVnni.MultiplyWideningAndAdd(Vector128<int>.Zero, padded, Weights);
        }

        // _mm_dpbusd_epi32 emulation: maddubs then madd with ones.
        Vector128<short> prod = Ssse3.MultiplyAddAdjacent(padded, Weights);
        return Sse2.MultiplyAddAdjacent(prod, Vector128.Create((short)1));
    }
}
