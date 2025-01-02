using System;
using System.Net;
        
// https://github.com/dotnet-script/dotnet-script
        string url = "https://microsoft.coⅯ";

        try
        {
            Uri uri = new Uri(url);
            string host = uri.Host;

            if (string.IsNullOrEmpty(host))
            {
                Console.WriteLine("URL could not be parsed.");
                return;
            }

            // Convert host to UTF-32
            string hexUtf32 = "";
            foreach (char c in host)
            {
                int codePoint = char.ConvertToUtf32(host, host.IndexOf(c));
                byte[] utf32Bytes = BitConverter.GetBytes(codePoint);

                // Ensure big-endian order since BitConverter uses little-endian
                Array.Reverse(utf32Bytes);

                // Convert to hex for display
                hexUtf32 += BitConverter.ToString(utf32Bytes).Replace("-", "").ToLower();
            }

            Console.WriteLine($"UTF-32 encoding of the domain: {hexUtf32}");
            Console.WriteLine("Note: UTF-32 encoding results in 4 bytes per character, so each character will appear as 8 hexadecimal digits.");
        }
        catch (UriFormatException e)
        {
            Console.WriteLine("URL could not be parsed: " + e.Message);
        }
