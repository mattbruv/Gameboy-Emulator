#pragma once

// Gameboy CPU: 8-bit (Similar to the Z80 processor)
class CPU
{
	public: 
		CPU();
		void debug();

	private:
		unsigned char reg_A;
		unsigned char reg_B;
		unsigned char reg_C;
		unsigned char reg_D;
		unsigned char reg_E;
		unsigned char reg_H;
		unsigned char reg_L;
		unsigned char reg_F;

		unsigned short reg_SP; // Stack Pointer
		unsigned short reg_PC; // Program Counter

		void init();
		void reset();
		void execute(int cycles);
		void interrupt_signal();
		void stop();
		int parse_opcode(unsigned char opcode);
		void LD(unsigned char & destination, unsigned char value);
};