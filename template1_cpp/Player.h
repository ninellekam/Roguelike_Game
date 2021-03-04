#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "Image.h"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <iostream>
#include <fcntl.h>

struct Point
{
  int x;
  int y;
};

enum class MovementDir
{
  UP,
  DOWN,
  LEFT,
  RIGHT
};

struct Player
{
  char **map;
  explicit Player(Point pos = {.x = 10, .y = 10}) :
                 coords(pos), old_coords(coords) {};
  bool Moved() const;
  void ProcessInput(MovementDir dir, std::vector<std::vector<int>> map);
  void Draw(Image &screen, Image &cur);
  void Collision(std::vector<std::vector<int>> map);

  const std::vector<std::string> read_map_strings(const std::string& filename);
  void get_text(const std::vector<std::string>& map_strings, Image &screen);

private:
  Point coords {.x = 10, .y = 10};
  Point old_coords {.x = 10, .y = 10};
  Pixel color {.r = 255, .g = 255, .b = 0, .a = 255};
  Pixel color1 {.r = 0, .g = 0, .b = 255, .a = 255};
  Pixel color2 {.r = 255, .g = 0, .b = 0, .a = 255};
  int move_speed = 4;

};

#endif //MAIN_PLAYER_H
