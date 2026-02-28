import base64
import io
from PIL import Image

def main():
    # Create a 4x4 PNG image (red color)
    img = Image.new('RGB', (4, 4), color='red')

    # Save the image to a bytes buffer
    buffer = io.BytesIO()
    img.save(buffer, format='PNG')
    png_data = buffer.getvalue()

    # Encode to base64
    base64_data = base64.b64encode(png_data).decode('utf-8')

    # Print the base64 string
    print(base64_data)

if __name__ == "__main__":
    main()