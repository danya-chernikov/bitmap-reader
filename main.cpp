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
		std::string	src_img_path;
		std::cout << "Enter input BMP file path: ";
		std::getline (std::cin, src_img_path);

		Bitmap	image (src_img_path);

		/* Draw a point */
		/*image.draw_point((point){0,0}, pixel_color::BLACK);
		image.draw_point((point){0,1}, pixel_color::BLACK);
		image.draw_point((point){0,2}, pixel_color::BLACK);*/
		image.draw_line((point){5,4}, (point){25,14}, pixel_color::BLACK);
		image.draw_line((point){10,15}, (point){10,25}, pixel_color::BLACK);
		image.draw_line((point){10,15}, (point){10,25}, pixel_color::BLACK);
		image.draw_line((point){0,0}, (point){0,14}, pixel_color::BLACK);
		image.draw_line((point){0,0}, (point){14,0}, pixel_color::BLACK);
		image.draw_line((point){32,10}, (point){60,12}, pixel_color::BLACK);

		image.print_header();
		image.display();

	}
	catch (const std::runtime_error &err)
	{
		std::cerr << "Error: " << err.what() << '\n';
		exit (1);
	}
	catch (const char *err)
	{
		std::cerr << "Eitmap error: " << err << '\n';
		exit (1);
	}
	catch (...)
	{
		std::cerr << "Unkown error occured" << '\n';
		exit (1);
	}
	return 0;
}
