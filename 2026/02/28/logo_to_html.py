import base64
import os
import textwrap

def main():
    image_path = 'logo_simdjson_noir.png'
    if not os.path.exists(image_path):
        print(f"File {image_path} not found.")
        return

    with open(image_path, 'rb') as f:
        image_data = f.read()

    base64_data = base64.b64encode(image_data).decode('utf-8')

    wrapped_base64 = textwrap.fill(base64_data, width=72)

    html_content = f"""<!DOCTYPE html>
<html>
<head>
    <title>SimDJSON Logo</title>
</head>
<body>
    <img src="data:image/png;base64,{wrapped_base64}" alt="SimDJSON Logo">
</body>
</html>"""

    with open('logo.html', 'w') as f:
        f.write(html_content)

    print("HTML page generated: logo.html")

if __name__ == "__main__":
    main()