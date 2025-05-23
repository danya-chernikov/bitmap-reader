#include <iostream>
#include <fstream>
#include <string>

#include <cstdlib>

#include "bitmap.h"

int main()
{
	std::string	src_img_path;
	std::cout << "Enter input BMP file path: ";
	std::cin >> src_img_path;

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
	
	src_file.close();

	return 0;
}
