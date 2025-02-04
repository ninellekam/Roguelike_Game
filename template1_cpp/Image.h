#ifndef MAIN_IMAGE_H
#define MAIN_IMAGE_H
#include <string>

constexpr	int tileSize = 32;
constexpr	int WINDOW_WIDTH = 1024, WINDOW_HEIGHT = 800;

struct		Pixel
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};

constexpr Pixel backgroundColor{0, 0, 0, 0};

struct Image
{
	explicit Image(const std::string &a_path);
	Image(int a_width, int a_height, int a_channels);
	Image(const Image &screen);

	int			Save(const std::string &a_path);
	int			Width()			const { return width; }
	int			Height()		const { return height; }
	int			Channels()	const { return channels; }
	size_t	Size()		const { return size; }
	Pixel*	Data()					{ return  data; }

	Pixel		GetPixel(int x, int y) { return data[width * y + x];}
	void		PutPixel(int x, int y, const Pixel &pix) { data[width * y + x] = pix; }
	void		drawTile(Image image,int x, int y);
	void		drawTraps(Image image,int x, int y);
	int			DrawPicForSeconds(Image image,int x, int y);
	void		drawChechik(Image image,int x, int y);
	int			DrawBonus();
	int			DrawBonus_next();

	~Image();

private:
	int			width = -1;
	int			height = -1;
	int			channels = 3;
	size_t	size = 0;
	Pixel		*data = nullptr;
	bool		self_allocated = false;
};

#endif
