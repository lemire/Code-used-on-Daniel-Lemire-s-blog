<?php
// URL to parse
$url = "https://microsoft.coⅯ";

// Parse the URL
$parsed_url = parse_url($url);

if ($parsed_url === false) {
    echo "URL could not be parsed.";
} else {
    $host = $parsed_url['host'] ?? 'Not available';

    // Convert the host to UTF-32 without mb_ord
    $utf32 = '';
    for ($i = 0; $i < strlen($host);) {
        $char = $host[$i];
        $byte = ord($char);
        
        if ($byte < 128) {
            // Single-byte character
            $codePoint = $byte;
            $i++;
        } elseif (($byte & 0xE0) == 0xC0) {
            // Two-byte character (110xxxxx)
            $codePoint = (($byte & 0x1F) << 6) | (ord($host[++$i]) & 0x3F);
            $i++;
        } elseif (($byte & 0xF0) == 0xE0) {
            // Three-byte character (1110xxxx)
            $codePoint = (($byte & 0x0F) << 12) | ((ord($host[++$i]) & 0x3F) << 6) | (ord($host[++$i]) & 0x3F);
            $i++;
        } elseif (($byte & 0xF8) == 0xF0) {
            // Four-byte character (11110xxx)
            $codePoint = (($byte & 0x07) << 18) | ((ord($host[++$i]) & 0x3F) << 12) | ((ord($host[++$i]) & 0x3F) << 6) | (ord($host[++$i]) & 0x3F);
            $i++;
        } else {
            // Invalid UTF-8 character or beyond our simple parser
            $codePoint = 0xFFFD; // Replacement character
            $i++;
        }
        
        $utf32 .= pack('N', $codePoint); // Pack each character as a 32-bit integer
    }

    // Convert binary UTF-32 to hexadecimal for display
    $hexUtf32 = bin2hex($utf32);
    
    echo "UTF-32 encoding of the domain: " . $hexUtf32 . "\n";
    echo "Note: UTF-32 encoding results in 4 bytes per character, so each character will appear as 8 hexadecimal digits.";
}
