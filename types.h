#pragma once

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdint>
#include <fstream>
#include <iterator>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

typedef uint16_t Address, Byte_2;
typedef uint8_t Byte, Opcode;
typedef int8_t Byte_Signed;

const Byte
	BIT_7 = 7,
	BIT_6 = 6,
	BIT_5 = 5,
	BIT_4 = 4,
	BIT_3 = 3,
	BIT_2 = 2,
	BIT_1 = 1,
	BIT_0 = 0;

const Byte
	INTERRUPT_VBLANK = 0,
	INTERRUPT_LCDC = 1,
	INTERRUPT_TIMER = 2,
	INTERRUPT_SERIAL = 3,
	INTERRUPT_JOYPAD = 4;

// Helper functions
Byte high_byte(Byte_2 target);
Byte low_byte(Byte_2 target);
Byte high_nibble(Byte target);
Byte low_nibble(Byte target);
Byte_2 combine(Byte high, Byte low);
bool between(Byte target, int low, int high);

Byte set_bit(Byte data, Byte bit);
Byte clear_bit(Byte data, Byte bit);
bool is_bit_set(Byte data, Byte bit);

// Register pair helper class
class Pair
{
private:
	Byte& high;
	Byte& low;

public:
	Pair(Byte& high_, Byte& low_)
		: high(high_), low(low_) {}
	void inc();
	void dec();
	void set(Byte_2 value);
	void set(Byte upper, Byte lower);
	Byte_2 get();

	Address address();
};

// Memory register helper class
class MemoryRegister
{
private:
	Byte *value;

public:
	MemoryRegister::MemoryRegister();
	MemoryRegister::MemoryRegister(Byte *_data);
	Byte get();
	void set(Byte data);
	void clear();
	void set_bit(Byte bit);
	void clear_bit(Byte bit);
	bool is_bit_set(Byte bit);
};