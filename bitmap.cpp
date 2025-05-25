#include "bitmap.h"

#include <fstream>
#include <sstream>
#include <filesystem>

Bitmap::Bitmap (std::string img_path)
{
	this->img_path = img_path;

	/* Trying to open the file provided */
	file.open (img_path, std::ios::in | std::ios::binary);
	if (!file.is_open() || file.fail())
	{
		throw std::runtime_error ("Unable to open the source image file");
	}

	/* Read the bitmap headers first */
	file.read (reinterpret_cast<char *>(&header), sizeof (header));
	if (file.fail())
	{
		throw std::runtime_error ("Unable to read from the source image file");
	}

	/* Determine if the bitmap image has the correct structure */
	if (!is_file_bitmap (header, img_path))
	{
		throw std::runtime_error ("It does not seem like this file is in BMP format");
	}

	width = image.info_header.width;
	height = image.info_header.height;

	if (width > MAX_WIDTH || height > MAX_HEIGHT)
	{
		throw std::runtime_error ("The image is way too big");
	}
}

Bitmap::~Bitmap()
{
	for (int32_t i = 0; i < height; ++i)
		delete [] data [i];
	delete [] data;

	file.close();
}

void Bitmap::display()
{

	/* Display the read image */
	std::cout << std::endl;
	for (int32_t i = height - 1; i >= 0; --i)
	{
		for (int32_t q = 0; q < width; ++q)
		{
			if (!data[i][q].blue && !data[i][q].green && !data[i][q].red)
				std::cout << "\033[32m" << 'x' <<"\033[0m";
			else
				std::cout << 'o';
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

/* Performs minimal checking to ensure that the bitmap file structure is correct */
bool Bitmap::is_file_bitmap(bitmap &img, const std::string &file)
{
	const std::filesystem::path	path = file;

	// The file size in the header does not match the actual file size
	if (static_cast<uintmax_t>(img.file_header.file_size) != std::filesystem::file_size(path))
		return false;

	// The bitmap signature in the header is incorrect
	if (img.file_header.signature != BM_SIGNATURE)
		return false;

	return true;
}

/* On success returns 0 */
int read_data(std::ifstream &file, rgba **data, bitmap &img)
{	
	int32_t	width = img.info_header.width;
	int32_t	height = img.info_header.height;
	int32_t	data_offset = img.file_header.data_offset;
	int16_t	bits_per_pix = img.info_header.bits_per_pix;
	int32_t	bytes_read = 0;

	char stuffer[3]; // To store possible padding bytes

	// How many bytes to skip in the end of each row
	int32_t	bytes_to_skip = 0; // by default skipping 0 bytes

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
	for (int32_t i = 0; i < height; ++i)
	{
		for (int32_t q = 0; q < width && !file.eof(); ++q)
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

/* Coordinates are measured from the bottom-left corner of
 * the image (based on its normal, unflipped orientation, as
 * displayed in standard image viewers by default) */
int	draw_point(rgba **data, int x, int y, pixel_color color)
{

	return 1;
}

void Bitmap::print_header()
{
	std::cout << std::endl;
	std::cout << "Signature:\t\t"			<< std::hex << header.file.signature		<< std::endl;
	std::cout << "File size:\t\t"			<< std::hex << header.file.file_size		<< std::endl;
	std::cout << "Reserved 1:\t\t"			<< std::hex << header.file.reserved1		<< std::endl;
	std::cout << "Reserved 2:\t\t"			<< std::hex << header.file.reserved2		<< std::endl;
	std::cout << "Data offset:\t\t"			<< std::hex << header.file.data_offset		<< std::endl;
	std::cout << "Size:\t\t\t"				<< std::hex << header.info.size				<< std::endl;
	std::cout << "Width:\t\t\t"				<< std::hex << header.info.width			<< std::endl;
	std::cout << "Height:\t\t\t"			<< std::hex << header.info.height			<< std::endl;
	std::cout << "Planes:\t\t\t"			<< std::hex << header.info.planes			<< std::endl;
	std::cout << "Bits per pixel:\t\t"		<< std::hex << header.info.bits_per_pix		<< std::endl;
	std::cout << "Compression:\t\t"			<< std::hex << header.info.compression		<< std::endl;
	std::cout << "Image size:\t\t"			<< std::hex << header.info.img_size			<< std::endl;
	std::cout << "X pixels per meter:\t"	<< std::hex << header.info.x_pix_per_mtr	<< std::endl;
	std::cout << "Y pixels per meter:\t"	<< std::hex << header.info.y_pix_per_mtr	<< std::endl;
	std::cout << "Colors used:\t\t"			<< std::hex << header.info.colors_used		<< std::endl;
	std::cout << "Important colors:\t"		<< std::hex << header.info.important_colors	<< std::endl;
	std::cout << std::endl;
}
