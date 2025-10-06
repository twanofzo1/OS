from PIL import Image
import sys

char_width = 16
char_height = 24


def font_to_bitmap(input_png, output_file):
    img = Image.open(input_png).convert('L')
    width, height = img.size
    pixels = img.load()
    chars_per_row = width // char_width
    rows_of_chars = height // char_height

    with open(output_file, 'w') as f:
        f.write("#pragma once\n")
        f.write("#include <stdint.h>\n\n")
        f.write(f"#define CHAR_TYPE uint{char_width}_t\n")
        f.write(f"#define CHAR_WIDTH {char_width }\n")
        f.write(f"#define CHAR_HEIGHT {char_height}\n\n")
        
        f.write(f"uint{char_width}_t font[{chars_per_row}][{char_height}] =")
        f.write(" {\n")
        for row in range(rows_of_chars):
            for col in range(chars_per_row):
                f.write("{\n")
                for y in range(char_height):
                    f.write("\t0b")
                    for x in range(char_width):
                        px = pixels[col * char_width + x, row * char_height + y]
                        val = 0 if px < 128 else 1  # Threshold to determine if pixel is set
                        f.write(str(val))
                    f.write(',\n')
                f.write("},\n")

        f.write("};\n")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python font_to_bitmap.py <input.png> <output.h>")
        sys.exit(1)
    font_to_bitmap(sys.argv[1], sys.argv[2])