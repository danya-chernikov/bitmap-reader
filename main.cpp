/* I've created a set of test images (see the images/ directory),
 * but to better demonstrate the functionality, I suggest using
 * images/hxh_logo_32bit.bmp (or its 24-bit version). While the
 * program works with any image, this particular one makes the
 * cross appear more natural and logical, as it's part of the
 * logo from my favorite anime */

#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <cstdlib>

#include "bitmap.h"

/* Draws a square cross with its center at center and in the specified color */
void draw_cross(Bitmap &img, point center, uint32_t side_len, pixel_color color)
{
	uint32_t x, y;
	x = center.x;
	y = center.y;

	img.draw_line(
			(point){x + side_len/2, y - side_len/2},
			(point){x - side_len/2, y + side_len/2},		  
				   color);
	img.display();
	img.draw_line((point){x - side_len/2, y - side_len/2},
				  (point){x + side_len/2, y + side_len/2},
				   color);

}

int main()
{
	try
	{
		std::string	in_img_path;
		std::cout << "Enter input BMP file path: ";
		std::getline (std::cin, in_img_path);

		Bitmap image (in_img_path);

		image.display();

		// Let's say the program draws a square-shaped
		// cross at the center of the image
		uint32_t width, height;
		width = image.get_width();
		height = image.get_height();
		draw_cross(image, (point){width/2, height/2}, 10, pixel_color::BLACK);

		image.display();

		std::string out_img_path;
		std::cout << "Enter output BMP file path: ";
		std::getline(std::cin, out_img_path);

		image.save_as(out_img_path);
	}
	catch (const std::runtime_error &err)
	{
		std::cerr << "Error: " << err.what() << '\n';
		exit (1);
	}
	catch (...)
	{
		std::cerr << "Unkown error occured" << '\n';
		exit (1);
	}
	return 0;
}
