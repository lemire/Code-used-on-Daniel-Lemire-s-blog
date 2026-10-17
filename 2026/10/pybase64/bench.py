"""Base64 decoding speed in Python: base64 (standard library), pybase64 and
fastpybase64, on plain base64 and on MIME-style base64 (a line break every 76
characters).

Usage: python bench.py > results.csv
Speeds are in GB/s of decoded (binary) data; best of several repetitions.
"""
import base64
import platform
import random
import sys
import timeit

import fastpybase64
import pybase64

LIBRARIES = {"base64": base64, "pybase64": pybase64, "fastpybase64": fastpybase64}
SIZES = [64, 1024, 4096, 65536, 1 << 20]
FORMATS = {
    "plain": base64.b64encode,  # one long line
    "mime": base64.encodebytes,  # a newline every 76 characters
}


def measure(call, min_time=0.2, repeat=7):
    timer = timeit.Timer(call)
    number, elapsed = timer.autorange()
    number = max(1, int(number * min_time / max(elapsed, 1e-9)))
    return min(timer.repeat(repeat=repeat, number=number)) / number


def main():
    print(f"# Python {sys.version.split()[0]}, {platform.platform()}", file=sys.stderr)
    print(f"# fastpybase64 {fastpybase64.__version__} (simdutf {fastpybase64.simdutf_version}, "
          f"{fastpybase64.get_implementation()}), pybase64 {pybase64.get_version()}", file=sys.stderr)
    print("format,validate,size,library,gbps")
    rng = random.Random(42)
    for size in SIZES:
        raw = rng.randbytes(size)
        for fmt, encode in FORMATS.items():
            encoded = encode(raw)
            for validate in (False, True):
                if validate and fmt == "mime":
                    continue  # validate=True rejects line breaks
                for name, module in LIBRARIES.items():
                    func = module.b64decode
                    assert func(encoded, validate=validate) == raw
                    seconds = measure(lambda: func(encoded, validate=validate))
                    gbps = size / seconds / 1e9
                    print(f"{fmt},{validate},{size},{name},{gbps:.3f}", flush=True)


if __name__ == "__main__":
    main()
