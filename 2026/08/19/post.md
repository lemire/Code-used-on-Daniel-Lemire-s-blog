# Parsing IP addresses in C# at crazy speeds

We are all familiar with IP addresses such as `192.168.0.1`. They are typically
written as four numbers in the range 0 to 255 inclusive, separated by dots.
In C#, you can parse them with the standard library using `IPAddress.TryParse`.


Pedantic people are quick to point out that IP addresses can take different forms:
they can be IPv6 or IPv4 and there are many weird ways to write an IPv4 address.
But for the purpose of performance optimization, we care about the common case.
The common case is strings such as `192.168.0.1` or `12.121.244.111`.


Our processors are capable of data parallelism, meaning that they have instructions
(called SIMD) that can process several bytes at once, at least 16 bytes, sometimes
more. [A few years ago, I showed that you can parse IPv4 addresses with SIMD](https://lemire.me/blog/2023/06/08/parsing-ip-addresses-crazily-fast/).
I have been [revisiting this idea with AVX-512](https://github.com/lemire/simdip),
the instruction set that recent x64 (AMD/Intel) processors support.
I expect that all Intel and AMD processors made in the near future will have
great support for AVX-512, and it is already the case for server processors
and recent AMD processors.


So I wondered, could we do it in C#? People are sometimes surprised that I care
about C#. Isn't that more Microsoft slop? No. Not at all. C# and .NET are
very reasonable, portable systems.

Plus you can write fast code in C#. I have two optimized libraries that I hope the Microsoft
.NET team will one day adopt in the standard .NET library: an optimized
`Utf8Utility.GetPointerToFirstInvalidByte` function used internally to validate
Unicode strings (in the [SimdUnicode library](https://github.com/simdutf/SimdUnicode))
and a [fast base64 decoding library](https://github.com/simdutf/SimdBase64).
I love working with .NET C#.

As of .NET 10, we have AVX-512 support, including masked loads. What are masked
loads and why do they matter? Suppose that I give you a string that is no longer
than 16 bytes, but could be shorter. If you load data in a SIMD register, you
normally have to load the full register width (so 8, 16, 32, 64 bytes). So what do
you do when it is not possible? You can pad the input string or pull other
tricks, but it gets dirty. A nice approach is to have masked loads where you, say,
load the full register (say 16 bytes), but you indicate which bytes you want to be
loaded from memory with a mask. So if you use `0b10011` as a mask, then only the
first, second, and fifth bytes are loaded from memory. This makes it possible
to initialize a 16-byte register with a string that has between 0 and 16 bytes,
while never reading beyond the string. I have an article entitled [Modern vector programming with masked loads and stores](https://lemire.me/blog/2022/11/08/modern-vector-programming-with-masked-loads-and-stores/)
if you want to know more.

To make things trickier, C#, like Java and JavaScript, defaults to UTF-16, meaning
that each character, even if it is an ASCII character like `A` or `1`, uses
two bytes. The ASCII codepoint value occupies the least significant bits of a
16-bit word.

So what we need to do is to selectively load from a 32-byte input, and then
drop the unnecessary zero bytes. The gist of it looks as follows in C#.

```csharp
unsafe bool TryParseAvx512(ReadOnlySpan<char> s, out uint ip) {
        int len = s.Length;
        fixed (char* cp = s)
        {
            // next two lines are a trick to load just the first len characters
            Vector256<ushort> charMask = Vector256.LessThan(CharLaneIndex, Vector256.Create((ushort)len));
            Vector256<ushort> chars = Avx512BW.VL.MaskLoad((ushort*)cp, charMask, Vector256.Create((ushort)'0'));
            // check that everything is ASCII otherwise, it is not an IP!
            if (Avx512BW.VL.CompareGreaterThan(chars, Vector256.Create((ushort)0x7F)).ExtractMostSignificantBits() != 0)
            {
                return false;
            }
            // There we go, we have the address as ASCII
            // in a 16-byte register.
            Vector128<byte> str = Avx512BW.VL.ConvertToVector128Byte(chars);
            // ...
        }
}
```

This looks a bit difficult to read, but that's fine. Most people never
need to worry about such code.

Then we use a somewhat fancy trick where we locate the dots, and use
the fact that there are only 81 ways to position the dots. We then move the
bytes, do a dot product and validate. It is the same routine as the C++
code. It is not trivial, but I am working on a formal paper to document
the tricks used.

The pedantic people will say: wait, there are other ways to write IP addresses !!!
Ok fine. We handle them with a fallback, like so.

```csharp
if (TryParseAvx512(s, out uint ip))
{
    address = new IPAddress(ip);
    return true;
}
return IPAddress.TryParse(s, out address);
```

What about the cases where your processor does not support AVX-512? C# makes this
dead easy. You can just guard it with one if:

```csharp
if (Avx512BW.VL.IsSupported) { ... }
```


To benchmark this, I generated 10,000 random 32-bit addresses and parsed the resulting
strings 20 million times, constructing an `IPAddress` each time.
On a relatively recent Intel processor (Intel Xeon Gold 6548N, Emerald Rapids) running .NET 10, I get
the following.

| function              | ns/addr | million addr/s |
| --------------------- | ------: | -------------: |
| `IPAddress.TryParse`  |    45.3 |           22.1 |
| AVX-512 + fallback    |    14.1 |           71.1 |

So the AVX-512 approach is about three times faster than the standard library.
My routine itself does not take fourteen nanoseconds; there is other overhead.


As usual, [the C# source is available](https://github.com/lemire/Code-used-on-Daniel-Lemire-s-blog/tree/master/2026/08/19).
