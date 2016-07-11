#include "memory.h"

Memory::Memory()
{
	for (int i = 0; i < 0xFFFF; i++)
	{
		MemoryMap[i] = 0;// rand() % 0xFF;
	}
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