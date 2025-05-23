#!/bin/python

from PIL import Image
import sys

# Open the image and convert to grayscale
img = Image.open(sys.argv[1]).convert("L")

# Apply threshold: pixels >128 become white, others become black
bw = img.point(lambda x: 255 if x > 128 else 0, '1') # '1' = 1-bit mode

# Save as 1-bit BMP
bw.save(sys.argv[2], format="BMP")
