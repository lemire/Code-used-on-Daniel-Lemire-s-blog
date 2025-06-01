from z3 import *

# Initialize Z3 solver
s = Solver()

# Define the lookup tables as arrays of 8-bit values (0 to 255)
# lut_lo, lut_hi, and roll each have 16 entries (for nibbles 0 to 15)
lut_lo = [BitVec(f'lut_lo_{i}', 8) for i in range(16)]
lut_hi = [BitVec(f'lut_hi_{i}', 8) for i in range(16)]
roll = [BitVec(f'roll_{i}', 8) for i in range(16)]


# Define the base64 characters, whitespace characters, and invalid characters
# Base64 characters (A-Z, a-z, 0-9, +, /)
base64_chars = list(range(65, 91)) + list(range(97, 123)) + list(range(48, 58)) + [43, 47]
assert len(base64_chars) == 64, "Base64 character set should contain 64 characters."


# All ASCII characters (0 to 255) for classification
all_chars = list(range(256))

# Invalid characters are those that are neither base64 nor whitespace
invalid_chars = [c for c in all_chars if c not in base64_chars]

# Define the classification function f(c) = lut_lo[c & 0x0F] & lut_hi[c >> 4]
def classify(c):
    lower_nibble = c & 0x0F
    upper_nibble = c >> 4
    return lut_lo[lower_nibble] & lut_hi[upper_nibble]

# Add classification constraints
for c in base64_chars:
    s.add(classify(c) == 0)


for c in invalid_chars:
    s.add(classify(c) > 0)

# Add transformation constraints for the roll array
# Map each base64 character to its corresponding 6-bit value
base64_values = {}
# A-Z: 0 to 25
for i, c in enumerate(range(65, 91)):
    base64_values[c] = i
# a-z: 26 to 51
for i, c in enumerate(range(97, 123)):
    base64_values[c] = i + 26
# 0-9: 52 to 61
for i, c in enumerate(range(48, 58)):
    base64_values[c] = i + 52
# +: 62
base64_values[43] = 62
# /: 63
base64_values[47] = 63

# Add constraints for the transformation
for c in base64_chars:
    expected_value = base64_values[c]
    upper_nibble = c >> 4
    if c == 47:
        upper_nibble -= 1
    index = (upper_nibble) & 0x0F  # Modulo 16
    s.add(c + roll[index] == expected_value)

# Check if the constraints are satisfiable
if s.check() == sat:
    m = s.model()
    # Extract the values for lut_lo, lut_hi, and roll
    lut_lo_values = [m[lut_lo[i]].as_long() for i in range(16)]
    lut_hi_values = [m[lut_hi[i]].as_long() for i in range(16)]
    roll_values = [0 if m[roll[i]] is None else m[roll[i]].as_long() for i in range(16)]
    
    print("Solution found!")
    print("lut_lo =", lut_lo_values)
    print("lut_hi =", lut_hi_values)
    print("roll =", roll_values)
    
    # Verify the solution for a few characters
    def verify_classification(c):
        lower = c & 0x0F
        upper = c >> 4
        result = lut_lo_values[lower] & lut_hi_values[upper]
        return result
    
    def verify_transformation(c):
        upper = c >> 4
        index_adjust = -1 if c == 47 else 0
        index = (upper + index_adjust) & 0x0F
        result = (c + roll_values[index]) & 0xFF
        return result
    
    # Test classification
    # Test a base64 character: 'A' (ASCII 65)
    for c in base64_chars:
        assert verify_classification(c) == 0, f"Base64 character {chr(c)} should classify as 0"
    # Test an invalid character: '#' (ASCII 35)
    for c in invalid_chars:
        assert verify_classification(c) > 0, f"Invalid character {chr(c)} should classify as > 0"
    
    # Test transformation
    assert verify_transformation(65) == 0, "Base64 character 'A' should transform to 0"
    assert verify_transformation(90) == 25, "Base64 character 'Z' should transform to 25"
    assert verify_transformation(97) == 26, "Base64 character 'a' should transform to 26"
    assert verify_transformation(122) == 51, "Base64 character 'z' should transform to 51"
    assert verify_transformation(48) == 52, "Base64 character '0' should transform to 52"
    assert verify_transformation(57) == 61, "Base64 character '9' should transform to 61"
    assert verify_transformation(43) == 62, "Base64 character '+' should transform to 62"
    assert verify_transformation(47) == 63, "Base64 character '/' should transform to 63"
    
    print("Verification passed for sample characters.")
else:
    print("No solution exists.")