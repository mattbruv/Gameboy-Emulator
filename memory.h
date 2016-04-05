#pragma once

#include "types.h"

#include <fstream>
#include <iterator>
#include <algorithm>
#include <string>
#include <vector>
#include <iostream>

class Memory
{
	private:
		Byte MemoryMap[0x10000];

	public:

		const Address ITERRUPT_REGISTER        = 0xFFFF;
		const Address INTERNAL_RAM_1           = 0xFF80;
		const Address EMPTY_1                  = 0xFF4C;
		const Address IO_PORTS                 = 0xFF00;
		const Address EMPTY_0                  = 0xFEA0;
		const Address SPRITE_MEMORY            = 0xFE00;
		const Address ECHO_INTERNAL_RAM_8k     = 0xE000;
		const Address INTERNAL_RAM_8k          = 0xC000;
		const Address SWITCH_RAM_BANK_8k       = 0xA000;
		const Address VIDEO_RAM_8k             = 0x8000;
		const Address CART_SWITCH_ROM_BANK_16k = 0x4000;
		const Address CART_ROM_BANK_0_16k      = 0x0;

		// Registers

		Memory::Memory();
		void load_rom(std::string location);

		void write(Address location, Byte data);
		Byte read(Address location);
};