#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>

#include "bitmap.h"

Bitmap::Bitmap (const std::string &file_path)
{
	bytes_to_skip = 0;
	this->file_path = file_path;

	// Trying to open the file provided
	file.open (file_path, std::ios::in | std::ios::binary);
	if (!file.is_open() || file.fail())
		throw std::runtime_error (OPEN_FILE_ERR);

	// Read the bitmap headers first
	file.read (reinterpret_cast<char *>(&header), sizeof (header));
	if (file.fail())
		throw std::runtime_error (READ_FILE_ERR);

	// Determine if the bitmap image has the correct structure
	if (!is_file_bitmap ())
		throw std::runtime_error (FORMAT_BMP_ERR);

	// Let's duplicate the most frequently used variables
	width = header.info.width;
	height = header.info.height;
	bits_per_pix = header.info.bits_per_pix;

	if (width > MAX_WIDTH || height > MAX_HEIGHT)
		throw std::runtime_error (DIMENSIONS_BMP_ERR);

	// Initialize the color's table vector
	color_table_size = header.file.data_offset - FILE_HEADER_SIZE - INFO_HEADER_SIZE;
	color_table = std::vector<uint8_t>(color_table_size);

	// Read color table
	file.read(reinterpret_cast<char *>(color_table.data()), color_table_size);

	// Initialize the vector of pixel data
	pixel_data = std::vector<std::vector<rgba> > (height, std::vector<rgba>(width));

	// Read bitmap pixel data
	size_t data_size = read_pixel_data();
	if (!data_size)
		throw std::runtime_error (FORMAT_BMP_ERR);
}

Bitmap::~Bitmap()
{
	file.close();
}

/* Returns the total number of
 * pixel data bytes read from
 * the file, or 0 on error */
size_t Bitmap::read_pixel_data()
{	
	uint32_t	data_offset = header.file.data_offset;
	size_t		bytes_read = 0;
	u_char		stuffer[3]; // To store possible padding bytes

	size_t		bytes_to_read = bits_per_pix / 8; // How many bytes will read each read() call

	// If there are some padding bytes
	if ((width * bytes_to_read) % 4 != 0)
	{
		// We need to skip them
		bytes_to_skip = 4 - ((width * bytes_to_read) % 4);
	}

	// Let's read file data raw by raw
	file.seekg(data_offset);
	for (uint32_t i = 0; i < height; ++i)
	{
		for (uint32_t q = 0; q < width && !file.eof(); ++q)
		{
			file.read(reinterpret_cast<char *>(&pixel_data[i][q]), bytes_to_read);
			if (file.fail() || file.bad())
				return 0;
			bytes_read += file.gcount();
		}
		if (bytes_to_skip)
			file.read(reinterpret_cast<char *>(&stuffer), bytes_to_skip);
	}

	return (bytes_read);
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

	// Bitmaps of this bit depth are not supported
	if (header.info.bits_per_pix != 24 && header.info.bits_per_pix != 32)
		return false; // error

	return true;
}

/* Draws a point at coordinates (p.x, p.y) using the color
 * selected by the caller. Returns 1 on success, or 0 if
 * the coordinates are out of the image bounds.
 * Coordinates are measured from the bottom-left corner of
 * the image (based on its normal, unflipped orientation,
 * as displayed in standard image viewers by default) */
bool Bitmap::draw_point(point p, pixel_color color)
{
	if (p.x < 0 || p.x > width - 1 || p.y < 0 || p.y > height - 1)
		return false;

	pixel_data[p.y][p.x].blue = static_cast<uint8_t>(color);
	pixel_data[p.y][p.x].green = static_cast<uint8_t>(color);
	pixel_data[p.y][p.x].red = static_cast<uint8_t>(color);

	return true;
}

/* Draws a line between two points, p1 and p2, using the
 * color selected by the caller. Returns 1 on success.
 * Returns 0 if the specified coordinates are out of the
 * image bounds */
bool Bitmap::draw_line(point p1, point p2, pixel_color color)
{
	if (p1.x < 0 || p1.x > width - 1 || p1.y < 0 || p1.y > height - 1)
		return false;
	if (p2.x < 0 || p2.x > width - 1 || p2.y < 0 || p2.y > height - 1)
		return false;

	int32_t x1 = p1.x, y1 = p1.y;
	int32_t x2 = p2.x, y2 = p2.y;

	int32_t dx = abs(x2 - x1), dy = -abs(y2 - y1);
	int32_t sx = x1 < x2 ? 1 : -1;
	int32_t sy = y1 < y2 ? 1 : -1;
	int32_t err = dx + dy;

	while (true)
	{
		draw_point((point){static_cast<uint32_t>(x1), static_cast<uint32_t>(y1)}, color);

		if (x1 == x2 && y1 == y2)
			break;

		int32_t e2 = 2 * err;
		if (e2 >= dy)
		{
			err += dy;
			x1 += sx;
		}
		if (e2 <= dx)
		{
			err += dx;
			y1 += sy;
		}
	}
	return true;
}

/* Writes a bitmap image into a new file */
void Bitmap::save_as(const std::string &file_path)
{	
	u_char	stuffer[3] = {0, 0, 0};

	std::ofstream file(file_path, std::ios::out | std::ios::binary);
	if (!file.is_open() || file.fail())
		throw std::runtime_error (OPEN_FILE_ERR);

	// Write the headers
	file.write(reinterpret_cast<char *>(&header), sizeof (header));

	// Write the color table
	file.write(reinterpret_cast<char *>(color_table.data()), color_table_size);

	size_t	bytes_to_write = bits_per_pix / 8;

	// Write pixel data
	for (uint32_t raw_ind = 0; raw_ind < height; ++raw_ind)
	{
		for (uint32_t p_ind = 0; p_ind < width; ++p_ind)
		{
			file.write(reinterpret_cast<char *>(&pixel_data[raw_ind][p_ind]), bytes_to_write);
			if (bytes_to_skip)
				file.write(reinterpret_cast<char *>(&stuffer), bytes_to_skip);
			if (file.bad() || file.fail())
				throw std::runtime_error(WRITE_FILE_ERR);
		}
	}

	file.close();
}

/* Displays the image
 * read from the bitmap
 * file on the console */
void Bitmap::display()
{
	std::cout << '\n';
	for (int32_t i = static_cast<int32_t>(height) - 1; i >= 0; --i)
	{
		for (int32_t q = 0; q < static_cast<int32_t>(width); ++q)
		{
			if (!pixel_data[i][q].blue && !pixel_data[i][q].green && !pixel_data[i][q].red)
				std::cout << "\033[32m" << 'x' <<"\033[0m";
			else
				std::cout << 'o';
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}

uint32_t Bitmap::get_width() const
{
	return width;
}

uint32_t Bitmap::get_height() const
{
	return height;
}
