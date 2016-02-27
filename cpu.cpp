#include "cpu.h"

// Initialize CPU internal data structures
void CPU::init()
{
	// Program Counter is initialized to $100
	// Stack Pointer is initialized to $FFFE
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
