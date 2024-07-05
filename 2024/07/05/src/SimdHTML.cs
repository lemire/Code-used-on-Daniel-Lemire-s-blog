using System;
using System.Runtime.Intrinsics;
using System.Runtime.Intrinsics.X86;
using System.Runtime.Intrinsics.Arm;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Numerics;
namespace SimdHTML
{
    public static class FastScan
    {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public unsafe static void NaiveAdvanceString(ref byte* start, byte* end)
        {
            while (start < end)
            {
                if (*start == '<' || *start == '&' || *start == '\r' || *start == '\0')
                {
                    return;
                }
                start++;
            }

        }
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public unsafe static void SIMDAdvanceString(ref byte* start, byte* end)
        {
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
                        return;
                    }
                    start += stride;
                }
            } else if (Avx2.IsSupported)
            {
                // credit : Harold Aptroot
                Vector128<byte> low_nibble_lut128 = Vector128.Create(
                    0, 0, 0, 0, 0, 0, (byte)0x26, 0, 0, 0, 0, 0, (byte)0x3c, (byte)0xd, 0, 0);
                Vector256<byte> low_nibble_lut = Vector256.Create(low_nibble_lut128, low_nibble_lut128);
                Vector256<byte> v0f = Vector128.Create((byte)0x0F);


                const int stride = 32;
                while (start + (stride - 1) < end)
                {
                    Vector256<byte> data = Avx2.LoadVector256((byte*)start);
                    Vector256<byte> transform = Avx2.Shuffle(low_nibble_lut, data & v0f);
                    Vector256<byte> matches_transform = Avx2.CompareEqual(transform, data);
                    int mask = Avx2.MoveMask(matches_transform);
                    if (mask != 0)
                    {
                        start += BitOperations.TrailingZeroCount(mask);
                        return;
                    }
                    start += stride;
                }
            } else if (Ssse3.IsSupported)
            {
                // credit : Harold Aptroot
                Vector128<byte> low_nibble_lut = Vector128.Create(
                    0, 0, 0, 0, 0, 0, (byte)0x26, 0, 0, 0, 0, 0, (byte)0x3c, (byte)0xd, 0, 0);
                Vector128<byte> v0f = Vector128.Create((byte)0x0F);
                const int stride = 16;
                while (start + (stride - 1) < end)
                {
                    Vector128<byte> data = Sse2.LoadVector128((byte*)start);
                    // for bytes < 0x80, lookup based on the lowest nibble
                    // for bytes >= 0x80, zero
                    Vector128<byte> transform = Ssse3.Shuffle(low_nibble_lut, data & v0f);
                    // only for 0, 0x26, 0x3C, 0x0D will the transformed value match the original
                    Vector128<byte> matches_transform = Sse2.CompareEqual(transform, data);
                    int mask = Sse2.MoveMask(matches_transform);
                    if (mask != 0)
                    {
                        start += BitOperations.TrailingZeroCount(mask);
                        return;
                    }
                    start += stride;
                }
            }


            while (start < end)
            {
                if (*start == '<' || *start == '&' || *start == '\r' || *start == '\0')
                {
                    return;
                }
                start++;
            }
        }
    }
}

