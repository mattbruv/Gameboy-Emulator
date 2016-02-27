#include "cpu.h"

// Initialize CPU internal data structures
void GB_CPU::init()
{
	// Program Counter is initialized to $100
	// Stack Pointer is initialized to $FFFE
}

// Reproduces the effect of a reset signal sent to the CPU
void GB_CPU::reset()
{
}

// Start emulation of CPU
void GB_CPU::execute(int cycles)
{
}

void GB_CPU::interrupt_signal()
{
}

void GB_CPU::stop()
{
}
