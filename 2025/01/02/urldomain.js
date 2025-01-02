 const url = "https://microsoft.coⅯ";

        // Parse the URL
        const urlObj = new URL(url);
        const host = urlObj.hostname;

        // Convert host to UTF-32
        let hexUtf32 = '';
        for (let i = 0; i < host.length; i++) {
            // Convert each character to its UTF-32 code point
            const codePoint = host.codePointAt(i);
            
            // Convert code point to 4 bytes in big-endian order
            const buffer = new ArrayBuffer(4);
            const view = new DataView(buffer);
            view.setUint32(0, codePoint, false); // false for big-endian
            
            // Convert bytes to hex string
            for (let j = 0; j < 4; j++) {
                const byte = view.getUint8(j);
                hexUtf32 += byte.toString(16).padStart(2, '0');
            }

            // Skip surrogate pairs by moving to the next character if necessary
            if (codePoint > 0xFFFF) i++;
        }

        console.log(`UTF-32 encoding of the domain: ${hexUtf32}`);
        console.log("Note: UTF-32 encoding results in 4 bytes per character, so each character will appear as 8 hexadecimal digits.");
