#include "display.h"

void Display::init(Memory* _memory)
{
	memory = _memory;
	window.create(sf::VideoMode(width, height), "Gameboy Emulator");
	pixel_array.create(160, 144, sf::Color(255, 0, 255));
}

void Display::render()
{
	window.clear(sf::Color::Green);

	render_tiles();
	/*
	pixel_array.setPixel(0, 0, sf::Color::Yellow);
	pixel_array.setPixel(0, 1, sf::Color::Green);
	pixel_array.setPixel(0, 2, sf::Color::Blue);
	pixel_array.setPixel(0, 3, sf::Color::Yellow);
	pixel_array.setPixel(0, 4, sf::Color::Green);
	pixel_array.setPixel(0, 5, sf::Color::Blue);
	pixel_array.setPixel(0, 6, sf::Color::Yellow);
	pixel_array.setPixel(0, 7, sf::Color::Green);
	pixel_array.setPixel(0, 8, sf::Color::Blue);
	pixel_array.setPixel(0, 9, sf::Color::Green);
	pixel_array.setPixel(0, 10, sf::Color::Yellow);
	pixel_array.setPixel(0, 11, sf::Color::Green);
	pixel_array.setPixel(0, 12, sf::Color::Blue);
	pixel_array.setPixel(0, 13, sf::Color::Yellow);
	pixel_array.setPixel(0, 14, sf::Color::Green);
	pixel_array.setPixel(0, 15, sf::Color::Blue); /**/


	sf::Texture pixel_texture;
	pixel_texture.loadFromImage(pixel_array);
	pixel_sprite.setTexture(pixel_texture);
	window.draw(pixel_sprite);
	
	window.display();
}

void Display::render_tiles()
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

	Byte scroll_x = memory->SCX.get();
	Byte scroll_y = memory->SCY.get();

	// return if the display is not enabled
	//if (!lcd_enabled)
	//	return;

	// Figure out where the current background character data is being stored
	const Address bg_data_location = 0x8000;

	// loop through background data as tiles and print tile
	for (int current_tile = 0; current_tile < 256; current_tile++)
	{
		for (int y = 0; y < 8; y++)
		{
			int offset = (current_tile * 16) + bg_data_location;

			Byte
				high = memory->read(offset + (y * 2)),
				low  = memory->read(offset + (y * 2) + 1);

			for (int x = 0; x < 8; x++)
			{
				int pixel_x = ((current_tile % 20) * 8) + x;
				int pixel_y = (floor(current_tile / 20) * 8) + y;

				sf::Color color = get_pixel_color(low, high, x);
				pixel_array.setPixel(pixel_x, pixel_y, color);
			}
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
		case 0b11: return sf::Color(0, 0, 0);
		case 0b10: return sf::Color(85, 85, 85);
		case 0b01: return sf::Color(170, 170, 170);
		case 0b00: return sf::Color(255, 255, 255);
		default:   return sf::Color(255, 0, 255);
	}
}

void Display::handle_window_events()
{
	sf::Event event;
	
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
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