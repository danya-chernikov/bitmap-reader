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
	std::getline(std::cin, src_img_path);

	try
	{
		Bitmap	image(src_img_path);
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

	image.print_header();

	rgba **data = new rgba*[height];
	for (int32_t i = 0; i < height; ++i)
		data[i] = new rgba[width];

	/* Read bitmap data */
	int	res = read_data(src_file, data, image);
	if (res == 1)
	{
		std::cerr << "Bitmaps of this bit depth are not supported\n";
		exit(3);
	}
	else if (res == 2)
	{
		std::cerr << "Unable to read from the source image file\n";
		exit(2);
	}


	image.display();

	/* Draw a point */

	return 0;
}
