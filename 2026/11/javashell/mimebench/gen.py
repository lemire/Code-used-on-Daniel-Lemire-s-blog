# Generates a MIME base64 input (76-char lines, CRLF).
import base64, random, sys, zlib
n = int(sys.argv[1]) if len(sys.argv) > 1 else 1_000_000
random.seed(1234)
raw = bytes(random.getrandbits(8) for _ in range(n))
plain = base64.b64encode(raw)
mime = b"\r\n".join(plain[i:i+76] for i in range(0, len(plain), 76)) + b"\r\n"
assert base64.b64decode(mime) == raw
open("mime.txt", "wb").write(mime)
open("expected.txt", "w").write(f"{len(raw)} {sum(raw)}\n")
print(f"decoded {len(raw)} bytes, mime {len(mime)} bytes")
