#include "types.h"

/*
	Helper Functions
*/
Byte high_byte(Byte_2 target)
{
    return (Byte)(target >> 8) & 0xFF;
}

Byte low_byte(Byte_2 target)
{
    return (Byte)target;
}

Byte high_nibble(Byte target)
{
    return ((target >> 4) & 0xF);
}

Byte low_nibble(Byte target)
{
    // Byte test = (target & 0xF);
    return (target & 0xF);
}

Byte_2 combine(Byte high, Byte low)
{
    return (high << 8 | low);
}

bool between(Byte target, int low, int high)
{
    return (target >= low && target <= high);
}

Byte set_bit(Byte data, Byte bit)
{
    return data | (1 << bit);
}

Byte clear_bit(Byte data, Byte bit)
{
    return data & (~(1 << bit));
}

bool is_bit_set(Byte data, Byte bit)
{
    return ((data >> bit) & 1) ? true : false;
}

/*
	8-Bit Pair Helper Class
*/
void Pair::inc()
{
    Address addr = address() + 1;
    set(addr);
}

void Pair::dec()
{
    Address addr = address() - 1;
    set(addr);
}

void Pair::set(Byte_2 value)
{
    high = high_byte(value);
    low = low_byte(value);
}

void Pair::set(Byte upper, Byte lower)
{
    high = upper;
    low = lower;
}

Byte_2 Pair::get()
{
    return address();
}

Address Pair::address()
{
    return combine(high, low);
}

/*
	Memory Register Helper Class
*/
MemoryRegister::MemoryRegister()
{
}

MemoryRegister::MemoryRegister(Byte* _data)
{
    value = _data;
}

Byte MemoryRegister::get()
{
    return *value;
}

void MemoryRegister::set(Byte data)
{
    *value = data;
}

void MemoryRegister::clear()
{
    set(0x00);
}

void MemoryRegister::set_bit(Byte bit)
{
    *value |= 1 << bit;
}

void MemoryRegister::clear_bit(Byte bit)
{
    *value &= ~(1 << bit);
}

bool MemoryRegister::is_bit_set(Byte bit)
{
    return ((*value >> bit) & 1) ? true : false;
}
