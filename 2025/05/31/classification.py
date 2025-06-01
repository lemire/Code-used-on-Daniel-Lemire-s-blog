from z3 import *
s = Solver()
lut_lo = [BitVec(f'lut_lo_{i}', 8) for i in range(16)]
lut_hi = [BitVec(f'lut_hi_{i}', 8) for i in range(16)]
# Base64 characters (A-Z, a-z, 0-9, +, /)
base64_chars = list(range(65, 91)) + list(range(97, 123)) + list(range(48, 58)) + [43, 47]
invalid_chars = [c for c in range(256) if c not in base64_chars]
def classify(c):
    lower_nibble = c & 0x0F
    upper_nibble = c >> 4
    return lut_lo[lower_nibble] & lut_hi[upper_nibble]
for c in base64_chars:
    s.add(classify(c) == 0)
for c in invalid_chars:
    s.add(classify(c) > 0)
if s.check() == sat:
    m = s.model()
    print("lut_lo =", [m[lut_lo[i]].as_long() for i in range(16)])
    print("lut_hi =", [m[lut_hi[i]].as_long() for i in range(16)])