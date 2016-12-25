#pragma once

#include <SFML\Graphics.hpp>
#include <iostream>
#include "memory.h"

class Display
{
	public:
		sf::RenderWindow window;
		
		sf::Image bg_array;
		sf::Image sprites_array;

		sf::Sprite bg_sprite;
		sf::Sprite sprites_sprite; // lol

		int width = 160,
			height = 144;

		bool emulate_pallete = true;

		// debug variables
		bool debug_enabled = false;
		bool force_bg_map = false;
		bool force_bg_loc = false;

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

		sf::Color shades_of_gray[4];

		void render_background();
		void render_bg_tile_pixel(Byte palette, int display_x, int display_y, int tile_x, int tile_y, Byte tile_id);
		void render_sprites();
		void render_sprite_tile(Byte pallete, int start_x, int start_y, Byte tile_id, Byte flags);
		sf::Color get_pixel_color(Byte palette, Byte top, Byte bottom, int bit, bool is_sprite);
};