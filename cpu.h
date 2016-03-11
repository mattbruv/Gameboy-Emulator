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

		Byte high_reg_pair(Byte_2 reg_pair);
		Byte low_reg_pair(Byte_2 reg_pair);
		void dec_reg_pair(Byte& high, Byte& low);
		void inc_reg_pair(Byte& high, Byte& low);

		// 8-bit loads
		void LD(Byte& destination, Byte value);
		void LD(Byte& destination, Address addr);
		void LD(Address addr, Byte value);
		// 16-bit loads
		void LD(Byte_2& reg, Byte msb, Byte lsb);
		void LD(Byte& reg_high, Byte& reg_low, Byte msb, Byte lsb);
		void LD(Byte& reg_high, Byte& reg_low, Byte_2 value);
		void LD(Address destination, Byte_2 value);
};