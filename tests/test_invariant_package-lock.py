import pytest
import os
import urllib.parse
import posixpath


# Simulated secure file resolver that represents what a secure implementation should do
def resolve_path_securely(root: str, user_input: str) -> str:
    """
    Simulates a secure path resolution function.
    Returns the resolved absolute path if within root, raises ValueError otherwise.
    """
    # Decode URL encoding
    decoded = urllib.parse.unquote(user_input)
    
    # Normalize the path
    # Join root with the user-supplied path
    joined = os.path.join(root, decoded.lstrip('/'))
    
    # Resolve to absolute path (resolves .., ., symlinks conceptually)
    resolved = os.path.normpath(joined)
    
    # Ensure the resolved path starts with the root
    root_real = os.path.normpath(root)
    
    if not resolved.startswith(root_real + os.sep) and resolved != root_real:
        raise ValueError(f"Path traversal detected: '{user_input}' resolves outside root '{root}'")
    
    return resolved


def is_path_safe(root: str, user_input: str) -> bool:
    """Returns True if path is safe (within root), False if traversal detected."""
    try:
        # Decode various encodings
        decoded = user_input
        for _ in range(5):  # Multiple decode passes to handle double encoding
            decoded = urllib.parse.unquote(decoded)
        
        # Replace common traversal patterns
        normalized = decoded.replace('\\', '/')
        
        # Join and normalize
        joined = posixpath.join(root, normalized.lstrip('/'))
        resolved = posixpath.normpath(joined)
        
        root_normalized = posixpath.normpath(root)
        
        return resolved.startswith(root_normalized + '/') or resolved == root_normalized
    except Exception:
        return False


@pytest.mark.parametrize("payload", [
    # Classic path traversal
    "../../../etc/passwd",
    "../../etc/passwd",
    "../etc/passwd",
    "../../../../etc/shadow",
    "../../../../../etc/hosts",
    
    # Double dot variations
    "....//....//etc/passwd",
    "....\\\\....\\\\etc\\\\passwd",
    "....//../../../etc/passwd",
    
    # URL encoded
    "%2e%2e%2fetc%2fpasswd",
    "%2e%2e/%2e%2e/etc/passwd",
    "%2e%2e%2f%2e%2e%2fetc%2fpasswd",
    "..%2fetc%2fpasswd",
    "..%2F..%2Fetc%2Fpasswd",
    "%2e%2e%2f%2e%2e%2f%2e%2e%2fetc%2fpasswd",
    
    # Double URL encoded
    "%252e%252e%252fetc%252fpasswd",
    "%252e%252e/etc/passwd",
    "..%252fetc%252fpasswd",
    
    # Unicode/UTF-8 encoded
    "%c0%ae%c0%ae%c0%afetc%c0%afpasswd",
    "%e0%80%ae%e0%80%ae%e0%80%afetc%e0%80%afpasswd",
    
    # Windows-style separators
    "..\\..\\..\\windows\\system32\\drivers\\etc\\hosts",
    "..\\..\\etc\\passwd",
    "..\\..\\/etc/passwd",
    
    # Mixed separators
    "..//../../etc/passwd",
    "..\\/../etc/passwd",
    
    # Null byte injection
    "../../../etc/passwd\x00.jpg",
    "../../../etc/passwd%00.jpg",
    
    # Absolute paths
    "/etc/passwd",
    "/etc/shadow",
    "//etc/passwd",
    
    # Long traversal chains
    "../../../../../../../../../../../../etc/passwd",
    "../" * 20 + "etc/passwd",
    
    # Encoded slash variations
    "..%c0%afetc%c0%afpasswd",
    "..%c1%9cetc/passwd",
    
    # Dot variations
    ".%2e/etc/passwd",
    "%2e./etc/passwd",
    ".%2e%2fetc%2fpasswd",
    
    # Fast-uri specific bypass attempts
    "/%2e%2e/%2e%2e/etc/passwd",
    "/..%2F..%2Fetc%2Fpasswd",
    "/%252e%252e/%252e%252e/etc/passwd",
    
    # Tilde and home directory
    "~/../../etc/passwd",
    "~root/../../etc/passwd",
])
def test_path_traversal_stays_within_root(payload):
    """Invariant: File operations must never resolve paths outside the declared root directory.
    
    Any path traversal attempt must either be rejected or resolved to a path
    within the allowed root boundary. This guards against CWE-22 path traversal
    vulnerabilities similar to the fast-uri bypass vulnerability.
    """
    root = "/var/www/html"
    
    # The resolved path must stay within root OR the operation must be rejected
    path_is_safe = is_path_safe(root, payload)
    
    if path_is_safe:
        # If considered safe, verify it actually resolves within root
        try:
            resolved = resolve_path_securely(root, payload)
            root_normalized = os.path.normpath(root)
            assert resolved.startswith(root_normalized), (
                f"SECURITY VIOLATION: payload '{payload}' resolved to '{resolved}' "
                f"which is outside root '{root}'"
            )
        except ValueError:
            # Rejection is also acceptable
            pass
    else:
        # Path was detected as unsafe - verify that secure resolver also rejects it
        with pytest.raises(ValueError, match="Path traversal detected"):
            resolve_path_securely(root, payload)


@pytest.mark.parametrize("payload", [
    "../../../etc/passwd",
    "%2e%2e%2fetc%2fpasswd",
    "....//....//etc/passwd",
    "..\\..\\..\\etc\\passwd",
    "%252e%252e%252fetc%252fpasswd",
    "../../../../../../../../etc/passwd",
])
def test_path_traversal_never_escapes_root_boundary(payload):
    """Invariant: Under no circumstances should a resolved path escape the root boundary.
    
    This is the core security property: the resolved absolute path must always
    be a subdirectory of (or equal to) the declared root.
    """
    root = "/var/www/html"
    root_normalized = os.path.normpath(root)
    
    # Attempt multiple decode passes (simulating what an attacker might exploit)
    decoded_variants = set()
    current = payload
    decoded_variants.add(current)
    for _ in range(5):
        current = urllib.parse.unquote(current)
        decoded_variants.add(current)
    
    for variant in decoded_variants:
        # Normalize the variant
        normalized = os.path.normpath(os.path.join(root, variant.lstrip('/')))
        
        # SECURITY INVARIANT: resolved path must be within root
        assert normalized.startswith(root_normalized), (
            f"SECURITY VIOLATION: payload '{payload}' (variant: '{variant}') "
            f"resolved to '{normalized}' which escapes root '{root}'"
        )