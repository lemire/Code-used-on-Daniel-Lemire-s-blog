using System.Runtime.Intrinsics;
using System.Runtime.Intrinsics.X86;
using System.Runtime.Intrinsics.Arm;
using System.Runtime.CompilerServices;
using System.Numerics;

namespace SimdHTML;

// modified by Arseniy Zlobintsev
public unsafe struct Neon64Scanner(byte* start, byte* end)
{
    static Vector128<byte> V0F => Vector128.Create((byte)0x0F);
    static Vector128<byte> LowNibbleMask => Vector128.Create((byte)
        0, 0, 0, 0, 0, 0, 0x26, 0, 0, 0, 0, 0, 0x3c, 0xd, 0, 0);
    static Vector128<byte> BitMask => Vector128.Create(
        0x01, 0x02, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80,
        0x01, 0x02, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80);

    ulong matches;
    int offset;

    // C# allows duck-typed matching on IEnumerable<T> signatures for foreach
    public Neon64Scanner GetEnumerator() => this;

    // Usually this is an anti-pattern: IEnumerator<T>.Current
    // should be side-effect free. However, because this is a low-level
    // type, we can make an exception.
    public byte* Current
    {
        get
        {
            var curr = start + offset;
            matches >>= 1;
            offset++;
            return curr;
        }
    }

    public bool MoveNext()
    {
        while (matches is 0)
        {
            var ptr = start;
            if (ptr + 64 < end)
            {
                matches = Scan(ptr);
                start = ptr + 64;
                offset = 0;
            }
            else
            {
                if (ptr >= end)
                {
                    return false;
                }

                matches = ScanShort(ptr, end);
                start = end;
                offset = 0;

                if (matches is 0)
                {
                    return false;
                }
            }
        }

        var off = BitOperations.TrailingZeroCount(matches);
        matches >>= off;
        offset += off;
        return true;
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    static ulong Scan(byte* s)
    {
        var (data1, data2, data3, data4) = AdvSimd.Arm64.Load4xVector128(s);
        var lowpart1 = AdvSimd.Arm64.VectorTableLookup(LowNibbleMask, data1 & V0F);
        var lowpart2 = AdvSimd.Arm64.VectorTableLookup(LowNibbleMask, data2 & V0F);
        var lowpart3 = AdvSimd.Arm64.VectorTableLookup(LowNibbleMask, data3 & V0F);
        var lowpart4 = AdvSimd.Arm64.VectorTableLookup(LowNibbleMask, data4 & V0F);
        var matchesones1 = Vector128.Equals(lowpart1, data1);
        var matchesones2 = Vector128.Equals(lowpart2, data2);
        var matchesones3 = Vector128.Equals(lowpart3, data3);
        var matchesones4 = Vector128.Equals(lowpart4, data4);
        var sum0 = AdvSimd.Arm64.AddPairwise(matchesones1 & BitMask, matchesones2 & BitMask);
        var sum1 = AdvSimd.Arm64.AddPairwise(matchesones3 & BitMask, matchesones4 & BitMask);
        sum0 = AdvSimd.Arm64.AddPairwise(sum0, sum1);
        sum0 = AdvSimd.Arm64.AddPairwise(sum0, sum0);
        return sum0.AsUInt64().ToScalar();
    }

    static ulong ScanShort(byte* s, byte* end)
    {
        var buffer = (stackalloc byte[64]);
        buffer.Fill(1);

        var length = (int)(uint)(end - s);
        new Span<byte>(s, length).CopyTo(buffer);

        fixed (byte* ptr = buffer) return Scan(ptr);
    }
}

public unsafe struct NeonMatch
{
    private byte* _start;
    private readonly byte* _end;
    private int _offset;
    private ulong _matches;

    private static readonly Vector128<byte> v0f = Vector128.Create((byte)0x0F);
    private static readonly Vector128<byte> low_nibble_mask = Vector128.Create(0, 0, 0, 0, 0, 0, (byte)0x26, 0, 0, 0, 0, 0, (byte)0x3c, (byte)0xd, 0, 0);

    public NeonMatch(byte* start, byte* end)
    {
        _start = start;
        _end = end;
        _offset = 0;
        _matches = 0;

        if (_start + 16 >= _end)
        {
            carefulUpdate();
        }
        else
        {
            Update();
        }
    }

    public byte* Get() => _start + _offset;

    public void Consume()
    {
        _offset++;
        _matches >>= 4;
    }

    public bool Advance()
    {
        while (_matches == 0)
        {
            _start += 16;


            if (_start + 16 >= _end)
            {
                if (_start >= _end)
                {
                    return false;
                }
                carefulUpdate();

                if (_matches == 0)
                {
                    return false;
                }
            }
            else
            {
                Update();
            }
        }

        int off = BitOperations.TrailingZeroCount(_matches);
        _matches >>= off;
        _offset += off >> 2;
        return true;
    }
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    private void carefulUpdate()
    {
        // 'stackalloc' blocks inlining because it is
        // a very special construct which emits a
        // stack cookie to guard against stack overruns
        // and inserts a failfast path should that happen.
        // use "inline array" instead which is automatically
        // emitted by Roslyn (C# compiler) for idioms like this.
        Span<byte> buffer =
        [
            1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1
        ];

        fixed (byte* ptr = buffer)
        {
            Buffer.MemoryCopy(_start, ptr, 16, _end - _start);
            Update(ptr);
        }
    }
    private void Update() => Update(_start);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    private void Update(byte* s)
    {
        Vector128<byte> data = AdvSimd.LoadVector128(s);
        Vector128<byte> lowpart = AdvSimd.Arm64.VectorTableLookup(low_nibble_mask, data & v0f);
        Vector128<byte> matchesones = AdvSimd.CompareEqual(lowpart, data);
        _matches = AdvSimd.ShiftRightLogicalNarrowingLower(matchesones.AsUInt16(), 4).AsUInt64().ToScalar();
        _offset = 0;
    }

}
public static class FastScan
{
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public unsafe static void NaiveAdvanceString(ref byte* startm, byte* end)
    {
        byte* start = startm;
        while (start < end)
        {
            if (*start == '<' || *start == '&' || *start == '\r' || *start == '\0')
            {
                startm = start;
                return;
            }
            start++;
        }

    }
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public unsafe static void SIMDAdvanceString(ref byte* startm, byte* end)
    {
        byte* start = startm;
        if (AdvSimd.Arm64.IsSupported)
        {
            Vector128<byte> low_nibble_mask = Vector128.Create(0, 0, 0, 0, 0, 0, (byte)0x26, 0, 0, 0, 0, 0, (byte)0x3c, (byte)0xd, 0, 0);
            Vector128<byte> v0f = Vector128.Create((byte)0x0F);
            Vector128<byte> bit_mask = Vector128.Create((byte)16, 15, 14, 13, 12, 11, 10, 9, 8,
                                7, 6, 5, 4, 3, 2, 1);

            const int stride = 16;
            while (start + (stride - 1) < end)
            {
                Vector128<byte> data = AdvSimd.LoadVector128((byte*)start);
                Vector128<byte> lowpart = AdvSimd.Arm64.VectorTableLookup(low_nibble_mask, data & v0f);
                Vector128<byte> matchesones = AdvSimd.CompareEqual(lowpart, data);
                if (matchesones != Vector128<byte>.Zero)
                {
                    Vector128<byte> matches = AdvSimd.And(bit_mask, matchesones);
                    int m = AdvSimd.Arm64.MaxAcross(matches).ToScalar();
                    start += 16 - m;
                    startm = start;
                    return;
                }
                start += stride;
            }
        }
        // We deliberately disable it
        else if (false) // Vector512.IsHardwareAccelerated && Avx512F.IsSupported && Avx512BW.IsSupported && Avx512Vbmi.IsSupported)
        {
            Vector512<byte> low_nibble_lut = Vector512.Create(0, 0, 0, 0, 0, 0, (byte)0x26, 0, 0, 0, 0, 0, (byte)0x3c, (byte)0xd, 0, 0,
            0, 0, 0, 0, 0, 0, (byte)0x26, 0, 0, 0, 0, 0, (byte)0x3c, (byte)0xd, 0, 0,
            0, 0, 0, 0, 0, 0, (byte)0x26, 0, 0, 0, 0, 0, (byte)0x3c, (byte)0xd, 0, 0,
            0, 0, 0, 0, 0, 0, (byte)0x26, 0, 0, 0, 0, 0, (byte)0x3c, (byte)0xd, 0, 0);
            const int stride = 64;
            while (start + (stride - 1) < end)
            {
                Vector512<byte> data = Avx512F.LoadVector512((byte*)start);
                Vector512<byte> transform = Avx512BW.Shuffle(low_nibble_lut, data);
                Vector512<byte> matches_transform = Avx512BW.CompareEqual(transform, data);
                UInt64 mask = matches_transform.ExtractMostSignificantBits(); ;
                if (mask != 0)
                {
                    start += BitOperations.TrailingZeroCount(mask);
                    startm = start;
                    return;
                }
                start += stride;
            }
        }
        else if (Avx2.IsSupported)
        {
            // credit : Harold Aptroot
            Vector128<byte> low_nibble_lut128 = Vector128.Create(
                0, 0, 0, 0, 0, 0, (byte)0x26, 0, 0, 0, 0, 0, (byte)0x3c, (byte)0xd, 0, 0);
            Vector256<byte> low_nibble_lut = Vector256.Create(low_nibble_lut128, low_nibble_lut128);


            const int stride = 32;
            while (start + (stride - 1) < end)
            {
                Vector256<byte> data = Avx2.LoadVector256((byte*)start);
                Vector256<byte> transform = Avx2.Shuffle(low_nibble_lut, data);
                Vector256<byte> matches_transform = Avx2.CompareEqual(transform, data);
                int mask = Avx2.MoveMask(matches_transform);
                if (mask != 0)
                {
                    start += BitOperations.TrailingZeroCount(mask);
                    startm = start;
                    return;
                }
                start += stride;
            }
        }
        else if (Ssse3.IsSupported)
        {
            // credit : Harold Aptroot
            Vector128<byte> low_nibble_lut = Vector128.Create(
                0, 0, 0, 0, 0, 0, (byte)0x26, 0, 0, 0, 0, 0, (byte)0x3c, (byte)0xd, 0, 0);
            const int stride = 16;
            while (start + (stride - 1) < end)
            {
                Vector128<byte> data = Sse2.LoadVector128((byte*)start);
                // for bytes < 0x80, lookup based on the lowest nibble
                // for bytes >= 0x80, zero
                Vector128<byte> transform = Ssse3.Shuffle(low_nibble_lut, data);
                // only for 0, 0x26, 0x3C, 0x0D will the transformed value match the original
                Vector128<byte> matches_transform = Sse2.CompareEqual(transform, data);
                int mask = Sse2.MoveMask(matches_transform);
                if (mask != 0)
                {
                    start += BitOperations.TrailingZeroCount(mask);
                    startm = start;
                    return;
                }
                start += stride;
            }
        }


        while (start < end)
        {
            if (*start == '<' || *start == '&' || *start == '\r' || *start == '\0')
            {
                startm = start;
                return;
            }
            start++;
        }
        startm = start;
    }
}

