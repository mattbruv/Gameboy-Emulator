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
	Byte test = (target & 0xF);
	return (target & 0xF);
}

Byte_2 combine(Byte high, Byte low)
{
	return (high << 8 | low);
}

bool between(Byte target, int low, int high)
{
	return (target >= low && target <= high);
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

// Initialize CPU internal data structures
void CPU::init()
{
	/*
		Gameboy Power Up Sequence:
		* On startup, the gameboy runs a 256 byte ROM program which is physically located inside the gameboy, seperate from cartridges.
		* It compares memory from cartridge 0x104 - 0x133 to internal memory to verify nintendo logo.
			* Nintendo logo is scrolled to middle of screen and two musical notes play
		* If any byte fails to compare, it stops comparing and halts all operations.	

		* If all above checks pass, the internal ROM is disabled and the cartridge is excecuted at 0x100 with the following
		ARBITRARY, UNEXPLAINED register values. 
	*/
	reg_A = 0x01;
	reg_B = 0x00;
	reg_C = 0x13;
	reg_D = 0x00;
	reg_E = 0xD8;
	reg_F = 0xB0;
	reg_H = 0x01;
	reg_L = 0x4D;
	reg_SP = 0xFFFE;
	reg_PC = 0x100;
}

// Reproduces the effect of a reset signal sent to the CPU
void CPU::reset()
{
}

// Start emulation of CPU
void CPU::execute(int total_iterations)
{
	// frames per second
	int framerate = 60;

	// CPU cycles to emulate per frame draw
	int cycles_per_frame = CLOCK_SPEED / framerate;

	// Current cycle in frame
	int current_cycle = 0;

	for (int i = 0; i < total_iterations; i++)
	{
		while (current_cycle < cycles_per_frame)
		{
			Opcode code = memory.read(reg_PC);
			
			if (reg_PC == 0x29D) {// 0x282A) {
				bool breakpoint = true;
			}

			if (reg_PC == 0x2B6) {
				bool breakpoint = true;
			} /**/

			parse_opcode(code);
			current_cycle += num_cycles;
			update_timers(num_cycles);
			num_cycles = 0;
		}
		// Render();
	}
}

void CPU::update_divider(int cycles)
{
	divider_counter += cycles;

	if (divider_counter >= 256) // 16384 Hz
	{
		divider_counter = 0;
		memory.DIV.set(memory.DIV.get() + 1);
	}
}

// Opcode cycle number may need adjusted, used Nintendo values
void CPU::update_timers(int cycles)
{
	update_divider(cycles);

	// This can be optimized if needed
	Byte new_freq = get_timer_frequency();

	if (timer_frequency != new_freq)
	{
		set_timer_frequency();
		timer_frequency = new_freq;
	}

	if (timer_enabled())
	{
		timer_counter -= cycles;

		// enough cpu clock cycles have happened to update timer
		if (timer_counter <= 0)
		{
			Byte timer_value = memory.TIMA.get();
			set_timer_frequency();

			// Timer will overflow, generate interrupt
			if (timer_value == 255)
			{
				memory.TIMA.set(memory.TMA.get());
				request_interrupt(INTERRUPT_TIMER);
			}
			else
			{
				memory.TIMA.set(timer_value + 1);
			}
		}
	}
}

bool CPU::timer_enabled()
{
	return memory.TAC.is_bit_set(BIT_2);
}

Byte CPU::get_timer_frequency()
{
	return (memory.TAC.get() & 0x3);
}

void CPU::set_timer_frequency()
{
	Byte frequency = get_timer_frequency();
	timer_frequency = frequency;

	switch (frequency)
	{
		// timer_counter calculated by (Clock Speed / selected frequency)
	case 0: timer_counter = 1024; break; // 4096 Hz
	case 1: timer_counter = 16; break; // 262144 Hz
	case 2: timer_counter = 64; break; // 65536 Hz
	case 3: timer_counter = 256; break; // 16384 Hz
	}
}

void CPU::request_interrupt(Byte id)
{
	memory.IF.set_bit(id);
}

void CPU::do_interrupts()
{
	// If master flag is enabled
	if (interrupt_master_enable)
	{
		// If there are any interrupts set
		if (memory.IF.get() > 0)
		{
			// Loop through each bit and call interrupt for lowest -> highest priority bits set
			for (int i = 0; i < 5; i++)
			{
				if (memory.IF.is_bit_set(i))
				{
					if (memory.IE.is_bit_set(i))
					{
						service_interrupt(i);
					}
				}
			}
		}
	}
}

void CPU::service_interrupt(Byte id)
{
	interrupt_master_enable = false;
	memory.IF.clear_bit(id);

	// Push current execution address to stack
	PUSH(high_byte(reg_PC), low_byte(reg_PC));

	switch (id)
	{
		case INTERRUPT_VBLANK: reg_PC = 0x40; break;
		case INTERRUPT_LCDC:   reg_PC = 0x48; break;
		case INTERRUPT_TIMER:  reg_PC = 0x50; break;
		case INTERRUPT_JOYPAD: reg_PC = 0x60; break;
	}
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

void CPU::op(int pc, int cycle)
{
	reg_PC += pc;
	num_cycles += cycle;
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
	Byte_Signed signed_val = (Byte_Signed) value;
	int result = reg_SP + signed_val;

	set_flag(FLAG_ZERO, false); // reset
	set_flag(FLAG_SUB, false); // reset
	set_flag(FLAG_HALF_CARRY, ((((reg_SP & 0xFFF) + (signed_val & 0xFFF)) & 0x1000) != 0)); // set if carry from bit 11
	set_flag(FLAG_CARRY, (result > 0xFFFF)); // set if carry from bit 15

	Pair(reg_H, reg_L).set(reg_SP + signed_val);
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
	set_flag(FLAG_ZERO, (((1 << bit) & ~target) != 0));
	set_flag(FLAG_HALF_CARRY, true);
	set_flag(FLAG_SUB, false);
}

void CPU::BIT(Address addr, int bit)
{
	Byte value = memory.read(addr);
	BIT(value, bit);
}

void CPU::SET(Byte& target, int bit)
{
	target = (target | (1 << bit));
}

void CPU::SET(Address addr, int bit)
{
	Byte value = memory.read(addr);
	SET(value, bit);
	memory.write(addr, value);
}

void CPU::RES(Byte& target, int bit)
{
	target = (~(1 << bit) & target);
}

void CPU::RES(Address addr, int bit)
{
	Byte value = memory.read(addr);
	RES(value, bit);
	memory.write(addr, value);
}

// Jump Operations

void CPU::JP(Pair target)
{
	reg_PC = target.address();
	op(0, 1); // Add 1 cycle when conditions are true
}

void CPU::JPNZ(Pair target)
{
	if ((reg_F & FLAG_ZERO) == 0)
		JP(target);
}

void CPU::JPZ(Pair target)
{
	if ((reg_F & FLAG_ZERO) != 0)
		JP(target);
}

void CPU::JPNC(Pair target)
{
	if ((reg_F & FLAG_CARRY) == 0)
		JP(target);
}

void CPU::JPC(Pair target)
{
	if ((reg_F & FLAG_CARRY) != 0)
		JP(target);
}

// Jumps -127 to +129 steps from current address
void CPU::JR(Byte value)
{
	Byte_Signed signed_val = ((Byte_Signed)(value));
	reg_PC += signed_val; // no reason for subtracting 2, but listed in docs
	op(0, 1); // Add 1 cycle when conditions are true
}

void CPU::JRNZ(Byte value)
{
	if ((reg_F & FLAG_ZERO) == 0)
		JR(value);
}

void CPU::JRZ(Byte value)
{
	if ((reg_F & FLAG_ZERO) != 0)
		JR(value);
}

void CPU::JRNC(Byte value)
{
	if ((reg_F & FLAG_CARRY) == 0)
		JR(value);
}

void CPU::JRC(Byte value)
{
	if ((reg_F & FLAG_CARRY) != 0)
		JR(value);
}

// Jump to address
void CPU::JPHL()
{
	reg_PC = Pair(reg_H, reg_L).address();
}

// Function Instructions
void CPU::CALL(Byte low, Byte high)
{
	memory.write(--reg_SP, high_byte(reg_PC));
	memory.write(--reg_SP, low_byte(reg_PC));

	JP(Pair(high, low));
	op(0, 3);
}

void CPU::CALLNZ(Byte low, Byte high)
{
	if ((reg_F & FLAG_ZERO) == 0)
		CALL(low, high);
}

void CPU::CALLZ(Byte low, Byte high)
{
	if ((reg_F & FLAG_ZERO) != 0)
		CALL(low, high);
}

void CPU::CALLNC(Byte low, Byte high)
{
	if ((reg_F & FLAG_CARRY) == 0)
		CALL(low, high);
}

void CPU::CALLC(Byte low, Byte high)
{
	if ((reg_F & FLAG_CARRY) != 0)
		CALL(low, high);
}

void CPU::RET()
{
	Byte low = memory.read(reg_SP++);
	Byte high = memory.read(reg_SP++);

	reg_PC = Pair(high, low).get();
	op(0, 3);
}

void CPU::RETI() // UNIMPLEMENTED
{
	RET();
	// master interrupt enable flag is returned to pre-interrupt status
}

void CPU::RETNZ()
{
	if ((reg_F & FLAG_ZERO) == 0)
		RET();
}

void CPU::RETZ()
{
	if ((reg_F & FLAG_ZERO) != 0)
		RET();
}

void CPU::RETNC()
{
	if ((reg_F & FLAG_CARRY) == 0)
		RET();
}

void CPU::RETC()
{
	if ((reg_F & FLAG_CARRY) != 0)
		RET();
}

void CPU::RST(Address addr)
{
	memory.write(--reg_SP, high_byte(reg_PC));
	memory.write(--reg_SP, low_byte(reg_PC));

	reg_PC = addr;
}

// Decimal Adjust Accumulator
// Binary coded decimal representation is used to set the contents of
// Register A to a binary coded decimal number
void CPU::DAA()
{
	Byte high = high_nibble(reg_A);
	Byte low = low_nibble(reg_A);

	bool add = ((reg_F & FLAG_SUB) == 0);
	bool carry = ((reg_F & FLAG_CARRY) != 0);
	bool half_carry = ((reg_F & FLAG_HALF_CARRY) != 0);

	if (add) // ADD, ADDC
	{
		if (!carry && !half_carry && (between(high, 0x0, 0x8) && between(low, 0xA, 0xF)))
			reg_A += 0x06;

		else if (!carry && half_carry && (between(high, 0x0, 0x9) && between(low, 0x0, 0x3)))
			reg_A += 0x06;

		else if (!carry && !half_carry && (between(high, 0xA, 0xF) && between(low, 0x0, 0x9)))
		{
			reg_A += 0x60;
			set_flag(FLAG_CARRY, true);
		}

		else if (!carry && !half_carry && (between(high, 0x9, 0xF) && between(low, 0xA, 0xF)))
		{
			reg_A += 0x66;
			set_flag(FLAG_CARRY, true);
		}

		else if (!carry && half_carry && (between(high, 0xA, 0xF) && between(low, 0x0, 0x3)))
		{
			reg_A += 0x66;
			set_flag(FLAG_CARRY, true);
		}

		else if (carry && !half_carry && (between(high, 0x0, 0x2) && between(low, 0x0, 0x9)))
			reg_A += 0x60;

		else if (carry && !half_carry && (between(high, 0x0, 0x2) && between(low, 0xA, 0xF)))
			reg_A += 0x66;

		else if (carry && half_carry && (between(high, 0x0, 0x3) && between(low, 0x0, 0x3)))
			reg_A += 0x66;
	}
	else // SUB, SUBC
	{
		if (!carry && half_carry && (between(high, 0x0, 0x8) && between(low, 0x6, 0xF)))
			reg_A += 0xFA;

		else if (carry && !half_carry && (between(high, 0x7, 0xF) && between(low, 0x0, 0x9)))
			reg_A += 0xA0;

		else if (carry && half_carry && (between(high, 0x6, 0xF) && between(low, 0x6, 0xF)))
			reg_A += 0x9A;
	}

	set_flag(FLAG_HALF_CARRY, false); // Questionable
}

void CPU::CPL()
{
	reg_A = ~reg_A;
	set_flag(FLAG_HALF_CARRY, true);
	set_flag(FLAG_SUB, true);
}

void CPU::NOP()
{
	// Hardest operation ever
}

void CPU::HALT()
{
	// System clock stopped, HALT mode is entered
	// Oscillator circuit and LCD controller continue to operate
	// RAM unchanged

	// HALT mode canceled by interrupt or reset signal
}

void CPU::STOP()
{
}

void CPU::DI()
{
	interrupt_master_enable = false;
}

void CPU::EI()
{
	interrupt_master_enable = true;
}

void CPU::debug()
{
	reg_SP = 0xFFF8;
	parse_opcode(0xF8);
}