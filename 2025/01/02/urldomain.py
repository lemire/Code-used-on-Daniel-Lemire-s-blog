import urllib.parse

def main():
    url = "https://microsoft.coⅯ"

    # Parse the URL
    try:
        parsed_url = urllib.parse.urlparse(url)
        host = parsed_url.netloc  # Equivalent to host in this context
    except ValueError:
        print("URL could not be parsed.")
        return

    # Convert host to UTF-32
    utf32_bytes = bytearray()
    for char in host:
        # Convert each character to its UTF-32 representation
        code_point = ord(char)
        utf32_bytes.extend(code_point.to_bytes(4, byteorder='big'))

    # Convert to hexadecimal for display
    hex_utf32 = utf32_bytes.hex()

    print(f"UTF-32 encoding of the domain: {hex_utf32}")
    print("Note: UTF-32 encoding results in 4 bytes per character, so each character will appear as 8 hexadecimal digits.")

if __name__ == "__main__":
    main()
