#pragma once

#include <iostream>
#include <cstdlib>
#include <cstdint>

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