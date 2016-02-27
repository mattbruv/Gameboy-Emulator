#pragma once

#include <cstdint>

class Memory {

	uint8_t MemoryMap[0xFFFF];

	int ITERRUPT_REGISTER        = 0xFFFF;
	int INTERNAL_RAM_1           = 0xFF80;
	int EMPTY_1                  = 0xFF4C;
	int IO_PORTS                 = 0xFF00;
	int EMPTY_0                  = 0xFEA0;
	int SPRITE_MEMORY            = 0xFE00;
	int ECHO_INTERNAL_RAM_8k     = 0xE000;
	int INTERNAL_RAM_8k          = 0xC000;
	int SWITCH_RAM_BANK_8k       = 0xA000;
	int VIDEO_RAM_8k             = 0x8000;

	// Cartridge
	int CART_SWITCH_ROM_BANK_16k = 0x4000;
	int CART_ROM_BANK_0_16k      = 0x0;

};