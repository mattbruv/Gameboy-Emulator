#include "cpu.h"

/*
 *	Helper Functions
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
	return (target & 0xF);
}

Byte_2 combine(Byte high, Byte low)
{
	return (high << 8 | low);
}

/*
 *	8-Bit Pair Helper Class
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

Address Pair::address()
{
	return combine(high, low);
}

/*
	Gameboy CPU Class
	Modified Zilog Z80
*/
CPU::CPU()
{
	init();
}

// Initialize CPU internal data structures
void CPU::init()
{
	// Program Counter is initialized to $100
	// Stack Pointer is initialized to $FFFE

	// set meaningless register values for opcode debugging
	reg_A = 0;
	reg_B = 0;
	reg_C = 0;
	reg_D = 0;
	reg_E = 0;
	reg_F = 0;
	reg_H = 0;
	reg_L = 0;
	reg_SP = 0xFFFE;
	reg_PC = 0;
}

// Reproduces the effect of a reset signal sent to the CPU
void CPU::reset()
{
}

// Start emulation of CPU
void CPU::execute(int cycles)
{
}

void CPU::interrupt_signal()
{
}

void CPU::stop()
{
}

void CPU::set_flag(int flag, bool value)
{
	if (value == true)
		reg_F |= flag;
	else
		reg_F &= ~(flag);
}

void CPU::debug()
{
	reg_A = 0x5;
	reg_SP = 0xFFF8;
	parse_opcode(0x08);
}

int CPU::parse_opcode(Opcode code)
{
	int opbytes = 1;

	Byte value = 0x00;
	Byte value2 = 0xC1;

	Byte addr1 = 0b01000000;
	Byte addr2 = 0b00000000;

	reg_H = addr1;
	reg_L = addr2;

	switch (code)
	{
		// 85
		case 0x7F: LD(reg_A, reg_A); break;
		case 0x78: LD(reg_A, reg_B); break;
		case 0x79: LD(reg_A, reg_C); break;
		case 0x7A: LD(reg_A, reg_D); break;
		case 0x7B: LD(reg_A, reg_E); break;
		case 0x7C: LD(reg_A, reg_H); break;
		case 0x7D: LD(reg_A, reg_L); break;
		case 0x47: LD(reg_B, reg_A); break;
		case 0x40: LD(reg_B, reg_B); break;
		case 0x41: LD(reg_B, reg_C); break;
		case 0x42: LD(reg_B, reg_D); break;
		case 0x43: LD(reg_B, reg_E); break;
		case 0x44: LD(reg_B, reg_H); break;
		case 0x45: LD(reg_B, reg_L); break;
		case 0x4F: LD(reg_C, reg_A); break;
		case 0x48: LD(reg_C, reg_B); break;
		case 0x49: LD(reg_C, reg_C); break;
		case 0x4A: LD(reg_C, reg_D); break;
		case 0x4B: LD(reg_C, reg_E); break;
		case 0x4C: LD(reg_C, reg_H); break;
		case 0x4D: LD(reg_C, reg_L); break;
		case 0x57: LD(reg_D, reg_A); break;
		case 0x50: LD(reg_D, reg_B); break;
		case 0x51: LD(reg_D, reg_C); break;
		case 0x52: LD(reg_D, reg_D); break;
		case 0x53: LD(reg_D, reg_E); break;
		case 0x54: LD(reg_D, reg_H); break;
		case 0x55: LD(reg_D, reg_L); break;
		case 0x5F: LD(reg_E, reg_A); break;
		case 0x58: LD(reg_E, reg_B); break;
		case 0x59: LD(reg_E, reg_C); break;
		case 0x5A: LD(reg_E, reg_D); break;
		case 0x5B: LD(reg_E, reg_E); break;
		case 0x5C: LD(reg_E, reg_H); break;
		case 0x5D: LD(reg_E, reg_L); break;
		case 0x67: LD(reg_H, reg_A); break;
		case 0x60: LD(reg_H, reg_B); break;
		case 0x61: LD(reg_H, reg_C); break;
		case 0x62: LD(reg_H, reg_D); break;
		case 0x63: LD(reg_H, reg_E); break;
		case 0x64: LD(reg_H, reg_H); break;
		case 0x65: LD(reg_H, reg_L); break;
		case 0x6F: LD(reg_L, reg_A); break;
		case 0x68: LD(reg_L, reg_B); break;
		case 0x69: LD(reg_L, reg_C); break;
		case 0x6A: LD(reg_L, reg_D); break;
		case 0x6B: LD(reg_L, reg_E); break;
		case 0x6C: LD(reg_L, reg_H); break;
		case 0x6D: LD(reg_L, reg_L); break;
		case 0x3E: LD(reg_A, value); opbytes = 2; break;
		case 0x06: LD(reg_B, value); opbytes = 2; break;
		case 0x0E: LD(reg_C, value); opbytes = 2; break;
		case 0x16: LD(reg_D, value); opbytes = 2; break;
		case 0x1E: LD(reg_E, value); opbytes = 2; break;
		case 0x26: LD(reg_H, value); opbytes = 2; break;
		case 0x2E: LD(reg_L, value); opbytes = 2; break;
		case 0x7E: LD(reg_A, Pair(reg_H, reg_L).address()); break;
		case 0x46: LD(reg_B, Pair(reg_H, reg_L).address()); break;
		case 0x4E: LD(reg_C, Pair(reg_H, reg_L).address()); break;
		case 0x56: LD(reg_D, Pair(reg_H, reg_L).address()); break;
		case 0x5E: LD(reg_E, Pair(reg_H, reg_L).address()); break;
		case 0x66: LD(reg_H, Pair(reg_H, reg_L).address()); break;
		case 0x6E: LD(reg_L, Pair(reg_H, reg_L).address()); break;
		// 86
		case 0x77: LD(Pair(reg_H, reg_L).address(), reg_A); break;
		case 0x70: LD(Pair(reg_H, reg_L).address(), reg_B); break;
		case 0x71: LD(Pair(reg_H, reg_L).address(), reg_C); break;
		case 0x72: LD(Pair(reg_H, reg_L).address(), reg_D); break;
		case 0x73: LD(Pair(reg_H, reg_L).address(), reg_E); break;
		case 0x74: LD(Pair(reg_H, reg_L).address(), reg_H); break;
		case 0x75: LD(Pair(reg_H, reg_L).address(), reg_L); break;
		case 0x36: LD(Pair(reg_H, reg_L).address(), value); opbytes = 2; break;
		case 0x0A: LD(Pair(reg_B, reg_C).address(), value); opbytes = 2; break;
		case 0x1A: LD(Pair(reg_D, reg_E).address(), value); opbytes = 2; break;
		case 0xF2: LD(reg_A, (Address)(0xFF00 + reg_C)); break;
		// 87
		case 0xE2: LD((Address)(0xFF00 + reg_C), reg_A); break;
		case 0xF0: LD(reg_A, (Address)(0xFF00 + value)); break;
		case 0xE0: LD((Address)(0xFF00 + value), reg_A); break;
		case 0xFA: LD(reg_A, Pair(value, value2).address()); opbytes = 3; break;
		// 88
		case 0xEA: LD(Pair(value, value2).address(), reg_A); opbytes = 3; break;
		case 0x2A: LD(reg_A, Pair(reg_H, reg_L).address()); Pair(reg_H, reg_L).inc(); break;
		case 0x3A: LD(reg_A, Pair(reg_H, reg_L).address()); Pair(reg_H, reg_L).dec(); break;
		case 0x02: LD(Pair(reg_B, reg_C).address(), reg_A); break;
		case 0x12: LD(Pair(reg_D, reg_E).address(), reg_A); break;
		// 89
		case 0x22: LD(Pair(reg_H, reg_L).address(), reg_A); Pair(reg_H, reg_L).inc(); break;
		case 0x32: LD(Pair(reg_H, reg_L).address(), reg_A); Pair(reg_H, reg_L).dec(); break;
		// 90
		case 0x01: LD(Pair(reg_B, reg_C), value, value2); opbytes = 3; break;
		case 0x11: LD(Pair(reg_D, reg_E), value, value2); opbytes = 3; break; // says DD in nintindo manual, assumed DE pair
		case 0x21: LD(Pair(reg_H, reg_L), value, value2); opbytes = 3; break;
		case 0x31: LD(reg_SP, value, value2); opbytes = 3; break;
		case 0xF9: LD(reg_SP, reg_H, reg_L); break;
		case 0xC5: PUSH(reg_B, reg_C); break;
		case 0xD5: PUSH(reg_D, reg_E); break;
		case 0xE5: PUSH(reg_H, reg_L); break;
		case 0xF5: PUSH(reg_A, reg_F); break;
		// 91
		case 0xC1: POP(reg_B, reg_C); break;
		case 0xD1: POP(reg_D, reg_E); break;
		case 0xE1: POP(reg_H, reg_L); break;
		case 0xF1: POP(reg_A, reg_F); break;
		case 0xF8: LDHL(value); opbytes = 2; break;
		case 0x08: LDNN(value, value2); opbytes = 3; break;
	}

	return opbytes;
}

// 8-bit loads

void CPU::LD(Byte& destination, Byte value)
{
	destination = value;
}

void CPU::LD(Byte& destination, Address addr)
{
	destination = memory.read(addr);
}

void CPU::LD(Address addr, Byte value)
{
	memory.write(addr, value);
}

// 16-bit loads

void CPU::LD(Pair reg_pair, Byte upper, Byte lower)
{
	reg_pair.set(upper, lower);
}

void CPU::LD(Byte_2& reg_pair, Byte upper, Byte lower)
{
	reg_pair = combine(upper, lower);
}

void CPU::LDHL(Byte value)
{
	// value = -128 to + 127, could this mean that we make this signed and then add?
	int result = reg_SP + value;

	set_flag(FLAG_ZERO, false); // reset
	set_flag(FLAG_SUB, false); // reset
	set_flag(FLAG_HALF_CARRY, (((reg_SP & 0xFFF) + (value & 0xFFF)) & 0x1000)); // set if carry from bit 11
	set_flag(FLAG_CARRY, (result > 0xFFFF)); // set if carry from bit 15

	Pair(reg_H, reg_L).set(reg_SP + value);
}

void CPU::LDNN(Byte low, Byte high)
{
	Byte lsb = low_byte(reg_SP);
	Byte msb = high_byte(reg_SP);

	Address addr = Pair(high, low).address();

	LD(addr++, lsb);
	LD(addr, msb);
}

// Stack Operations

void CPU::PUSH(Byte high, Byte low)
{
	memory.write(--reg_SP, high);
	memory.write(--reg_SP, low);
}

void CPU::POP(Byte& high, Byte& low)
{
	low = memory.read(reg_SP++);
	high = memory.read(reg_SP++);
}

// ALU Operations

void CPU::ADD(Byte& target, Byte value)
{
	int result = target + value;

	set_flag(FLAG_ZERO, (result == 0)); // set if result is 0
	set_flag(FLAG_SUB, false); // reset
	set_flag(FLAG_HALF_CARRY, (((target & 0xF) + (value & 0xF)) & 0x10)); // Set if carry from bit 3
	set_flag(FLAG_CARRY, (result > 0xFF)); // Set if carry from bit 7

	target += value;
}

void CPU::ADD(Byte& target, Address addr)
{
	Byte val = memory.read(addr);
	ADD(target, val);
}

void CPU::ADDC(Byte& target, Byte value)
{
	int carry = (reg_F & FLAG_CARRY) ? 1 : 0;
	ADD(target, value);
	target += carry;
}

void CPU::ADDC(Byte& target, Address addr)
{
	Byte val = memory.read(addr);
	ADDC(target, val);
}

void CPU::SUB(Byte& target, Byte value)
{
	int result = target - value;

	set_flag(FLAG_ZERO, (result == 0)); // set if result is zero
	set_flag(FLAG_SUB, true); // set
	// GB CPU manual says to set this flag if there is NO borrow from bit 4, Z80 manual says otherwise?
	// as of now it sets half carry if there was a borrow
	set_flag(FLAG_HALF_CARRY, (((target & 0xF) - (value & 0xF)) < 0));
	set_flag(FLAG_CARRY, (result < 0)); // set if borrow

	target -= value;
}

void CPU::SUB(Byte& target, Address addr)
{
	Byte val = memory.read(addr);
	SUB(target, val);
}

void CPU::SUBC(Byte& target, Byte value)
{
	int carry = (reg_F & FLAG_CARRY) ? 1 : 0;
	SUB(target, value);
	target -= carry;
}

void CPU::SUBC(Byte& target, Address addr)
{
	Byte val = memory.read(addr);
	SUBC(target, val);
}

void CPU::AND(Byte& target, Byte value)
{
	target &= value;

	set_flag(FLAG_ZERO, (target == 0));
	set_flag(FLAG_SUB, false);
	set_flag(FLAG_HALF_CARRY, true);
	set_flag(FLAG_CARRY, false);
}

void CPU::AND(Byte& target, Address addr)
{
	Byte val = memory.read(addr);
	AND(target, val);
}

void CPU::OR(Byte& target, Byte value)
{
	target |= value;

	set_flag(FLAG_ZERO, (target == 0));
	set_flag(FLAG_SUB, false);
	set_flag(FLAG_HALF_CARRY, false);
	set_flag(FLAG_CARRY, false);
}

void CPU::OR(Byte& target, Address addr)
{
	Byte val = memory.read(addr);
	OR(target, val);
}

void CPU::XOR(Byte& target, Byte value)
{
	target ^= value;

	set_flag(FLAG_ZERO, (target == 0));
	set_flag(FLAG_SUB, false);
	set_flag(FLAG_HALF_CARRY, false);
	set_flag(FLAG_CARRY, false);
}

void CPU::XOR(Byte& target, Address addr)
{
	Byte val = memory.read(addr);
	OR(target, val);
}

// Compare A with n. This is basically a A - n subtraction but the results are thrown away
void CPU::CP(Byte& target, Byte value)
{
	int result = target - value;

	set_flag(FLAG_ZERO, (result == 0)); // set if result is zero
	set_flag(FLAG_SUB, true); // set
	set_flag(FLAG_HALF_CARRY, (((target & 0xF) - (value & 0xF)) < 0));
	set_flag(FLAG_CARRY, (result < 0)); // set if borrow
}

void CPU::CP(Byte& target, Address addr)
{
	Byte val = memory.read(addr);
	CP(target, val);
}

void CPU::INC(Byte& target)
{
	int result = target + 1;

	set_flag(FLAG_ZERO, (result == 0));
	set_flag(FLAG_SUB, false);
	set_flag(FLAG_HALF_CARRY, (((target & 0xF) + 1) & 0x10));

	target++;
}

void CPU::INC(Address addr)
{
	Byte value = memory.read(addr);
	INC(value);
	memory.write(addr, value);
}

void CPU::DEC(Byte& target)
{
	int result = target - 1;

	set_flag(FLAG_ZERO, (result == 0));
	set_flag(FLAG_SUB, true);
	set_flag(FLAG_HALF_CARRY, (((target & 0xF) - 1) < 0));

	target--;
}

void CPU::DEC(Address addr)
{
	Byte value = memory.read(addr);
	DEC(value);
	memory.write(addr, value);
}