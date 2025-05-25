#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>

#include <cstdlib>

#include "bitmap.h"

int main()
{
	std::string	src_img_path;
	std::cout << "Enter input BMP file path: ";
	std::getline (std::cin, src_img_path);

	try
	{
		Bitmap	image (src_img_path);
		image.print_header();
		image.display();
		/* Draw a point */
	}
	catch (const std::runtime_error &err)
	{
		std::cerr << "Bitmap error: " << err.what() << std::endl;
		exit (1);
	}
	catch (const std::exception &err)
	{
		std::cerr << "General error: " << err.what() << std::endl;
		exit(1);
	}
	catch (...)
	{
		std::cerr << "Unkown error occured" << std::endl;
		exit (1);
	}
	return 0;
}
