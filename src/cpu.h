#pragma once

#include "types.h"
#include "memory.h"

// Gameboy CPU: 8-bit (Similar to the Z80 processor)
class CPU
{
	public:
		Byte reg_A; // Accumulator
		Byte reg_B;
		Byte reg_C;
		Byte reg_D;
		Byte reg_E;
		Byte reg_H;
		Byte reg_L;
		Byte reg_F; // Flag Register
		Byte_2 reg_SP; // Stack Pointer
		Byte_2 reg_PC; // Program Counter

		int CLOCK_SPEED = 4194304; // 4194304 Hz CPU speed
		int num_cycles = 0;
		bool interrupt_master_enable = true;
		bool halted = false;

		void save_state(ofstream &file);
		void load_state(ifstream &file);

		void init(Memory* _memory);
		void reset();
		void parse_opcode(Opcode code);
		void debug();

	private:

		Memory* memory;

		const int
			FLAG_ZERO       = 0b10000000,
			FLAG_SUB        = 0b01000000,
			FLAG_HALF_CARRY = 0b00100000,
			FLAG_CARRY      = 0b00010000;

		void op(int pc, int cycle);
		void parse_bit_op(Opcode code);
		void set_flag(int flag, bool value);

		// ---------- CPU INSTRUCTIONS ---------- //

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
		void ADDHLSP();
		void ADDSP(Byte value);

		void INC(Pair reg_pair);
		void INCSP();
		void DEC(Pair reg_pair);
		void DECSP();

		// Rotate shift instructions

		// Shift Left
		void SL(Byte& target);
		void SL(Address addr);

		// Shift Right
		void SR(Byte& target, bool include_top_bit);
		void SR(Address addr, bool include_top_bit);

		// Shifts through carry
		void RL(Byte& target, bool carry, bool zero_flag = false);
		void RL(Address addr, bool carry);
		void RR(Byte& target, bool carry, bool zero_flag = false);
		void RR(Address addr, bool carry);

		void SRA(Byte& target);
		void SRA(Address addr);
		void SRL(Byte& target);
		void SRL(Address addr);

		void SWAP(Byte& target);
		void SWAP(Address addr);

		// Bit operations
		void BIT(Byte target, int bit);
		void BIT(Address addr, int bit);

		void SET(Byte& target, int bit);
		void SET(Address addr, int bit);

		void RES(Byte& target, int bit);
		void RES(Address addr, int bit);

		void SCF();
		void CCF();

		// Jump instructions
		void JP(Pair target);
		void JPNZ(Pair target);
		void JPZ(Pair target);
		void JPNC(Pair target);
		void JPC(Pair target);
		void JR(Byte value);

		void JRNZ(Byte value);
		void JRZ(Byte value);
		void JRNC(Byte value);
		void JRC(Byte value);
		void JPHL();

		// Function Instructions
		void CALL(Byte low, Byte high);
		void CALLNZ(Byte low, Byte high);
		void CALLZ(Byte low, Byte high);
		void CALLNC(Byte low, Byte high);
		void CALLC(Byte low, Byte high);

		void RET();
		void RETI(); // UNIMPLEMENTED
		void RETNZ();
		void RETZ();
		void RETNC();
		void RETC();

		// Miscellaneous Instructions
		void RST(Address addr);
		
		void DAA();
		void CPL();
		void NOP();

		void HALT();
		void STOP();

		// GBCPUMan
		void DI();
		void EI();
};
