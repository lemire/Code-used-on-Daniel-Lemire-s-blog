import sys

def count_ascii_text(file_path):
    try:
        ascii_count = 0
        with open(file_path, 'rb') as file:
            binary_data = file.read()
            i = 0
            while(i < len(binary_data)):
              byte = binary_data[i]
              if( 32 <= byte <= 126 ):
                j = 1
                while(i+j < len(binary_data)):
                  byte = binary_data[i+j]
                  if(byte < 32 or byte > 126):
                      break
                  j = j + 1
                if(j >= 16):
                  ascii_count = ascii_count + j
                i = i + j - 1
              i = i + 1
            return ascii_count
    except FileNotFoundError:
        return None

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python count_ascii.py <file_path>")
        sys.exit(1)

    file_path = sys.argv[1]
    ascii_count = count_ascii_text(file_path)

    if ascii_count is not None:
        print(f"Number of ASCII characters in {file_path}: {ascii_count}")
    else:
        print(f"File {file_path} not found.")
