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

int main()
{
	try
	{
		std::string	in_img_path;
		std::cout << "Enter input BMP file path: ";
		std::getline (std::cin, in_img_path);

		Bitmap image (in_img_path);

		image.display();

		// Let's say the program draws a cross in the image's center
		uint32_t	width, height;
		point		p1, p2;

		width = image.get_width();
		height = image.get_height();

		p1.x = width/2 - width/8;
		p1.y = height/2 - height/10;
		p2.x = width/2 + width/12;
		p2.y = height/2 + height/12;

		image.draw_line(p1, p2, pixel_color::BLACK);

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
