#include "memory.h"


Memory::Memory()
{
	for (int i = 0; i < 0xFFFF; i++)
	{
		MemoryMap[i] = rand() % 0xFF;
	}
}

void Memory::write(int location, Byte data)
{
	MemoryMap[location] = data;
}

unsigned char Memory::read(int location)
{
	return MemoryMap[location];
}