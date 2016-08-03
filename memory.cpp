#include "memory.h"

Memory::Memory()
{
	for (int i = 0; i < 0xFFFF; i++)
	{
		MemoryMap[i] = 0;// rand() % 0xFF;
	}

	// The following memory locations are set to these arbitrary values on gameboy power up
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
	std::ifstream input(location, std::ios::binary);
	std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(input)), (std::istreambuf_iterator<char>()));

	int address = 0;

	for (std::vector<unsigned char>::iterator it = buffer.begin(); it != buffer.end(); ++it)
	{
		write(address++, *it);
	}
}

void Memory::write(Address location, Byte data)
{
	// Use of the area from 0xE000 to 0xFDFF is prohibited
	MemoryMap[location] = data;
}

Byte Memory::read(Address location)
{
	if (location > 0xFFFF)
		return 0;

	return MemoryMap[location];
}