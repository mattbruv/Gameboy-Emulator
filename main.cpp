#include "disassembler.h"
#include "memory.h"

int main()
{
	Disassembler d;
	d.load_rom("C:/Users/Matt/Desktop/GameBoy Emulator Project/PokemonRedMask.gb");
	d.disassemble();
}