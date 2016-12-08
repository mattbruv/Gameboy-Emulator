#include "display.h"

void Display::init(Memory* _memory)
{
	memory = _memory;
	window.create(sf::VideoMode(width, height), "Gameboy Emulator");
	window.setSize(sf::Vector2u(width * 2, height * 2));
	pixel_array.create(160, 144, sf::Color(255, 0, 255));
}

void Display::render()
{
	window.clear(sf::Color::Green);

	render_screen();

	sf::Texture pixel_texture;
	pixel_texture.loadFromImage(pixel_array);
	pixel_sprite.setTexture(pixel_texture);
	window.draw(pixel_sprite);
	
	window.display();
}

void Display::render_screen()
{
	// The goal right now is to just print the first tile in memory to screen

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
		render_tile(map, tile_id);

	}
}

void Display::render_tile(int display_number, int tile_number)
{
	// Figure out where the current background character data is being stored
	const Address bg_data_location = 0x8000; // TODO: set via LCDC register

	for (int y = 0; y < 8; y++)
	{
		int offset = (tile_number * 16) + bg_data_location;

		Byte
			high = memory->read(offset + (y * 2)),
			low  = memory->read(offset + (y * 2) + 1);

		for (int x = 0; x < 8; x++)
		{
			int pixel_x = ((display_number % 20) * 8) + 7 - x;
			int pixel_y = (floor(display_number / 20) * 8) + y;

			sf::Color color = get_pixel_color(low, high, x);
			pixel_array.setPixel(pixel_x, pixel_y, color);
		}
	}
}

// Returns the color of a pixel at X bit based on the 2 relevant line bytes
sf::Color Display::get_pixel_color(Byte top, Byte bottom, int bit)
{
	Byte first = (Byte)is_bit_set(top, bit);
	Byte second = (Byte)is_bit_set(bottom, bit);

	Byte color_code = (second << 1) | first;

	switch (color_code)
	{

	case 0b11: return sf::Color(8, 24, 32);
	case 0b01: return sf::Color(48, 104, 80);
	case 0b10: return sf::Color(136, 192, 112);
	case 0b00: return sf::Color(224, 248, 208);
		/*
		case 0b11: return sf::Color(0, 0, 0);
		case 0b10: return sf::Color(85, 85, 85);
		case 0b01: return sf::Color(170, 170, 170);
		case 0b00: return sf::Color(255, 255, 255);
		*/

		default:   return sf::Color(0, 0, 255);
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