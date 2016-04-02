#include "memory.h"


Memory::Memory()
{
	for (int i = 0; i < 0x10000; i++)
	{
		MemoryMap[i] = rand() % 0xFF;
	}
}

void Memory::write(Address location, Byte data)
{
	MemoryMap[location] = data;
}

Byte Memory::read(Address location)
{
	return MemoryMap[location];
}