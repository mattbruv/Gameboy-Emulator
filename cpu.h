#pragma once

#include "types.h"
#include "memory.h"

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

		const int FLAG_ZERO       = 0b10000000;
		const int FLAG_SUB        = 0b01000000;
		const int FLAG_HALF_CARRY = 0b00100000;
		const int FLAG_CARRY      = 0b00010000;

		Memory memory;

		void init();
		void reset();
		void execute(int cycles);
		void interrupt_signal();
		void stop();
		int parse_opcode(Opcode code);
		void parse_bit_op(Opcode code);
		void set_flag(int flag, bool value);

		// 8-bit loads
		void LD(Byte& destination, Byte value);
		void LD(Byte& destination, Address addr);
		void LD(Address addr, Byte value);

		// 16-bit loads
		void LD(Pair reg_pair, Byte upper, Byte lower);
		void LD(Byte_2& reg_pair, Byte upper, Byte lower);
		void LDHL(Byte value);
		void LDNN(Byte low, Byte high);

		void PUSH(Byte high, Byte low);
		void POP(Byte& high, Byte& low);

		void ADD(Byte& target, Byte value);
		void ADD(Byte& target, Address addr);
		void ADC(Byte& target, Byte value);
		void ADC(Byte& target, Address addr);

		void SUB(Byte& target, Byte value);
		void SUB(Byte& target, Address addr);
		void SBC(Byte& target, Byte value);
		void SBC(Byte& target, Address addr);

		void AND(Byte& target, Byte value);
		void AND(Byte& target, Address addr);

		void OR(Byte& target, Byte value);
		void OR(Byte& target, Address addr);

		void XOR(Byte& target, Byte value);
		void XOR(Byte& target, Address addr);

		void CP(Byte& target, Byte value);
		void CP(Byte& target, Address addr);

		void INC(Byte& target);
		void INC(Address addr);

		void DEC(Byte& target);
		void DEC(Address addr);

		// 16-bit arithmetic
		void ADD16(Byte_2 target, Byte_2 value);
		void ADDHL(Pair reg_pair);
		void ADDSP(Byte value);

		void INC(Pair reg_pair);
		void DEC(Pair reg_pair);

		// Rotate shift instructions
		void RL(Byte& target, bool carry, bool zero_flag = false);
		void RL(Address addr, bool carry);
		void RR(Byte& target, bool carry, bool zero_flag = false);
		void RR(Address addr, bool carry);

		void SLA(Byte& target);
		void SLA(Address addr);
		void SRA(Byte& target);
		void SRA(Address addr);
		void SRL(Byte& target);
		void SRL(Address addr);

		void SWAP(Byte& target);
		void SWAP(Address addr);

		// Bit Operations
		void BIT(Byte target, int bit);
		void BIT(Address addr, int bit);

		void SET(Byte& target, int bit);
		void SET(Address addr, int bit);

		void RES(Byte& target, int bit);
		void RES(Address addr, int bit);

		// Miscellaneous Instructions
		void JP(Pair target);
		void JPNZ(Pair target);
		void JPZ(Pair target);
		void JPNC(Pair target);
		void JPC(Pair target);
};