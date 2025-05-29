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
	uint32_t	size; // Size of info header
	uint32_t	width;
	uint32_t	height;
	uint16_t	planes; // Always 1
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
constexpr uint16_t		BM_SIGNATURE		= 0x4d42;
constexpr uint32_t		FILE_HEADER_SIZE	= 14;
constexpr uint32_t		INFO_HEADER_SIZE	= 40;
constexpr uint32_t		MAX_WIDTH			= 1000;
constexpr uint32_t		MAX_HEIGHT			= 1000;

constexpr const char	*OPEN_FILE_ERR		= "Unable to open the source image file";
constexpr const char	*READ_FILE_ERR		= "Unable to read from the source image file";
constexpr const char	*WRITE_FILE_ERR		= "Unable to write into the file";
constexpr const char	*FORMAT_BMP_ERR		= "It doesn't seem like this file is in BMP format";
constexpr const char	*DIMENSIONS_BMP_ERR	= "The image is way too big";


/* Bitmap class */
class Bitmap
{
public:
	Bitmap (const std::string &file_path);
	~Bitmap();

	/* Interface functions */
	bool			draw_point(point p, pixel_color color);
	bool			draw_line(point p1, point p2, pixel_color color);
	void			save_as(const std::string &file_path);
	void			display();

	/* Getters */
	uint32_t		get_width() const;
	uint32_t		get_height() const;

private:
	/* Auxiliary member functions */
	size_t			read_data();
	bool			is_file_bitmap();

	/* Just member variables */
	std::ifstream	file;
	std::string		file_path;
	bitmap_header	header;
	size_t			color_table_size;
	uint32_t		bytes_to_skip; // How many bytes to skip in the end of each row

	/* Most useful and oft-used header fields */
	uint32_t		width;
	uint32_t		height;
	uint32_t		bits_per_pix;

	/* Containers to store bitmap's content */
	std::vector <std::vector<rgba> >	data; // pixel color data
	std::vector <uint8_t>				color_table;
};

#endif
