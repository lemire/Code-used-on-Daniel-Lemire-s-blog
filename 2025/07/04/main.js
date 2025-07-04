// You can run this with node main.js if you have Node.js
const jsonString = '{"key": "value\nda"}'; 

// Bad control character in string literal in JSON at position 14 (line 1 column 15)
let data = JSON.parse(jsonString);
