#include "Player.h"

bool Player::Moved() const
{
  if(coords.x == old_coords.x && coords.y == old_coords.y)
    return false;
  else
    return true;
}

void Player::ProcessInput(MovementDir dir, std::vector<std::vector<int>> map)
{
  int move_dist = move_speed * 1;
  switch(dir)
  {
    case MovementDir::UP:
      old_coords.y = coords.y;
      for (int i = 0; i < move_dist; i++) {
          if (map[(coords.y + 1) / 32 + 1][coords.x / 32] != 2 and map[(coords.y + 1) / 32 + 1][(coords.x + 32) / 32] != 2){
              coords.y++;
          }
      }
      break;
    case MovementDir::DOWN:
      old_coords.y = coords.y;
          for (int i = 0; i < move_dist; i++) {
              if (map[(coords.y - 33) / 32 + 1][coords.x / 32] != 2 and map[(coords.y - 33) / 32 + 1][(coords.x + 32) / 32] != 2){
                  coords.y--;
              }
          }
      break;
    case MovementDir::LEFT:
      old_coords.x = coords.x;
          for (int i = 0; i < move_dist; i++) {
              if (map[coords.y / 32][(coords.x - 1) / 32] != 2 and map[(coords.y + 32) / 32][(coords.x - 1) / 32] != 2){
                  coords.x--;
              }
          }
      break;
    case MovementDir::RIGHT:
      old_coords.x = coords.x;
          for (int i = 0; i < move_dist; i++) {
              if (map[coords.y / 32][(coords.x + 33) / 32] != 2 and map[(coords.y + 32)/ 32][(coords.x + 33) / 32] != 2){
                  coords.x++;
              }
          }
      break;
    default:
      break;
  }
}

void Player::Draw(Image &screen, Image &cur)
{
  if(Moved())
  {
    for(int x = old_coords.x; x <= old_coords.x + tileSize; ++x)
    {
      for(int y = old_coords.y; y <= old_coords.y + tileSize; ++y)
      {
        screen.PutPixel(x, y, cur.GetPixel(x, y));
      }
    }
    old_coords = coords;
  }

  for(int y = coords.y; y <= coords.y + tileSize; ++y)
  {
    for(int x = coords.x; x <= coords.x + tileSize; ++x)
    {
      screen.PutPixel(x, y, color);
    }
  }
}

void Player::Collision(std::vector<std::vector<int>> map) {
    int dx = coords.x - old_coords.x;
    int dy = coords.y - old_coords.y;
    for (int i = coords.y / 32; i < (coords.y + 64) / 32; i++) {
        for (int j = coords.x / 32; j < (coords.x + 64) / 32; j++) {
            if (map[i][j] == 2) {
                if (dx > 0) {
                    coords.x = old_coords.x;
                }
                if (dx < 0) {
                    coords.x = old_coords.x;
                }
                if (dy > 0) {
                    coords.y = old_coords.y;
                }
                if (dy < 0) {
                    coords.y = old_coords.y;
                }
            }
        }
    }
}

void    ft_scaled(Image &screen, int i, int j, Pixel color)
{
    int x = 0;
    int y = 0;
    i *= 15;
    j *= 15;
    while (x++ < 15)
    {
        y = 0;
        while (y++ < 15)
             screen.PutPixel(j + x, i + y, color);
    }
}

//------------------------------ my functions --------------------
void Player::get_text(const std::vector<std::string>& map_strings, Image &screen)
{
  for (int i = 0; i < map_strings.size(); i++)
    for (int j = 0; j < map_strings[i].size(); j++)
      {
        if (map_strings[i][j] == '#')
           ft_scaled(screen, i, j, color);
        if (map_strings[i][j] == '.')
           ft_scaled(screen, i, j, color2);
      }

  return ;
}

const std::vector<std::string> Player::read_map_strings(const std::string& filename)
{
  std::ifstream read_file{};
  read_file.open(filename, std::ios::in);

  std::vector<std::string> map_strings{};
  std::string input_buff{};
  while (!read_file.eof())
  {
    std::getline(read_file, input_buff);
    map_strings.emplace_back(input_buff);
  }
  return map_strings;
}