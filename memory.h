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
		// Memory Regions
		vector<Byte> CART_ROM;	// $0000 - $7FFF, 32kB Cartridge (potentially dynamic)
		vector<Byte> VRAM;		// $8000 - $9FFF, 8kB Video RAM
		vector<Byte> OAM;		// $FE00 - $FEA0, OAM Sprite RAM
		vector<Byte> ERAM;		// $A000 - $BFFF, 8kB Cartridge external switchable RAM bank
		vector<Byte> WRAM;		// $C000 - $DFFF, 8kB Working RAM
		vector<Byte> ZRAM;		// $FF80 - $FFFF, 128 bytes of RAM

	public:

		const int 
			BIT_7 = 0b10000000,
			BIT_6 = 0b01000000,
			BIT_5 = 0b00100000,
			BIT_4 = 0b00010000,
			BIT_3 = 0b00001000,
			BIT_2 = 0b00000100,
			BIT_1 = 0b00000010,
			BIT_0 = 0b00000001;

		const Address
			ITERRUPT_REGISTER        = 0xFFFF,
			INTERNAL_RAM_1           = 0xFF80,
			EMPTY_1                  = 0xFF4C,
			IO_PORTS                 = 0xFF00,
			EMPTY_0                  = 0xFEA0,
			SPRITE_MEMORY            = 0xFE00,
			ECHO_INTERNAL_RAM_8k     = 0xE000,
			INTERNAL_RAM_8k          = 0xC000,
			SWITCH_RAM_BANK_8k       = 0xA000,
			VIDEO_RAM_8k             = 0x8000,
			CART_SWITCH_ROM_BANK_16k = 0x4000,
			CART_ROM_BANK_0_16k      = 0x0000;

		// Memory Register Address List
		const Address

			// Controller Data - 23
			P1 = 0xFF00,

			// Divider Registers - 24
			DIV = 0xFF04,

			// Timer Registers - 25
			TIMA = 0xFF05, // Timer Counter
			TMA  = 0xFF06, // Timer Modulo
			TAC  = 0xFF07, // Timer Controller

			// Interrupt Flags - 26
			IF = 0xFF0F, // Interrupt Request
			IE = 0xFFFF, // Interrupt Enable
			// IME or Interrupt Master Enable is defined in the CPU class

			// LCD Display Registers
			LCDC = 0xFF40,
			STAT = 0xFF41,
			SCY  = 0xFF42,
			SCX  = 0xFF43,
			LY   = 0xFF44,
			LYC  = 0xFF45,
			DMA  = 0xFF46,
			BGP  = 0xFF47,
			OBP0 = 0xFF48,
			OBP1 = 0xFF49,
			WY   = 0xFF4A,
			WX   = 0xFF4B;

			// OAM FE00 -> FE9F
			// Sound Registers
			// NR x x FF10 -> FF26 | Waveform RAM FF30 -> FF3F

		Memory::Memory();
		void load_rom(std::string location);

		void write(Address location, Byte data);
		Byte read(Address location);
};