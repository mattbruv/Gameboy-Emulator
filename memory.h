#pragma once

#include "types.h"

#include <fstream>
#include <iterator>
#include <algorithm>
#include <string>
#include <vector>
#include <iostream>

class MemoryRegister
{
	private:
		Byte *value;

	public:
		MemoryRegister::MemoryRegister();
		MemoryRegister::MemoryRegister(Byte *_data);
		Byte get();
		void set(Byte data);
		void set_bit(Byte bit);
		void clear_bit(Byte bit);
		bool is_bit_set(Byte bit);
};

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

		MemoryRegister
			DIV, TIMA, TMA, TAC,
			LCDC, SCY, SCX, LYC, BGP, ZBP0, ZBP1, WY, WX,
			IF, IE;

		Memory::Memory();
		void load_rom(std::string location);

		Byte read(Address location);

		void write(Address location, Byte data);
		void write_zero_page(Address location, Byte data);
};