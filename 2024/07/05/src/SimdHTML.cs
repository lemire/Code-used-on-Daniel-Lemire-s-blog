using System;
using System.Runtime.Intrinsics;
using System.Runtime.Intrinsics.X86;
using System.Runtime.Intrinsics.Arm;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Numerics;
namespace SimdHTML
{

    public unsafe struct NeonMatch
    {
        private byte* _start;
        private readonly byte* _end;
        private int _offset;
        private ulong _matches;
        private Vector128<byte> v0f;
        private Vector128<byte> low_nibble_mask;
        public NeonMatch(byte* start, byte* end)
        {
            _start = start;
            _end = end;
            low_nibble_mask = Vector128.Create(0, 0, 0, 0, 0, 0, (byte)0x26, 0, 0, 0, 0, 0, (byte)0x3c, (byte)0xd, 0, 0);
            v0f = Vector128.Create((byte)0x0F);
            _offset = 0;
            _matches = 0;

            if (_start + 16 >= _end)
            {
                Span<byte> buffer = stackalloc byte[16];
                buffer.Fill(1);
                fixed (byte* ptr = buffer)
                {
                    Buffer.MemoryCopy(_start, ptr, 16, _end - _start);
                    Update(ptr);
                }
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
                if (_start >= _end)
                {
                    return false;
                }

                if (_start + 16 >= _end)
                {
                    Span<byte> buffer = stackalloc byte[16];
                    buffer.Fill(1);
                    fixed (byte* ptr = buffer)
                    {
                        Buffer.MemoryCopy(_start, ptr, 16, _end - _start);
                        Update(ptr);
                    }

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

        private void Update() => Update(_start);

        private void Update(byte* s)
        {
            Vector128<byte> data = AdvSimd.LoadVector128(s);
            Vector128<byte> lowpart = AdvSimd.Arm64.VectorTableLookup(low_nibble_mask, data & v0f);
            Vector128<byte> matchesones = AdvSimd.CompareEqual(lowpart, data);
            _matches = AdvSimd.ShiftRightLogicalNarrowingLower(matchesones.AsUInt16(), 4).AsUInt64().ToScalar();
            _offset = 0;
        }

        public override bool Equals(object obj)
        {
            throw new NotImplementedException();
        }

        public override int GetHashCode()
        {
            throw new NotImplementedException();
        }

        public static bool operator ==(NeonMatch left, NeonMatch right)
        {
            return left.Equals(right);
        }

        public static bool operator !=(NeonMatch left, NeonMatch right)
        {
            return !(left == right);
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

