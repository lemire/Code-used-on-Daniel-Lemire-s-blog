import java.net.URI;
import java.net.URISyntaxException;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;

public class urldomain {
    public static void main(String[] args) {
        String url = "https://microsoft.coⅯ";

        try {
            URI uri = new URI(url);
            String host = uri.getHost();

            if (host == null) {
                System.out.println("URL could not be parsed.");
                return;
            }

            // Convert host to UTF-32
            StringBuilder hexUtf32 = new StringBuilder();
            for (char c : host.toCharArray()) {
                int codePoint = Character.codePointAt(new char[]{c}, 0);
                byte[] utf32Bytes = ByteBuffer.allocate(4).putInt(0, codePoint).array();
                
                // Convert to hex for display
                for (byte b : utf32Bytes) {
                    hexUtf32.append(String.format("%02x", b));
                }
            }

            System.out.println("UTF-32 encoding of the domain: " + hexUtf32);
            System.out.println("Note: UTF-32 encoding results in 4 bytes per character, so each character will appear as 8 hexadecimal digits.");
        } catch (URISyntaxException e) {
            System.out.println("URL could not be parsed: " + e.getMessage());
        }
    }
}
