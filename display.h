#pragma once

#include <SFML\Graphics.hpp>
#include <iostream>
#include "memory.h"

class Display
{
	public:
		void init(Memory* _memory);
		void draw_scanline();
		bool is_lcd_enabled();

	private:
		Memory* memory;
};