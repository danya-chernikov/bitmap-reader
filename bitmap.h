#ifndef BITMAP_H
#define BITMAP_H

#include <iostream>
#include <cstdint>

/* Structures definitions */
#pragma pack(push, 1)
struct	file_header
{
	int16_t	signature; // Always 'BM'
	int32_t	file_size; // The whole file size
	int16_t	reserved1; // Always 0
	int16_t	reserved2; // Always 0
	int32_t	data_offset;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct	info_header
{
	int32_t	size; // Size of info header (Is it always 40?)
	int32_t	width;
	int32_t	height;
	int16_t	planes;
	int16_t	bits_per_pix;
	int32_t	compression;
	int32_t	img_size;
	int32_t	x_pix_per_mtr; // For printers
	int32_t	y_pix_per_mtr;
	int32_t	colors_used;
	int32_t	important_colors;
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
	int8_t	blue;
	int8_t	green;
	int8_t	red;
	int8_t	reserved;
};
#pragma pack(pop)

enum class pixel_color
{
	BLACK, WHITE
};


/* Constants definitions */
constexpr int16_t	BM_SIGNATURE = 0x4d42;
constexpr int32_t	MAX_WIDTH = 1000;
constexpr int32_t	MAX_HEIGHT = 1000;


/* Bitmap class */
class Bitmap
{
public:
	Bitmap();
	~Bitmap();

	bool	is_file_bitmap(bitmap &img, const std::string &file);
	int		read_data(std::ifstream &file, rgba **data, bitmap &img);
	int		draw_point(rgba **data, int x, int y, pixel_color color);
	void	display(rgba **data);
	void	print_header();

private:
	std::string		file_path;
	std::ifstream	file;
	bitmap_header	header;

	/* Most useful and oft-used header fields */
	int32_t	width;
	int32_t	height;

	bool	is_file_bitmap(bitmap &img, const std::string &file);
};


#endif
