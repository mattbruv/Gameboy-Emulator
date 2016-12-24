#include "memory_controllers.h"

void MemoryController::init(vector<Byte> cartridge_buffer)
{
	CART_ROM = cartridge_buffer;
	ERAM = vector<Byte>(0x2000); // $A000 - $BFFF, 8kB switchable RAM bank, size liable to change in future
}

Byte MemoryController0::read(Address location)
{
	if (location >= 0x0000 && location <= 0x7FFF)
		return CART_ROM[location];
	else if (location >= 0xA000 && location <= 0xBFFF)
		return ERAM[location & 0x1FFF];
	else
		return 0x00;
}

void MemoryController0::write(Address location, Byte data)
{
	if (location >= 0xA000 && location <= 0xBFFF)
		ERAM[location & 0x1FFF] = data;
}

Byte MemoryController1::read(Address location) { return 0; }
void MemoryController1::write(Address location, Byte data) {}

Byte MemoryController2::read(Address location) { return 0; }
void MemoryController2::write(Address location, Byte data) {}