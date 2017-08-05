#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "memory.h"

class Display
{
	public:
		sf::RenderWindow window;

		// Debugging windows
		sf::RenderWindow bgmap;
		sf::RenderWindow tiles;

		// Reworking display
		vector<sf::Image> tile_RAM_1;

		sf::Image bg_array;
		sf::Image sprites_array;
		sf::Image window_array;

		sf::Sprite bg_sprite;
		sf::Sprite sprites_sprite; // lol
		sf::Sprite window_sprite;

		int width = 160,
			  height = 144;

		bool emulate_pallete = true;

		// debug variables
		bool debug_enabled = false;
		bool force_bg_map = false;
		bool force_bg_loc = false;

		void init(Memory* _memory);

		int scanlines_rendered = 0;

		// Scanline updating
		void update_scanline(Byte current_scanline);

		// Output all scanlines as a single frame
		void render();

		void show_vram();
		void handle_events();

		void resize_window(int scale);

		bool is_lcd_enabled();

	private:
		Memory* memory;

		const Byte
			COLOR_WHITE      = 0,
			COLOR_LIGHT_GRAY = 1,
			COLOR_DARK_GRAY  = 2,
			COLOR_BLACK      = 3;

		sf::Color shades_of_gray[4];

		void update_bg_scanline(Byte current_scanline);
		void update_window_scanline(Byte current_scanline);
		// TODO: void update_sprite_scanline(Byte current_scanline);

		void update_bg_tile_pixel(Byte palette, int display_x, int display_y, int tile_x, int tile_y, Byte tile_id);
		void update_window_tile_pixel(Byte palette, int display_x, int display_y, int tile_x, int tile_y, Byte tile_id);

		sf::Color get_pixel_color(Byte palette, Byte top, Byte bottom, int bit, bool is_sprite);

		void clear_window();
		void render_sprites();
		void render_sprite_tile(Byte pallete, int start_x, int start_y, Byte tile_id, Byte flags);

		void render_vram();
};
