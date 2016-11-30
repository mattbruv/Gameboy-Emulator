#include "display.h"

void Display::init(Memory* _memory)
{
	memory = _memory;
}

void Display::draw_scanline()
{

}

bool Display::is_lcd_enabled()
{
	return memory->LCDC.is_bit_set(BIT_7);
}