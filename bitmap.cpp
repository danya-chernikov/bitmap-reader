#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>

#include "bitmap.h"

Bitmap::Bitmap (const std::string &file_path)
{
	this->file_path = file_path;

	/* Trying to open the file provided */
	file.open (file_path, std::ios::in | std::ios::binary);
	if (!file.is_open() || file.fail())
		throw std::runtime_error (OPEN_FILE_ERR);

	/* Read the bitmap headers first */
	file.read (reinterpret_cast<char *>(&header), sizeof (header));
	if (file.fail())
		throw std::runtime_error (READ_FILE_ERR);

	/* Determine if the bitmap image has the correct structure */
	if (!is_file_bitmap ())
		throw std::runtime_error (FORMAT_BMP_ERR);

	width = header.info.width;
	height = header.info.height;

	if (width > MAX_WIDTH || height > MAX_HEIGHT)
		throw std::runtime_error (DIMENSIONS_BMP_ERR);

	data = std::vector<std::vector<rgba> > (height, std::vector<rgba>(width));

	/* Read bitmap data */
	int	res = read_data();
	if (res == 1)
		throw std::runtime_error (DEPTH_BMP_ERR);
	else if (res == 2)
		throw std::runtime_error (READ_FILE_ERR);
}

Bitmap::~Bitmap()
{
	file.close();
}

void Bitmap::display()
{
	std::cout << "width: " << width << '\n' << "height: " << height << '\n';

	/* Display the read image */
	std::cout << '\n';
	for (uint32_t i = height - 1; i >= 0; --i)
	{
		for (uint32_t q = 0; q < width; ++q)
		{
			if (!data[i][q].blue && !data[i][q].green && !data[i][q].red)
				std::cout << "\033[32m" << 'x' <<"\033[0m";
			else
				std::cout << 'o';
		}
		std::cout << '\n';
	}
	std::cout << '\n';

	std::cout << '\n';
	for (uint32_t i = 0; i < height; ++i)
	{
		for (uint32_t q = 0; q < width; ++q)
		{
			if (!data[i][q].blue && !data[i][q].green && !data[i][q].red)
				std::cout << "\033[32m" << 'x' <<"\033[0m";
			else
				std::cout << 'o';
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}

void Bitmap::print_header()
{
	std::cout << '\n' << std::hex;
	std::cout << "Signature:\t\t"			<<  header.file.signature		<< '\n';
	std::cout << "File size:\t\t"			<<  header.file.file_size		<< '\n';
	std::cout << "Reserved 1:\t\t"			<<  header.file.reserved1		<< '\n';
	std::cout << "Reserved 2:\t\t"			<<  header.file.reserved2		<< '\n';
	std::cout << "Data offset:\t\t"			<<  header.file.data_offset		<< '\n';
	std::cout << "Size:\t\t\t"				<<  header.info.size			<< '\n';
	std::cout << "Width:\t\t\t"				<<  header.info.width			<< '\n';
	std::cout << "Height:\t\t\t"			<<  header.info.height			<< '\n';
	std::cout << "Planes:\t\t\t"			<<  header.info.planes			<< '\n';
	std::cout << "Bits per pixel:\t\t"		<<  header.info.bits_per_pix	<< '\n';
	std::cout << "Compression:\t\t"			<<  header.info.compression		<< '\n';
	std::cout << "Image size:\t\t"			<<  header.info.img_size		<< '\n';
	std::cout << "X pixels per meter:\t"	<<  header.info.x_pix_per_mtr	<< '\n';
	std::cout << "Y pixels per meter:\t"	<<  header.info.y_pix_per_mtr	<< '\n';
	std::cout << "Colors used:\t\t"			<<  header.info.colors_used		<< '\n';
	std::cout << "Important colors:\t"		<<  header.info.important_colors<< '\n';
	std::cout << '\n' << std::dec;
}

/* Coordinates are measured from the bottom-left corner of
 * the image (based on its normal, unflipped orientation, as
 * displayed in standard image viewers by default) */
int	Bitmap::draw_point(point p, pixel_color color)
{
	if (p.x < 0 || p.x > width - 1 || p.y < 0 || p.y > height - 1)
		return 0;

	data[p.y][p.x].blue = static_cast<uint8_t>(color);
	data[p.y][p.x].green = static_cast<uint8_t>(color);
	data[p.y][p.x].red = static_cast<uint8_t>(color);

	return 1;
}

/* Having two points P1(x1; y1) and P2(x2; y2)
 * the equation of straight line with these points is:
 * (y - y1)/(y2 - y1) = (x - x1)/(x2 - x1)
 * Let's derive the y
 * (y - y1)(x2 - x1) = (y2 - y1)(x - x1)
 * y = ( y2(x - x1) -y1(x - x1) + y1(x2 - x1) )/(x2 - x1)
 * y = (y2 - y1)(x - x1)/(x2 - x1) + y1
 * */
int	Bitmap::draw_line(point p1, point p2, pixel_color color)
{
	if (p1.x < 0 || p1.x > width - 1 || p1.y < 0 || p1.y > height - 1)
		return 0;
	if (p2.x < 0 || p2.x > width - 1 || p2.y < 0 || p2.y > height - 1)
		return 0;

	if (p1.x == p2.x)
	{
		/* Determine mutual positioning of the points on the ordinate */
		uint32_t lpy = std::min(p1.y, p2.y); // lower point
		uint32_t upy = std::max(p1.y, p2.y); // upper point

		for (uint32_t y = lpy; y < upy; ++y)
		{
			draw_point((point){p1.x, y}, color);
		}
	}
	else
	{
		/* Determine mutual positioning of the points on the absiss */
		uint32_t lpx = std::min(p1.x, p2.x); // left point
		uint32_t rpx = std::max(p1.x, p2.x); // right point

		for (uint32_t x = lpx; x <= rpx; ++x)
		{
			double slope = static_cast<double>(p2.y - p1.y) / (p2.x - p1.x);
			uint32_t y = static_cast<uint32_t>(slope * (x - p1.x) + p1.y);
			draw_point((point){x, y}, color);
		}
	}

	return 1;
}

/* On success returns 0 */
int Bitmap::read_data()
{	
	uint32_t	data_offset = header.file.data_offset;
	uint16_t	bits_per_pix = header.info.bits_per_pix;
	uint32_t	bytes_read = 0;

	char stuffer[3]; // To store possible padding bytes

	// How many bytes to skip in the end of each row
	uint32_t	bytes_to_skip = 0; // by default skipping 0 bytes

	// How many bytes will read each read() call
	size_t	bytes_to_read;

	// Bitmaps of this bit depth are not supported
	if (bits_per_pix != 0x18 && bits_per_pix != 0x20)
		return 1; // error

	bytes_to_read = bits_per_pix / 8;

	std::cout << "Bytes to read: " << bytes_to_read << std::endl;

	// If there are some padding bytes
	if ((width * bytes_to_read) % 4 != 0)
	{
		// We need to skip them
		bytes_to_skip = 4 - ((width * bytes_to_read) % 4);
	}

	std::cout << "Bytes to skip: " << bytes_to_skip << std::endl;

	/* Let's read file data raw by raw */
	file.seekg(data_offset);
	for (uint32_t i = 0; i < height; ++i)
	{
		for (uint32_t q = 0; q < width && !file.eof(); ++q)
		{
			file.read(reinterpret_cast<char *>(&data[i][q]), bytes_to_read);
			/*if (file.fail())
				return 2; // error */
			bytes_read += file.gcount();
		}
		if (bytes_to_skip)
			file.read(&stuffer[0], bytes_to_skip);
	}

	std::cout << "Bytes read: " << bytes_read << std::endl;

	return 0;
}

/* Performs minimal checking to
 * ensure that the bitmap file
 * structure is correct */
bool Bitmap::is_file_bitmap()
{
	const std::filesystem::path	path = file_path;

	// The file size in the header does not match
	// the actual file size
	if (static_cast<uintmax_t>(header.file.file_size) !=
			std::filesystem::file_size(path))
		return false;

	// The bitmap signature in the header is incorrect
	if (header.file.signature != BM_SIGNATURE)
		return false;

	return true;
}
