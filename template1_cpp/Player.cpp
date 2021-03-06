#include "Player.h"
#include <GLFW/glfw3.h>

bool Player::Moved() const
{
	if(coords.x == old_coords.x && coords.y == old_coords.y)
		return false;
	else
		return true;
}

int Player::map_draw(std::string file_name, Image &screen, Image &tmp, std::vector<int> vec) {
		std::ifstream F(file_name);
		char a;
		map.clear();
		for (int i = 0; i < WINDOW_HEIGHT ; i += 32) {
				for (int j = 0; j < WINDOW_WIDTH; j += 32) {
						F >> a;
						if (a == '.') {
								vec.push_back(1);
								screen.drawTile(Image("../resources/walls.png"), j, i);
								tmp.drawTile(Image("../resources/walls.png"), j, i);
						}
						else if (a == '@') {
								vec.push_back(1);
								screen.drawTile(Image("../resources/walls.png"), j, i);
								tmp.drawTile(Image("../resources/walls.png"), j, i);
								coords.x = j;
								coords.y = i;
								old_coords.x = j;
								old_coords.y = i;
						}
						else if (a == '#') {
								vec.push_back(2);
								screen.drawTile(Image("../resources/d.png"), j, i);
						}
						else if (a == ' ') {
							vec.push_back(2);
							screen.drawTile(Image("../resources/d.png"), j, i);
						}
						else if (a == 'T') {
								vec.push_back(3);
								screen.drawTile(Image("../resources/another_spikes.png"), j, i);
						}
						else if (a == 'x') {
								vec.push_back(4);
								screen.drawTile(Image("../resources/g.png"), j, i);
						}
				}
				map.push_back(vec);
				vec.clear();
		}
	return (0);
}


void Player::ProcessInput(MovementDir dir)
{
	int move_dist = move_speed * 1;
		if (dir == MovementDir::UP) {
			old_coords.y = coords.y;
			for (int i = 0; i < move_dist; i++) {
				 if (coords.y < WINDOW_HEIGHT - 32 - 2) {
					if (map[(coords.y + 1) / 32 + 1][coords.x / 32] != 2
							&& map[(coords.y + 1) / 32 + 1][(coords.x + 32) / 32] != 2)
							coords.y++;
				}
			}
		}
		if (dir == MovementDir::DOWN) {
			old_coords.y = coords.y;
			for (int i = 0; i < move_dist; i++) {
				if (coords.y > 0) {
					if (map[(coords.y - 33) / 32 + 1][coords.x / 32] != 2
						&& map[(coords.y - 33) / 32 + 1][(coords.x + 32) / 32] != 2)
						coords.y--;
				}
			}
		}
		if (dir == MovementDir::LEFT) {
			old_coords.x = coords.x;
			for (int i = 0; i < move_dist; i++) {
				if (map[coords.y / 32][(coords.x - 1) / 32] != 2
						&& map[(coords.y + 32) / 32][(coords.x - 1) / 32] != 2)
					coords.x--;
			}
		}
		if (dir == MovementDir::RIGHT) {
			old_coords.x = coords.x;
			for (int i = 0; i < move_dist; i++) {
				if (map[coords.y / 32][(coords.x + 1) / 32] != 2
						&& map[(coords.y + 32)/ 32][(coords.x + 33) / 32] != 2)
					coords.x++;
			}
		}
}

int Player::Draw(Image &screen, Image &tmp, int counter_levels)
{
	if(Moved()) {
		for(int x = old_coords.x; x <= old_coords.x + tileSize; ++x)
			for(int y = old_coords.y; y <= old_coords.y + tileSize; ++y) {
				if (map[(y + 1) / 32 ][x / 32] == 3) {
						screen.DrawPicForSeconds(Image("../resources/image.png"), 250, 250);
						return (-1);
				}
				if (map[(y + 1) / 32 ][x / 32] == 4) {
						if (counter_levels == 2) {
							screen.DrawPicForSeconds(Image("../resources/image.png"), 250, 250);
							return (-1);
						}
						screen.DrawPicForSeconds(Image("../resources/newlevel.png"), 250, 250);
						return (0);
				}
				Pixel color_for_draw = tmp.GetPixel(x, y);
				screen.PutPixel(x, y, color_for_draw);
			}
		old_coords = coords;
	}

	for(int y = coords.y; y <= coords.y; ++y)
		for(int x = coords.x; x <= coords.x; ++x)
			screen.drawChechik(Image("../resources/chechik.png"), x, y);
	return (1);
}

void Player::BlockWall() {
		int delta_x = coords.x - old_coords.x;
		int delta_y = coords.y - old_coords.y;
		for (int i = coords.y / 32; i < (coords.y + 64) / 32; i++) {
				for (int j = coords.x / 32; j < (coords.x + 64) / 32; j++) {
						if (map[i][j] == 2) {
								if (delta_x > 0 || delta_x < 0) {
										coords.x = old_coords.x;
								}
								if (delta_y > 0 || delta_y < 0) {
										coords.y = old_coords.y;
								}
						}
				}
		}
}

void	ft_scaled(Image &screen, int i, int j, Pixel color)
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