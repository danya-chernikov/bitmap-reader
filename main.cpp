#include <iostream>
#include <string>
#include <fstream>

#include <cstdlib>

#include "bitmap.h"

int main()
{
	std::string	src_img_path;
	std::cout << "Enter input BMP file path: ";
	std::getline(std::cin, src_img_path);

	std::ifstream src_file(src_img_path, std::ios::in | std::ios::binary);
	if (!src_file.is_open() || src_file.fail())
	{
		std::cerr << "Unable to open the source image file\n";
		exit(1);
	}

	/* Let's read the headers first */
	bitmap	image;
	src_file.read(reinterpret_cast<char *>(&image), sizeof (image));
	if (src_file.fail())
	{
		std::cerr << "Unable to read from the source image file\n";
		exit(2);
	}

	if (!is_file_bitmap(image, src_img_path))
	{
		std::cerr << "It does not seem like this file is in BMP format\n";
		exit(3);
	}

	print_header(image);

	int8_t	width = image.info_header.width;
	int8_t	height = image.info_header.height;

	rgba **data = new rgba*[height];
	for (int8_t i = 0; i < height; ++i)
		data[i] = new rgba[width];

	if (!read_data(src_file, data, image))
	{
		std::cerr << "Unable to read from the source image file\n";
		exit(2);
	}

	std::cout << std::endl;
	for (int8_t i = height - 1; i >= 0; --i)
	{
		for (int8_t q = 0; q < width; ++q)
		{
			if (!data[i][q].blue && !data[i][q].green && !data[i][q].red)
				std::cout << "\033[32m" << 'x' <<"\033[0m";
			else
				std::cout << 'o';
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	for (int8_t i = 0; i < height; ++i)
		delete [] data [i];
	delete [] data;
	
	src_file.close();

	return 0;
}
