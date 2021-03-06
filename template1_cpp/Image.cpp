#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <iostream>
#include <unistd.h>


Image::Image(const std::string &a_path)
{
  if((data = (Pixel*)stbi_load(a_path.c_str(), &width, &height, &channels, 0)) != nullptr)
  {
    size = width * height * channels;
  }
  else
  {
    std::cout << "ERROR\n";
    exit(-1);
  }

}

Image::Image(int a_width, int a_height, int a_channels)
{
  data = new Pixel[a_width * a_height ]{};

  if(data != nullptr)
  {
    width = a_width;
    height = a_height;
    size = a_width * a_height * a_channels;
    channels = a_channels;
    self_allocated = true;
  }
}


int Image::Save(const std::string &a_path)
{
  auto extPos = a_path.find_last_of('.');
  if(a_path.substr(extPos, std::string::npos) == ".png" || a_path.substr(extPos, std::string::npos) == ".PNG")
  {
    stbi_write_png(a_path.c_str(), width, height, channels, data, width * channels);
  }
  else if(a_path.substr(extPos, std::string::npos) == ".jpg" || a_path.substr(extPos, std::string::npos) == ".JPG" ||
          a_path.substr(extPos, std::string::npos) == ".jpeg" || a_path.substr(extPos, std::string::npos) == ".JPEG")
  {
    stbi_write_jpg(a_path.c_str(), width, height, channels, data, 100);
  }
  else
  {
    std::cerr << "Unknown file extension: " << a_path.substr(extPos, std::string::npos) << "in file name" << a_path << "\n";
    return 1;
  }

  return 0;
}

Image::~Image()
{
  if(self_allocated)
    delete [] data;
  else
  {
    stbi_image_free(data);
  }
}

Image::Image(const Image &screen) {
  width = screen.width;
  height = screen.height;
  channels = screen.channels;
	size = screen.size;
  data = screen.data;
  // data->r = screen.data->r;
  // data->g = screen.data->g;
  // data->b = screen.data->b;
  // data->a = screen.data->a;
  self_allocated = 0;
}

void Image::drawTile(Image image,int x, int y){
  for (int i = 0; i < tileSize; i++){
        for (int j = 0; j < tileSize; j++){
            PutPixel(x + i, y + j, image.GetPixel(i, j));
        }
    }
}

void Image::drawChechik(Image image,int x, int y){
  for (int i = 0; i < tileSize; i++){
        for (int j = 0; j < tileSize; j++){
            PutPixel(x + i, y + j, image.GetPixel(i, j));
        }
    }
}

int Image::DrawPicForSeconds(Image image, int x, int y){
	for (int i = 0; i < 600; i++){
		for (int j = 0; j < 200; j++) {
			PutPixel(x + i, y + j, image.GetPixel(i, j));
		}
	}
	return 1;
}
