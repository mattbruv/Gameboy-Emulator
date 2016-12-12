#include "display.h"

void Display::init(Memory* _memory)
{
	memory = _memory;

	int scale = 1;
	
	window.create(sf::VideoMode(width, height), "Gameboy Emulator");
	window.setSize(sf::Vector2u(width * scale, height * scale));
	window.setKeyRepeatEnabled(false);

	bg_array.create(160, 144, sf::Color(255, 0, 255));
	sprites_array.create(160, 144, sf::Color(0, 0, 0, 0)); // transparent
}

void Display::render()
{
	window.clear(sf::Color::Transparent);

	// clear existig sprite data
	sprites_array.create(160, 144, sf::Color(0, 0, 0, 0));

	render_background();
	render_sprites();

	sf::Texture bg_texture;
	sf::Texture sprites_texture;

	bg_texture.loadFromImage(bg_array);
	sprites_texture.loadFromImage(sprites_array);

	bg_sprite.setTexture(bg_texture);
	sprites_sprite.setTexture(sprites_texture);
	
	window.draw(bg_sprite);
	window.draw(sprites_sprite);

	window.display();
}

void Display::render_background()
{
	// LCDC variables
	bool bg_enabled = memory->LCDC.is_bit_set(BIT_0);

	bool obj_enabled = memory->LCDC.is_bit_set(BIT_1);
	bool obj_selection = memory->LCDC.is_bit_set(BIT_2);

	bool bg_code_area = memory->LCDC.is_bit_set(BIT_3);
	bool bg_char_selection = memory->LCDC.is_bit_set(BIT_4);

	bool window_enabled = memory->LCDC.is_bit_set(BIT_5);
	bool window_code_area = memory->LCDC.is_bit_set(BIT_6);

	bool lcd_enabled = is_lcd_enabled();

	// return if the display is not enabled
	if (!lcd_enabled)
		return;

	Address tile_map = (bg_code_area) ? 0x9C00 : 0x9800;
	Byte scroll_x = memory->SCX.get();
	Byte scroll_y = memory->SCY.get();

	// width in tiles * height in tiles = total tiles
	int background_tiles = (160 / 8) * (144 / 8);

	for (int map = 0; map < background_tiles; map++)
	{
		int mod = (map % 20);
		int y = floor(map / 20);
		int result = mod + (y * 32);

		result += tile_map;
		int tile_id = memory->read(result);
		render_bg_tile(map, tile_id);

	}
}

void Display::render_bg_tile(int display_number, int tile_id)
{
	// Figure out where the current background character data is being stored
	Address bg_data_location = 0x8000; // TODO: set via LCDC register

	for (int y = 0; y < 8; y++)
	{
		int offset = (tile_id * 16) + bg_data_location;

		Byte
			high = memory->read(offset + (y * 2)),
			low  = memory->read(offset + (y * 2) + 1);

		for (int x = 0; x < 8; x++)
		{
			int pixel_x = ((display_number % 20) * 8) + 7 - x;
			int pixel_y = (floor(display_number / 20) * 8) + y;

			sf::Color color = get_pixel_color(low, high, x, false);
			bg_array.setPixel(pixel_x, pixel_y, color);
		}
	}
}

void Display::render_sprites()
{
	Address sprite_data_location = 0xFE00;

	// 160 bytes of sprite data / 4 bytes per sprite = 40 sprites
	for (int sprite_id = 0; sprite_id < 40; sprite_id++)
	{
		Address offset = sprite_data_location + (sprite_id * 4);
		Byte y_pos   = memory->read(offset) - 16;
		Byte x_pos   = memory->read(offset + 1) - 8;

		if (y_pos >= 160)
			continue;

		Byte tile_id = memory->read(offset + 2);
		Byte flags   = memory->read(offset + 3);

		render_sprite_tile(x_pos, y_pos, tile_id);
	}
}

void Display::render_sprite_tile(int start_x, int start_y, int tile_id)
{
	Address sprite_data_location = 0x8000;

	for (int y = 0; y < 8; y++)
	{
		int offset = (tile_id * 16) + sprite_data_location;

		Byte
			high = memory->read(offset + (y * 2)),
			low  = memory->read(offset + (y * 2) + 1);

		for (int x = 0; x < 8; x++)
		{
			int pixel_x = start_x + 7 - x;
			int pixel_y = start_y + y;

			sf::Color color = get_pixel_color(low, high, x, true);
			sprites_array.setPixel(pixel_x, pixel_y, color);
		}
	}
}

// Returns the color of a pixel at X bit based on the 2 relevant line bytes
sf::Color Display::get_pixel_color(Byte top, Byte bottom, int bit, bool is_sprite)
{
	Byte first = (Byte)is_bit_set(top, bit);
	Byte second = (Byte)is_bit_set(bottom, bit);

	Byte color_code = (second << 1) | first;

	if (emulate_pallete) {
		switch (color_code)
		{
			case 0b11: return sf::Color(8, 24, 32);
			case 0b01: return sf::Color(48, 104, 80);
			case 0b10: return sf::Color(136, 192, 112);
			case 0b00: return sf::Color(224, 248, 208, ((is_sprite) ? 0 : 255));
			default:   return sf::Color(0, 0, 255);
		}
	}
	else {
		switch (color_code)
		{
			case 0b11: return sf::Color(0, 0, 0);
			case 0b01: return sf::Color(127, 127, 127);
			case 0b10: return sf::Color(198, 198, 198);
			case 0b00: return sf::Color(255, 255, 255);
			default:   return sf::Color(0, 0, 255);
		}
	}
}

void Display::draw_scanline()
{
	Byte control = memory->LCDC.get();

	// Render tiles
	if (is_bit_set(control, BIT_0))
	{
	}

	// Render sprites
	if (is_bit_set(control, BIT_1))
	{
	}
}

bool Display::is_lcd_enabled()
{
	return memory->LCDC.is_bit_set(BIT_7);
}