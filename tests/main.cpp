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

		image.print_header();

		/* Draw a point */
		/*image.draw_point((point){0,0}, pixel_color::BLACK);
		image.draw_point((point){0,1}, pixel_color::BLACK);
		image.draw_point((point){0,2}, pixel_color::BLACK);*/
		/*image.draw_line((point){5,4}, (point){25,14}, pixel_color::BLACK);
		image.draw_line((point){10,15}, (point){10,25}, pixel_color::BLACK);
		image.draw_line((point){10,15}, (point){10,25}, pixel_color::BLACK);
		image.draw_line((point){0,0}, (point){0,14}, pixel_color::BLACK);
		image.draw_line((point){0,0}, (point){14,0}, pixel_color::BLACK);
		image.draw_line((point){32,10}, (point){60,12}, pixel_color::BLACK);*/

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
	catch (const char *err)
	{
		std::cerr << "Error: " << err << '\n';
		exit (1);
	}
	catch (...)
	{
		std::cerr << "Unkown error occured" << '\n';
		exit (1);
	}
	return 0;
}
