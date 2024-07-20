using System;
using System.Runtime.Intrinsics;
using System.Runtime.Intrinsics.X86;
using System.Runtime.Intrinsics.Arm;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Numerics;
namespace SimdHTML
{


    public unsafe sealed class NeonMatch64
    {

        private byte* _start;
        private readonly byte* _end;
        private int _offset;
        private UInt64 _matches;
        private static readonly Vector128<byte> v0f = Vector128.Create((byte)0x0F);
        private static readonly Vector128<byte> low_nibble_mask = Vector128.Create(0, 0, 0, 0, 0, 0, (byte)0x26, 0, 0, 0, 0, 0, (byte)0x3c, (byte)0xd, 0, 0);
        private static readonly Vector128<byte> bit_mask = Vector128.Create(0x01, 0x02, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0x01, 0x02, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80);

        public NeonMatch64(byte* start, byte* end)
        {
            _start = start;
            _end = end;

            _offset = 0;
            _matches = 0;

            if (_start + 64 >= _end)
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
            _matches >>= 1;
        }

        public bool Advance()
        {
            while (_matches == 0)
            {
                _start += 64;

                if (_start + 64 >= _end)
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
            _offset += off;
            return true;
        }

        private void carefulUpdate()
        {
            Span<byte> buffer = stackalloc byte[64];
            buffer.Fill(1);
            fixed (byte* ptr = buffer)
            {
                Buffer.MemoryCopy(_start, ptr, 64, _end - _start);
                Update(ptr);
            }
        }
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        private void Update() => Update(_start);

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        private void Update(byte* s)
        {
            Vector128<byte> data1 = AdvSimd.LoadVector128(s);
            Vector128<byte> data2 = AdvSimd.LoadVector128(s + 16);
            Vector128<byte> data3 = AdvSimd.LoadVector128(s + 32);
            Vector128<byte> data4 = AdvSimd.LoadVector128(s + 48);
            Vector128<byte> lowpart1 = AdvSimd.Arm64.VectorTableLookup(low_nibble_mask, data1 & v0f);
            Vector128<byte> lowpart2 = AdvSimd.Arm64.VectorTableLookup(low_nibble_mask, data2 & v0f);
            Vector128<byte> lowpart3 = AdvSimd.Arm64.VectorTableLookup(low_nibble_mask, data3 & v0f);
            Vector128<byte> lowpart4 = AdvSimd.Arm64.VectorTableLookup(low_nibble_mask, data4 & v0f);
            Vector128<byte> matchesones1 = AdvSimd.CompareEqual(lowpart1, data1);
            Vector128<byte> matchesones2 = AdvSimd.CompareEqual(lowpart2, data2);
            Vector128<byte> matchesones3 = AdvSimd.CompareEqual(lowpart3, data3);
            Vector128<byte> matchesones4 = AdvSimd.CompareEqual(lowpart4, data4);
            Vector128<byte> sum0 = AdvSimd.Arm64.AddPairwise(matchesones1 & bit_mask, matchesones2 & bit_mask);
            Vector128<byte> sum1 = AdvSimd.Arm64.AddPairwise(matchesones3 & bit_mask, matchesones4 & bit_mask);
            sum0 = AdvSimd.Arm64.AddPairwise(sum0, sum1);
            sum0 = AdvSimd.Arm64.AddPairwise(sum0, sum0);
            _matches = sum0.AsUInt64().ToScalar();
            _offset = 0;
        }
    }
    public unsafe sealed class NeonMatch
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
            Span<byte> buffer = stackalloc byte[16];
            buffer.Fill(1);
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
}

