#include "memory.h"


Memory::Memory()
{
	for (int i = 0; i < 0xFFFF; i++)
	{
		MemoryMap[i] = rand() % 0xFF;
	}
}

void Memory::write(int location, unsigned char byte)
{
	MemoryMap[location] = byte;
}

unsigned char Memory::read(int location)
{
	return MemoryMap[location];
}