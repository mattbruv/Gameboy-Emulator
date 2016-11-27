#include "memory.h"

Memory::Memory()
{
	WRAM = vector<Byte>(0x2000); // $C000 - $DFFF, 8kB Working RAM
	ERAM = vector<Byte>(0x2000); // $A000 - $BFFF, 8kB switchable RAM bank, size liable to change in future
	ZRAM = vector<Byte>(0x0080); // $FF80 - $FFFF, 128 bytes of RAM
	VRAM = vector<Byte>(0x2000); // $8000 - $9FFF, 8kB Video RAM
	OAM  = vector<Byte>(0x00A0); // $FE00 - $FEA0, OAM Sprite RAM
	
	// The following memory locations are set to the following arbitrary values on gameboy power up
	write(0xFF05, 0x00); // TIMA
	write(0xFF06, 0x00); // TMA
	write(0xFF07, 0x00); // TAC
	write(0xFF10, 0x80); // NR10
	write(0xFF11, 0xBF); // NR11
	write(0xFF12, 0xF3); // NR12
	write(0xFF14, 0xBF); // NR14
	write(0xFF16, 0x3F); // NR21
	write(0xFF17, 0x00); // NR22
	write(0xFF19, 0xBF); // NR24
	write(0xFF1A, 0x7F); // NR30
	write(0xFF1B, 0xFF); // NR31
	write(0xFF1C, 0x95); // NR32
	write(0xFF1D, 0xBF); // NR33
	write(0xFF20, 0xFF); // NR41
	write(0xFF21, 0x00); // NR42
	write(0xFF22, 0x00); // NR43
	write(0xFF23, 0xBF); // NR30
	write(0xFF24, 0x77); // NR50
	write(0xFF25, 0xF3); // NR51
	write(0xFF26, 0xF1); // $F0 - SUPERGB ; NR52
	write(0xFF40, 0x91); // LCDC
	write(0xFF42, 0x00); // SCY
	write(0xFF43, 0x00); // SCX
	write(0xFF45, 0x00); // LYC
	write(0xFF47, 0xFC); // BGP
	write(0xFF48, 0xFF); // 0BP0
	write(0xFF49, 0xFF); // 0BP1
	write(0xFF4A, 0x00); // WY
	write(0xFF4B, 0x00); // WX
	write(0xFFFF, 0x00); // IE
}

void Memory::load_rom(std::string location)
{
	ifstream input(location, ios::binary);
	vector<Byte> buffer((istreambuf_iterator<char>(input)), (istreambuf_iterator<char>()));
	CART_ROM = buffer;
}

Byte Memory::read(Address location)
{
	switch (location & 0xF000)
	{
	// ROM0
	case 0x0000:
	case 0x1000:
	case 0x2000:
	case 0x3000:
		return CART_ROM[location];

	// ROM1 (no bank switching)
	case 0x4000:
	case 0x5000:
	case 0x6000:
	case 0x7000:
		return CART_ROM[location];

	// Graphics VRAM
	case 0x8000:
	case 0x9000:
		return VRAM[location & 0x1FFF];

	// External RAM
	case 0xA000:
	case 0xB000:
		return ERAM[location & 0x1FFF];

	// Working RAM (8kB) and RAM Shadow
	case 0xC000:
	case 0xD000:
	case 0xE000:
		return WRAM[location & 0x1FFF];

	// Remaining Working RAM Shadow, I/O, Zero page RAM
	case 0xF000:
		switch (location & 0x0F00)
		{
			// Remaining Working RAM
			case 0x000: case 0x100: case 0x200: case 0x300:
			case 0x400: case 0x500: case 0x600: case 0x700:
			case 0x800: case 0x900: case 0xA00: case 0xB00:
			case 0xC00: case 0xD00:
				return WRAM[location & 0x1FFF];

			// Sprite OAM
			case 0xE00:
				if (location < 0xFEA0)
					return OAM[location & 0xFF];
				else
					return 0; // possibly return regular memory

			case 0xF00:
				if (location >= 0xFF80)
					return ZRAM[location & 0x7F];
				else
				{
					// TODO: I/O control handling
					return 0;
				}
		}
	}
}

void Memory::write(Address location, Byte data)
{
	switch (location & 0xF000)
	{
	// ROM0
	case 0x0000:
	case 0x1000:
	case 0x2000:
	case 0x3000:
		// CART_ROM[location] = data; - READ ONLY
		break;

	// ROM1 (no bank switching)
	case 0x4000:
	case 0x5000:
	case 0x6000:
	case 0x7000:
		// CART_ROM[location] = data; - READ ONLY
		break;

	// Graphics VRAM
	case 0x8000:
	case 0x9000:
		VRAM[location & 0x1FFF] = data;
		break;

	// External RAM
	case 0xA000:
	case 0xB000:
		ERAM[location & 0x1FFF] = data;
		break;

	// Working RAM (8kB) and RAM Shadow
	case 0xC000:
	case 0xD000:
	case 0xE000:
		WRAM[location & 0x1FFF] = data;
		break;

	// Remaining Working RAM Shadow, I/O, Zero page RAM
	case 0xF000:
		switch (location & 0x0F00)
		{
		// Remaining Working RAM
		case 0x000: case 0x100: case 0x200: case 0x300:
		case 0x400: case 0x500: case 0x600: case 0x700:
		case 0x800: case 0x900: case 0xA00: case 0xB00:
		case 0xC00: case 0xD00:
			WRAM[location & 0x1FFF] = data;
			break;

		// Sprite OAM
		case 0xE00:
			if (location < 0xFEA0)
			{
				OAM[location & 0xFF] = data;
				break;
			}
			else
				return; // Write here?

		case 0xF00:
			if (location >= 0xFF80)
			{
				ZRAM[location & 0x7F] = data;
				break;
			}
			else
			{
				// TODO: I/O control handling
				return; 
			}
		}
	}
}