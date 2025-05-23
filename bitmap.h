#ifndef BITMAP_H
#define BITMAP_H

#include <iostream>
#include <cstdint>

/* Structures definitions */

#pragma pack(push, 1)
struct	bitmap_file_header
{
	int16_t	signature; // Always 'BM'
	int32_t	file_size; // The whole file size
	int16_t	reserved1; // Always 0
	int16_t	reserved2; // Always 0
	int32_t	data_offset;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct	bitmap_info_header
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
struct	bitmap
{
	bitmap_file_header	file_header;
	bitmap_info_header	info_header;
};
#pragma pack(pop)


/* Constants definitions */

constexpr int16_t	BM_SIGNATURE = 0x4d42;


/* Functions definitions */

bool	is_file_bitmap(bitmap &img, const std::string &file);
void	print_header(bitmap &bmp);

#endif
