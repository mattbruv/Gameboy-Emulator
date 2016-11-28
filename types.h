#pragma once

#include <iostream>
#include <cstdlib>
#include <cstdint>

using namespace std;

typedef uint16_t Address, Byte_2;
typedef uint8_t Byte, Opcode;
typedef int8_t Byte_Signed;

const int
	BIT_7 = 0b10000000,
	BIT_6 = 0b01000000,
	BIT_5 = 0b00100000,
	BIT_4 = 0b00010000,
	BIT_3 = 0b00001000,
	BIT_2 = 0b00000100,
	BIT_1 = 0b00000010,
	BIT_0 = 0b00000001;