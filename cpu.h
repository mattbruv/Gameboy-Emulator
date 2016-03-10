#pragma once

#include "types.h"
#include "memory.h"

// Gameboy CPU: 8-bit (Similar to the Z80 processor)
class CPU
{
	public: 
		CPU();
		void debug();

	private:
		Byte reg_A;
		Byte reg_B;
		Byte reg_C;
		Byte reg_D;
		Byte reg_E;
		Byte reg_H;
		Byte reg_L;
		Byte reg_F;
		Byte_2 reg_SP; // Stack Pointer
		Byte_2 reg_PC; // Program Counter

		Memory memory;

		void init();
		void reset();
		void execute(int cycles);
		void interrupt_signal();
		void stop();
		Address address(Byte high, Byte low);
		int parse_opcode(Opcode code);

		void dec_reg_pair(Byte& high, Byte& low);
		void inc_reg_pair(Byte& high, Byte& low);

		void LD(Byte& destination, Byte value);
		void LD(Byte& destination, Address addr);
		void LD(Address addr, Byte value);
};