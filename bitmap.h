#ifndef BITMAP_H
#define BITMAP_H

#include <vector>

#include <cstdint>

using u_char = unsigned char;

/* Structures definitions */
#pragma pack(push, 1)
struct	file_header
{
	uint16_t	signature; // Always 'BM'
	uint32_t	file_size; // The whole file size
	uint16_t	reserved1; // Always 0
	uint16_t	reserved2; // Always 0
	uint32_t	data_offset;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct	info_header
{
	uint32_t	size; // Size of info header (Is it always 40?)
	uint32_t	width;
	uint32_t	height;
	uint16_t	planes;
	uint16_t	bits_per_pix;
	uint32_t	compression;
	uint32_t	img_size;
	int32_t		x_pix_per_mtr; // For printers
	int32_t		y_pix_per_mtr;
	uint32_t	colors_used;
	uint32_t	important_colors;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct	bitmap_header
{
	file_header	file;
	info_header	info;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct	rgba
{
	uint8_t	blue;
	uint8_t	green;
	uint8_t	red;
	uint8_t	reserved;
};
#pragma pack(pop)

struct	point
{
	uint32_t x;
	uint32_t y;
};

enum class pixel_color : uint8_t
{
	BLACK=0, WHITE=255
};

/* Constants definitions */
constexpr uint16_t	BM_SIGNATURE = 0x4d42;
constexpr uint32_t	FILE_HEADER_SIZE = 14;
constexpr uint32_t	INFO_HEADER_SIZE = 40;
constexpr uint32_t	MAX_WIDTH = 1000;
constexpr uint32_t	MAX_HEIGHT = 1000;

constexpr const char *OPEN_FILE_ERR = "Unable to open the source image file";
constexpr const char *READ_FILE_ERR = "Unable to read from the source image file";
constexpr const char *WRITE_FILE_ERR = "Unable to write into the file";
constexpr const char *FORMAT_BMP_ERR = "It does not seem like this file is in BMP format";
constexpr const char *DIMENSIONS_BMP_ERR = "The image is way too big";


/* Bitmap class */
class Bitmap
{
public:
	Bitmap (const std::string &file_path);
	~Bitmap();

	void	display();
	void	print_header();
	int		draw_point(point p, pixel_color color);
	int		draw_line(point p1, point p2, pixel_color color);
	int		save_as(const std::string &file_path);

private:
	/* Varialbe-members */
	std::string		file_path;
	std::ifstream	file;
	bitmap_header	header;

	std::vector <std::vector<rgba> > data;

	std::vector <uint8_t> color_table;

	size_t		color_table_size;
	size_t		data_size;
	uint32_t	bits_per_pix;
	uint32_t	bytes_to_skip; // How many bytes to skip in the end of each row

	/* Most useful and oft-used header fields */
	uint32_t	width;
	uint32_t	height;

	/* Functions-members */
	int		read_data();
	bool	is_file_bitmap();
};

#endif
