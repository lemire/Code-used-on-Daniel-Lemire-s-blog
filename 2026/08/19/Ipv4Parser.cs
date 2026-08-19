// AVX-512-VL IPv4 parser, ported from
// https://github.com/lemire/simdip/blob/main/include/avx512ip.h
//
// The SIMD path is strict dotted-decimal (four octets of 1..3 digits, no
// leading zeros). Anything it rejects is handed to IPAddress.TryParse.

using System.Diagnostics.CodeAnalysis;
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
