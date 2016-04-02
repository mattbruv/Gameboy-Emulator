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

Byte_2 Pair::get()
{
	return address();
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
	reg_PC = 0x100;
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
	set_flag(FLAG_HALF_CARRY, ((((reg_SP & 0xFFF) + (value & 0xFFF)) & 0x1000) != 0)); // set if carry from bit 11
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
	Byte real_result = target + value;

	set_flag(FLAG_ZERO, (real_result == 0)); // set if result is 0
	set_flag(FLAG_SUB, false); // reset
	set_flag(FLAG_HALF_CARRY, ((((target & 0xF) + (value & 0xF)) & 0x10) != 0)); // Set if carry from bit 3
	set_flag(FLAG_CARRY, (result > 0xFF)); // Set if carry from bit 7

	target = real_result;
}

void CPU::ADD(Byte& target, Address addr)
{
	Byte val = memory.read(addr);
	ADD(target, val);
}

void CPU::ADC(Byte& target, Byte value)
{
	int carry = (reg_F & FLAG_CARRY) ? 1 : 0;
	ADD(target, value);
	target += carry;
}

void CPU::ADC(Byte& target, Address addr)
{
	Byte val = memory.read(addr);
	ADC(target, val);
}

void CPU::SUB(Byte& target, Byte value)
{
	int result = target - value;
	Byte real_result = target - value;

	set_flag(FLAG_ZERO, (real_result == 0)); // set if result is zero
	set_flag(FLAG_SUB, true); // set
	set_flag(FLAG_HALF_CARRY, (((target & 0xF) - (value & 0xF)) < 0)); // set if borrow from bit 4
	set_flag(FLAG_CARRY, (result < 0)); // set if borrow

	target = real_result;
}

void CPU::SUB(Byte& target, Address addr)
{
	Byte val = memory.read(addr);
	SUB(target, val);
}

void CPU::SBC(Byte& target, Byte value)
{
	int carry = (reg_F & FLAG_CARRY) ? 1 : 0;
	SUB(target, value);
	target -= carry;
}

void CPU::SBC(Byte& target, Address addr)
{
	Byte val = memory.read(addr);
	SBC(target, val);
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
	Byte result = target + 1;
	set_flag(FLAG_ZERO, (result == 0));
	set_flag(FLAG_SUB, false);
	set_flag(FLAG_HALF_CARRY, ((((target & 0xF) + 1) & 0x10) != 0));

	target = result;
}

void CPU::INC(Address addr)
{
	Byte value = memory.read(addr);
	INC(value);
	memory.write(addr, value);
}

void CPU::DEC(Byte& target)
{
	Byte result = target - 1;
	set_flag(FLAG_ZERO, (result == 0));
	set_flag(FLAG_SUB, true);
	set_flag(FLAG_HALF_CARRY, (((target & 0xF) - 1) < 0));

	target = result;
}

void CPU::DEC(Address addr)
{
	Byte value = memory.read(addr);
	DEC(value);
	memory.write(addr, value);
}

// 16-bit arithmetic

void CPU::ADD16(Byte_2 target, Byte_2 value)
{
	int result = target + value;
	set_flag(FLAG_SUB, false); // reset
	set_flag(FLAG_HALF_CARRY, ((((target & 0xFFF) + (value & 0xFFF)) & 0x1000) != 0)); // Set if carry from bit 11
	set_flag(FLAG_CARRY, (result > 0xFFFF)); // Set if carry from bit 15
}

void CPU::ADDHL(Pair reg_pair)
{
	Byte_2 target = Pair(reg_H, reg_L).get();
	Byte_2 value = reg_pair.get();
	Byte_2 result = target + value;

	ADD16(target, value); // set appropriate flags
	
	Pair(reg_H, reg_L).set(result);
}

void CPU::ADDSP(Byte value)
{
	ADD16(reg_SP, value);
	set_flag(FLAG_ZERO, false); // manual says to reset
	reg_SP += value;
}

void CPU::INC(Pair reg_pair)
{
	reg_pair.set(reg_pair.get() + 1); // manual says flags are unchanged
}

void CPU::DEC(Pair reg_pair)
{
	reg_pair.set(reg_pair.get() - 1); // manual says flags are unchanged
}

// Rotate Shift Instructions

// Rotate 1-bit Left
void CPU::RL(Byte& target, bool carry, bool zero_flag)
{
	int bit7 = ((target & 0x80) != 0);
	target = target << 1;

	target |= (carry) ? ((reg_F & FLAG_CARRY) != 0) : bit7;

	set_flag(FLAG_ZERO, ((zero_flag) ? (target == 0) : false));
	set_flag(FLAG_SUB, false);
	set_flag(FLAG_HALF_CARRY, false);
	set_flag(FLAG_CARRY, (bit7 != 0));
}

void CPU::RL(Address addr, bool carry)
{
	Byte value = memory.read(addr);
	RL(value, carry, true);
	memory.write(addr, value);
}

void CPU::RR(Byte& target, bool carry, bool zero_flag)
{
	int bit1 = ((target & 0x1) != 0);
	target = target >> 1;

	target |= (carry) ? (((reg_F & FLAG_CARRY) != 0) << 7) : (bit1 << 7);

	set_flag(FLAG_ZERO, ((zero_flag) ? (target == 0) : false));
	set_flag(FLAG_SUB, false);
	set_flag(FLAG_HALF_CARRY, false);
	set_flag(FLAG_CARRY, (bit1 != 0));
}

void CPU::RR(Address addr, bool carry)
{
	Byte value = memory.read(addr);
	RR(value, carry, true);
	memory.write(addr, value);
}

// same as shift left but bit 0 is reset
void CPU::SLA(Byte& target)
{
	RL(target, true, true);
}

void CPU::SLA(Address addr)
{
	Byte value = memory.read(addr);
	SLA(value);
	memory.write(addr, value);
}

// same as shift right but bit 7 is unchanged
void CPU::SRA(Byte& target)
{
	// content of bit 7 is unchanged
	int bit7 = ((target & 0x80) != 0);
	RR(target, true);
	target |= (bit7 << 7);
	set_flag(FLAG_ZERO, (target == 0));
}

void CPU::SRA(Address addr)
{
	Byte value = memory.read(addr);
	SRA(value);
	memory.write(addr, value);
}

// same as shift right but bit 7 is reset
void CPU::SRL(Byte& target)
{
	RR(target, true, true);
}

void CPU::SRL(Address addr)
{
	Byte value = memory.read(addr);
	SRL(value);
	memory.write(addr, value);
}

// swap high nibble with low nibble
void CPU::SWAP(Byte& target)
{
	target = ((target >> 4) | (target << 4));
	set_flag(FLAG_ZERO, (target == 0));
}

void CPU::SWAP(Address addr)
{
	Byte value = memory.read(addr);
	SWAP(value);
	memory.write(addr, value);
}

// Bit Operations

void CPU::BIT(Byte target, int bit)
{
	set_flag(FLAG_ZERO, (~((1 << bit) & target) != 0));
	set_flag(FLAG_HALF_CARRY, true);
	set_flag(FLAG_SUB, false);
}

void CPU::parse_bit_op(Opcode code)
{
	switch (code)
	{
		case 0x07: RL(reg_A, false, true); break;
		case 0x00: RL(reg_B, false, true); break;
		case 0x01: RL(reg_C, false, true); break;
		case 0x02: RL(reg_D, false, true); break;
		case 0x03: RL(reg_E, false, true); break;
		case 0x04: RL(reg_H, false, true); break;
		case 0x05: RL(reg_L, false, true); break;
		case 0x06: RL(Pair(reg_H, reg_L).address(), false); break;
		case 0x17: RL(reg_A, true, true); break;
		case 0x10: RL(reg_B, true, true); break;
		case 0x11: RL(reg_C, true, true); break;
		case 0x12: RL(reg_D, true, true); break;
		case 0x13: RL(reg_E, true, true); break;
		case 0x14: RL(reg_H, true, true); break;
		case 0x15: RL(reg_L, true, true); break;
		case 0x16: RL(Pair(reg_H, reg_L).address(), true); break;

		case 0x0F: RR(reg_A, false, true); break;
		case 0x08: RR(reg_B, false, true); break;
		case 0x09: RR(reg_C, false, true); break;
		case 0x0A: RR(reg_D, false, true); break;
		case 0x0B: RR(reg_E, false, true); break;
		case 0x0C: RR(reg_H, false, true); break;
		case 0x0D: RR(reg_L, false, true); break;
		case 0x0E: RR(Pair(reg_H, reg_L).address(), false); break;
		case 0x1F: RR(reg_A, true, true); break;
		case 0x18: RR(reg_B, true, true); break;
		case 0x19: RR(reg_C, true, true); break;
		case 0x1A: RR(reg_D, true, true); break;
		case 0x1B: RR(reg_E, true, true); break;
		case 0x1C: RR(reg_H, true, true); break;
		case 0x1D: RR(reg_L, true, true); break;
		case 0x1E: RR(Pair(reg_H, reg_L).address(), false); break; // this could actually have a different beginning opcode, check manual

		case 0x27: SLA(reg_A); break;
		case 0x20: SLA(reg_B); break;
		case 0x21: SLA(reg_C); break;
		case 0x22: SLA(reg_D); break;
		case 0x23: SLA(reg_E); break;
		case 0x24: SLA(reg_H); break;
		case 0x25: SLA(reg_L); break;
		case 0x26: SLA(Pair(reg_H, reg_L).address()); break; // this could actually have a different beginning opcode, check manual

		case 0x2F: SRA(reg_A); break;
		case 0x28: SRA(reg_B); break;
		case 0x29: SRA(reg_C); break;
		case 0x2A: SRA(reg_D); break;
		case 0x2B: SRA(reg_E); break;
		case 0x2C: SRA(reg_H); break;
		case 0x2D: SRA(reg_L); break;
		case 0x2E: SRA(Pair(reg_H, reg_L).address()); break;

		case 0x3F: SRL(reg_A); break;
		case 0x38: SRL(reg_B); break;
		case 0x39: SRL(reg_C); break;
		case 0x3A: SRL(reg_D); break;
		case 0x3B: SRL(reg_E); break;
		case 0x3C: SRL(reg_H); break;
		case 0x3D: SRL(reg_L); break;
		case 0x3E: SRL(Pair(reg_H, reg_L).address()); break;

		case 0x37: SWAP(reg_A); break;
		case 0x30: SWAP(reg_B); break;
		case 0x31: SWAP(reg_C); break;
		case 0x32: SWAP(reg_D); break;
		case 0x33: SWAP(reg_E); break;
		case 0x34: SWAP(reg_H); break;
		case 0x35: SWAP(reg_L); break;
		case 0x36: SWAP(Pair(reg_H, reg_L).address()); break;

		case 0x47: BIT(reg_A, 0); break;
		case 0x4F: BIT(reg_A, 1); break;
		case 0x57: BIT(reg_A, 2); break;
		case 0x5F: BIT(reg_A, 3); break;
		case 0x67: BIT(reg_A, 4); break;
		case 0x6F: BIT(reg_A, 5); break;
		case 0x77: BIT(reg_A, 6); break;
		case 0x7F: BIT(reg_A, 7); break;
		case 0x40: BIT(reg_B, 0); break;
		case 0x48: BIT(reg_B, 1); break;
		case 0x50: BIT(reg_B, 2); break;
		case 0x58: BIT(reg_B, 3); break;
		case 0x60: BIT(reg_B, 4); break;
		case 0x68: BIT(reg_B, 5); break;
		case 0x70: BIT(reg_B, 6); break;
		case 0x78: BIT(reg_B, 7); break;
		case 0x41: BIT(reg_C, 0); break;
		case 0x49: BIT(reg_C, 1); break;
		case 0x51: BIT(reg_C, 2); break;
		case 0x59: BIT(reg_C, 3); break;
		case 0x61: BIT(reg_C, 4); break;
		case 0x69: BIT(reg_C, 5); break;
		case 0x71: BIT(reg_C, 6); break;
		case 0x79: BIT(reg_C, 7); break;
		case 0x42: BIT(reg_D, 0); break;
		case 0x4A: BIT(reg_D, 1); break;
		case 0x52: BIT(reg_D, 2); break;
		case 0x5A: BIT(reg_D, 3); break;
		case 0x62: BIT(reg_D, 4); break;
		case 0x6A: BIT(reg_D, 5); break;
		case 0x72: BIT(reg_D, 6); break;
		case 0x7A: BIT(reg_D, 7); break;
		case 0x43: BIT(reg_E, 0); break;
		case 0x4B: BIT(reg_E, 1); break;
		case 0x53: BIT(reg_E, 2); break;
		case 0x5B: BIT(reg_E, 3); break;
		case 0x63: BIT(reg_E, 4); break;
		case 0x6B: BIT(reg_E, 5); break;
		case 0x73: BIT(reg_E, 6); break;
		case 0x7B: BIT(reg_E, 7); break;
		case 0x44: BIT(reg_H, 0); break;
		case 0x4C: BIT(reg_H, 1); break;
		case 0x54: BIT(reg_H, 2); break;
		case 0x5C: BIT(reg_H, 3); break;
		case 0x64: BIT(reg_H, 4); break;
		case 0x6C: BIT(reg_H, 5); break;
		case 0x74: BIT(reg_H, 6); break;
		case 0x7C: BIT(reg_H, 7); break;
		case 0x45: BIT(reg_L, 0); break;
		case 0x4D: BIT(reg_L, 1); break;
		case 0x55: BIT(reg_L, 2); break;
		case 0x5D: BIT(reg_L, 3); break;
		case 0x65: BIT(reg_L, 4); break;
		case 0x6D: BIT(reg_L, 5); break;
		case 0x75: BIT(reg_L, 6); break;
		case 0x7D: BIT(reg_L, 7); break;
	}
}

int CPU::parse_opcode(Opcode code)
{
	int opbytes = 1;

	Byte value = 0x37;
	Byte value2 = 0;

	// REG_D could possibly be incorrect, assumed current value from manual to match GBCPUman
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
		// 92
		case 0x87: ADD(reg_A, reg_A); break;
		case 0x80: ADD(reg_A, reg_B); break;
		case 0x81: ADD(reg_A, reg_C); break;
		case 0x82: ADD(reg_A, reg_D); break;
		case 0x83: ADD(reg_A, reg_E); break;
		case 0x84: ADD(reg_A, reg_H); break;
		case 0x85: ADD(reg_A, reg_L); break;
		case 0xC6: ADD(reg_A, value); opbytes = 2; break;
		case 0x86: ADD(reg_A, Pair(reg_H, reg_L).address()); break;
		case 0x8F: ADC(reg_A, reg_A); break;
		case 0x88: ADC(reg_A, reg_B); break;
		case 0x89: ADC(reg_A, reg_C); break;
		case 0x8A: ADC(reg_A, reg_D); break;
		case 0x8B: ADC(reg_A, reg_E); break;
		case 0x8C: ADC(reg_A, reg_H); break;
		case 0x8D: ADC(reg_A, reg_L); break;
		case 0xCE: ADC(reg_A, value); opbytes = 2; break;
		case 0x8E: ADC(reg_A, Pair(reg_H, reg_L).address()); break;
		// 93
		case 0x97: SUB(reg_A, reg_A); break;
		case 0x90: SUB(reg_A, reg_B); break;
		case 0x91: SUB(reg_A, reg_C); break;
		case 0x92: SUB(reg_A, reg_D); break;
		case 0x93: SUB(reg_A, reg_E); break;
		case 0x94: SUB(reg_A, reg_H); break;
		case 0x95: SUB(reg_A, reg_L); break;
		case 0xD6: SUB(reg_A, value); opbytes = 2; break;
		case 0x96: SUB(reg_A, Pair(reg_H, reg_L).address()); break;
		case 0x9F: SBC(reg_A, reg_A); break;
		case 0x98: SBC(reg_A, reg_B); break;
		case 0x99: SBC(reg_A, reg_C); break;
		case 0x9A: SBC(reg_A, reg_D); break;
		case 0x9B: SBC(reg_A, reg_E); break;
		case 0x9C: SBC(reg_A, reg_H); break;
		case 0x9D: SBC(reg_A, reg_L); break;
		case 0xDE: SBC(reg_A, value); opbytes = 2; break;
		case 0x9E: SBC(reg_A, Pair(reg_H, reg_L).address()); break;
		// 94
		case 0xA7: AND(reg_A, reg_A); break;
		case 0xA0: AND(reg_A, reg_B); break;
		case 0xA1: AND(reg_A, reg_C); break;
		case 0xA2: AND(reg_A, reg_D); break;
		case 0xA3: AND(reg_A, reg_E); break;
		case 0xA4: AND(reg_A, reg_H); break;
		case 0xA5: AND(reg_A, reg_L); break;
		case 0xE6: AND(reg_A, value); break;
		case 0xA6: AND(reg_A, Pair(reg_H, reg_L).address()); break;
		case 0xB7: OR(reg_A, reg_A); break;
		case 0xB0: OR(reg_A, reg_B); break;
		case 0xB1: OR(reg_A, reg_C); break;
		case 0xB2: OR(reg_A, reg_D); break;
		case 0xB3: OR(reg_A, reg_E); break;
		case 0xB4: OR(reg_A, reg_H); break;
		case 0xB5: OR(reg_A, reg_L); break;
		case 0xF6: OR(reg_A, value); opbytes = 2; break;
		case 0xB6: OR(reg_A, Pair(reg_H, reg_L).address()); break;
		case 0xAF: XOR(reg_A, reg_A); break;
		case 0xA8: XOR(reg_A, reg_B); break;
		case 0xA9: XOR(reg_A, reg_C); break;
		case 0xAA: XOR(reg_A, reg_D); break;
		case 0xAB: XOR(reg_A, reg_E); break;
		case 0xAC: XOR(reg_A, reg_H); break;
		case 0xAD: XOR(reg_A, reg_L); break;
		case 0xEE: XOR(reg_A, value); opbytes = 2; break;
		case 0xAE: XOR(reg_A, Pair(reg_H, reg_L).address()); break;
		// 95 - 96
		case 0xBF: CP(reg_A, reg_A); break;
		case 0xB8: CP(reg_A, reg_B); break;
		case 0xB9: CP(reg_A, reg_C); break;
		case 0xBA: CP(reg_A, reg_D); break;
		case 0xBB: CP(reg_A, reg_E); break;
		case 0xBC: CP(reg_A, reg_H); break;
		case 0xBD: CP(reg_A, reg_L); break;
		case 0xFE: CP(reg_A, value); opbytes = 2; break;
		case 0xBE: CP(reg_A, Pair(reg_H, reg_L).address()); break;
		case 0x3C: INC(reg_A); break;
		case 0x04: INC(reg_B); break;
		case 0x0C: INC(reg_C); break;
		case 0x14: INC(reg_D); break;
		case 0x1C: INC(reg_E); break;
		case 0x24: INC(reg_H); break;
		case 0x2C: INC(reg_L); break;
		case 0x34: INC(Pair(reg_H, reg_L).address()); break;
		case 0x3D: DEC(reg_A); break;
		case 0x05: DEC(reg_B); break;
		case 0x0D: DEC(reg_C); break;
		case 0x15: DEC(reg_D); break;
		case 0x1D: DEC(reg_E); break;
		case 0x25: DEC(reg_H); break;
		case 0x2D: DEC(reg_L); break;
		case 0x35: DEC(Pair(reg_H, reg_L).address()); break;
		// 97
		case 0x09: ADDHL(Pair(reg_B, reg_C)); break;
		case 0x19: ADDHL(Pair(reg_D, reg_E)); break;
		case 0x29: ADDHL(Pair(reg_H, reg_L)); break;
		case 0x39: ADDHL(Pair(reg_A, reg_F)); break;
		case 0xE8: ADDSP(value); opbytes = 2; break;
		case 0x03: INC(Pair(reg_B, reg_C)); break;
		case 0x13: INC(Pair(reg_D, reg_E)); break;
		case 0x23: INC(Pair(reg_H, reg_L)); break;
		case 0x33: INC(Pair(reg_A, reg_F)); break;
		case 0x0B: DEC(Pair(reg_B, reg_C)); break;
		case 0x1B: DEC(Pair(reg_D, reg_E)); break;
		case 0x2B: DEC(Pair(reg_H, reg_L)); break;
		case 0x3B: DEC(Pair(reg_A, reg_F)); break;
		// 98
		case 0x07: RL(reg_A, true); break; // manual shows the logic switched for these functions
		case 0x17: RL(reg_A, false); break; // implementation matches expected output for these two
		case 0x0F: RR(reg_A, false); break; // implementation matches expected output when these two instructions are switched
		case 0x1F: RR(reg_A, true); break;
		// 99, 100!
		case 0xCB: parse_bit_op(value); opbytes = 2; break;

	}

	return opbytes;
}

void CPU::debug()
{
	reg_A = 0xF0;
	set_flag(FLAG_CARRY, false);
	parse_opcode(0xCB);
}