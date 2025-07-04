import com.fasterxml.jackson.databind.ObjectMapper;

import java.util.HashMap;
import java.util.Map;

public class Program {
    public static void main(String[] args) {
        // JSON string with an invalid control character (newline)
        String jsonString = "{\"key\": \"value\nda\"}";

        try {
            // Parse the JSON string into a Map
            Map<String, Object> data = parseJson(jsonString);

            // Get the value associated with "key"
            String value = (String) data.get("key");
            System.out.println("value " + value);
        } catch (Exception e) {
            System.out.println("JSON parsing error: " + e.getMessage());
        }
    }

    private static Map<String, Object> parseJson(String jsonString) throws Exception {
        ObjectMapper objectMapper = new ObjectMapper();
        return objectMapper.readValue(jsonString, Map.class);
    }
}
