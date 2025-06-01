from z3 import *

s = Solver()
lut_lo = [BitVec(f'lut_lo_{i}', 8) for i in range(16)]
lut_hi = [BitVec(f'lut_hi_{i}', 8) for i in range(16)]


# Define the base64 characters, whitespace characters, and invalid characters
# Base64 characters (A-Z, a-z, 0-9, +, /)
base64_chars = list(range(65, 91)) + list(range(97, 123)) + list(range(48, 58)) + [43, 47]
assert len(base64_chars) == 64, "Base64 character set should contain 64 characters."
print("Base64 characters:", [chr(c) for c in base64_chars])

all_chars = list(range(256))
invalid_chars = [c for c in all_chars if c not in base64_chars]

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
    lut_lo_values = [m[lut_lo[i]].as_long() for i in range(16)]
    lut_hi_values = [m[lut_hi[i]].as_long() for i in range(16)]
    
    print("Solution found!")
    print("lut_lo =", lut_lo_values)
    print("lut_hi =", lut_hi_values)
    
    # Verify the solution for a few characters
    def verify_classification(c):
        lower = c & 0x0F
        upper = c >> 4
        result = lut_lo_values[lower] & lut_hi_values[upper]
        return result
    
    # Test a base64 character: 'A' (ASCII 65)
    for c in base64_chars:
        assert verify_classification(c) == 0, f"Base64 character {chr(c)} should classify as 0"
    # Test an invalid character: '#' (ASCII 35)
    for c in invalid_chars:
        assert verify_classification(c) > 0, f"Invalid character {chr(c)} should classify as > 0"
    
    print("Verification passed for sample characters.")
else:
    print("No solution exists.")
