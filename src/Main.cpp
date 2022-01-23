#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window.hpp>

#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "func.hpp"

sf::RenderWindow window(sf::VideoMode::getFullscreenModes()[0], "SFML Window", sf::Style::Close);

int size = (window.getSize().x + window.getSize().y) / 75;
float speed = 10.0;
float time_from_last_move = 0.0;
sf::CircleShape apple;
sf::Vector2f way(1, 0);
std::vector<sf::CircleShape> snake_tiles;
sf::Clock delta_counter;
bool moved = true;
bool alive = true;
sf::Font font;

sf::Text end_game_text;

unsigned int map_size[2] = { window.getSize().x / size, window.getSize().y / size };

sf::CircleShape
snake_tile(sf::Vector2f pos)
{
	sf::CircleShape circle;

	if (pos.x < 0)
	{
		pos.x = size * map_size[0];
	}
	else if (pos.x > size * map_size[0])
	{
		pos.x = 0;
	}

	if (pos.y < 0)
	{
		pos.y = size * map_size[1] - size;
	}
	else if (pos.y > size * map_size[1] - size)
	{
		pos.y = 0;
	}

	circle.setPosition(pos);
	circle.setRadius(size / 2);
	circle.setFillColor(sf::Color::Green);
	return circle;
}

int random_number(int min, int max)
{
	return rand() % (max - min + 1) + min;
}

int spawn_apple()
{
	sf::Vector2f pos;
	bool b = false;
	while (!b)
	{
		b = true;
		pos = sf::Vector2f(random_number(0, map_size[0]) * size, random_number(0, map_size[1] - 1) * size);
		for (auto i : snake_tiles)
		{
			if (i.getPosition() == pos)
			{
				b = false;
			}
		}
	}
	apple.setPosition(pos);
	return 1;
}

int main()
{
	std::cout << font.loadFromFile("arial.ttf");
	end_game_text.setPosition(map_size[0] / 2 * size, map_size[1] / 2 * size);
	end_game_text.setString(sf::String("you died"));
	end_game_text.setFillColor(sf::Color::Red);
	end_game_text.setCharacterSize(size);
	end_game_text.setFont(font);
	apple.setRadius(size / 2);
	apple.setFillColor(sf::Color::Red);
	spawn_apple();
	//game data
	for (int i = 0; i < 10; i++)
	{
		snake_tiles.push_back(snake_tile(sf::Vector2f(0 + i * size, size * 10)));
	}

	while (window.isOpen())
	{
		sf::Time delta_time = delta_counter.restart();
		time_from_last_move += delta_time.asSeconds();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			else if (event.type == sf::Event::KeyPressed && moved)
			{
				//down
				if ((event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) && way.y != -1)
				{
					moved = false;
					way = sf::Vector2f(0, 1);
				}
				//up
				else if ((event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) && way.y != 1)
				{
					moved = false;
					way = sf::Vector2f(0, -1);
				}
				//left
				else if ((event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) && way.x != 1)
				{
					moved = false;
					way = sf::Vector2f(-1, 0);
				}
				//right
				else if ((event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) && way.x != -1)
				{
					moved = false;
					way = sf::Vector2f(1, 0);
				}
			}
		}

		//draw and move

		if (time_from_last_move >= 1 / speed && alive)
		{

			auto pos = snake_tiles[std::size(snake_tiles) - 1].getPosition();
			snake_tiles.push_back(snake_tile(sf::Vector2f(pos.x + way.x * size, pos.y + way.y * size)));

			time_from_last_move = 0.0;
			moved = true;

			//eating apple
			sf::Vector2f head_pos = snake_tiles[snake_tiles.size() - 1].getPosition();
			if (head_pos == apple.getPosition())
			{
				spawn_apple();
				speed += 0.25;
			}
			else
			{
				snake_tiles.erase(snake_tiles.begin());
			}
			// check death
			int times_overlaped = 0;
			for (auto i : snake_tiles)
			{
				if (i.getPosition() == head_pos)
				{
					times_overlaped++;
				}
			}
			if (times_overlaped > 1)
			{
				alive = false;
			}
		}
		window.clear(sf::Color::Black);
		for (auto& i : snake_tiles)
		{
			window.draw(i);
		}
		window.draw(apple);
		if (!alive)
		{
			window.draw(end_game_text);
		}
		window.display();
	}

	return 0;
}