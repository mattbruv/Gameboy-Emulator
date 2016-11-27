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
			TIMA, TMA, TAC,
			LCDC, SCY, SCX, LYC, BGP, ZBP0, ZBP1, WY, WX, IE;

		const int 
			BIT_7 = 0b10000000,
			BIT_6 = 0b01000000,
			BIT_5 = 0b00100000,
			BIT_4 = 0b00010000,
			BIT_3 = 0b00001000,
			BIT_2 = 0b00000100,
			BIT_1 = 0b00000010,
			BIT_0 = 0b00000001;

		Memory::Memory();
		void load_rom(std::string location);

		void write(Address location, Byte data);
		Byte read(Address location);
};