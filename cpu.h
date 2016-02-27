#pragma once

#include <cstdint>

typedef struct
{
	uint16_t value_16;
	uint8_t value_8;

	uint8_t high;
	uint8_t low;

} CPU_Reg;

// Gameboy CPU: 8-bit (Similar to the Z80 processor)
class CPU
{
	// Clock Speed: 4.19MHz 
	int clock_cycles;

	/*
		The Gameboy CPU has eight 8-bit registers A,B,C,D,E,F,H,L and two 16-bit registers SP & PC

		Some instructions, however, allow you to use the
		registers A,B,C,D,E,H, & L as 16-bit registers by
		pairing them up in the following manner: AF,BC,DE, &
		HL. The F register is indirectly accessible by the programmer
	*/
	CPU_Reg A;
	CPU_Reg B;
	CPU_Reg C;
	CPU_Reg D;
	CPU_Reg E;
	CPU_Reg F;
	CPU_Reg H;
	CPU_Reg L;

	CPU_Reg SP; // Stack Pointer
	CPU_Reg PC; // Program Counter

	/*
		The flag register consists of the following bits:

		| 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
		---------------------------------
		| Z | N | H | C | 0 | 0 | 0 | 0 |

		Zero Flag (Z): set when result of math operation is zero or 2 values match using CP instruction
		Subtract Flag (N): set when subtraction was performed last
		Half Carry Flag (H): set if a carry odccured from the lower nibble in the last math operation
		Carry Flag (C): set if a carry occured from the last math operation or if A is smaller when executing CP instruction
		
	*/
	CPU_Reg Flag; // Flag Register

	private:
		void init();
		void reset();
		void execute(int cycles);
		void interrupt_signal();
		void stop();
};