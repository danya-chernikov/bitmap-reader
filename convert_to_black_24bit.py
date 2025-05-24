#!/bin/python

from PIL import Image
import sys

def isolate_white_pixels(input_path, output_path):
    img = Image.open(input_path)

    if img.mode != "RGB":
        raise ValueError("The image must be a 24-bit BMP (RGB)")

    result = Image.new("RGB", img.size)

    for y in range(img.height):
        for x in range(img.width):
            r, g, b = img.getpixel((x, y))
            if (r, g, b) == (255, 255, 255):
                result.putpixel((x, y), (255, 255, 255))
            else:
                result.putpixel((x, y), (0, 0, 0))

    result.save(output_path, "BMP")
    print(f"Processed image saved as {output_path}")

# Example usage
isolate_white_pixels(sys.argv[1], sys.argv[2])
