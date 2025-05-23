#!/bin/python

from PIL import Image
import sys

# Open the 32-bit BMP (which is usually in RGBA format)
img = Image.open(sys.argv[1])

# Convert to 24-bit RGB (drops alpha)
rgb_img = img.convert("RGB")

# Save as 24-bit BMP
rgb_img.save(sys.argv[2], format="BMP")
