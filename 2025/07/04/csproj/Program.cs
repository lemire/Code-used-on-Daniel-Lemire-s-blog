using System;
using System.IO;
using System.Text.Json;

class Program
{
    static void Main()
    {
        try
        {
            string jsonString = "{"key": "value\nda"}";

            // Parse the JSON data
            using JsonDocument doc = JsonDocument.Parse(jsonString);
            JsonElement root = doc.RootElement;

            // Get the value associated with "key"
            if (root.TryGetProperty("key", out JsonElement valueElement))
            {
                Console.WriteLine(valueElement.GetString());
            }
            else
            {
                Console.Error.WriteLine("Key 'key' not found in JSON");
                Environment.Exit(1);
            }
        }
        catch (FileNotFoundException)
        {
            Console.Error.WriteLine("Error: input.json file not found");
            Environment.Exit(1);
        }
        catch (JsonException)
        {
            Console.Error.WriteLine("Error: Invalid JSON format");
            Environment.Exit(1);
        }
    }
}