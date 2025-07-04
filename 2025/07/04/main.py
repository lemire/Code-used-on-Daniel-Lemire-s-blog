import json
# This will throw with 
# Invalid control character at: line 1 column 15 (char 14)


json_string = '{"key": "value\nda"}' 
data = json.loads(json_string)
