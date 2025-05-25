#ifndef BITMAP_H
#define BITMAP_H

#include <vector>

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

struct	point
{
	int32_t	x;
	int32_t	y;
};

enum class pixel_color
{
	BLACK, WHITE
};


/* Constants definitions */
constexpr int16_t	BM_SIGNATURE = 0x4d42;
constexpr int32_t	MAX_WIDTH = 1000;
constexpr int32_t	MAX_HEIGHT = 1000;

constexpr const char *OPEN_FILE_ERR = "Unable to open the source image file";
constexpr const char *READ_FILE_ERR = "Unable to read from the source image file";
constexpr const char *FORMAT_BMP_ERR = "It does not seem like this file is in BMP format";
constexpr const char *DIMENSIONS_BMP_ERR = "The image is way too big";
constexpr const char *DEPTH_BMP_ERR = "Bitmaps of this bit depth are not supported";


/* Bitmap class */
class Bitmap
{
public:
	Bitmap (const std::string &file_path);
	~Bitmap();

	void	display();
	void	print_header();
	int		draw_point(point p, pixel_color color);

private:
	/* Varialbe-members */
	std::string		file_path;
	std::ifstream	file;
	bitmap_header	header;

	std::vector <std::vector<rgba> > data; 

	/* Most useful and oft-used header fields */
	int32_t	width;
	int32_t	height;

	/* Functions-members */
	int		read_data();
	bool	is_file_bitmap();
};


#endif
