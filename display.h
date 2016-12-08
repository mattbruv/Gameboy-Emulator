#pragma once

#include <SFML\Graphics.hpp>
#include <iostream>
#include "memory.h"

class Display
{
	public:
		sf::RenderWindow window;
		sf::Image pixel_array;
		sf::Sprite pixel_sprite;

		int width = 160,
			height = 144;

		void init(Memory* _memory);

		void draw_scanline();
		bool is_lcd_enabled();
		void render();

	private:
		Memory* memory;

		const Byte
			COLOR_WHITE      = 0,
			COLOR_LIGHT_GRAY = 1,
			COLOR_DARK_GRAY  = 2,
			COLOR_BLACK      = 3;

		void render_screen();
		void render_tile(int display_number, int tile_number);
		sf::Color get_pixel_color(Byte top, Byte bottom, int bit);
};