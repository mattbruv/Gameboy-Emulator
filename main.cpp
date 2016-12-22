#include "emulator.h"
#include "cpu.h"
#include "display.h"

int main(int argc, char *args[])
{
	Emulator emulator;

	//string name = "CPU - PASSED/02-interrupts";
	//string name = "instr_timing";

	//emulator.memory.load_rom("tests/" + name + ".gb");
	//emulator.memory.load_rom("roms/Dr. Mario.gb");
	emulator.memory.load_rom("roms/tetris.gb");
	//emulator.memory.load_rom("roms/minesweeper.gb");
	//emulator.memory.load_rom("roms/cASTELIAN.gb");
	//emulator.memory.load_rom("roms/PokemonRed.gb");

	emulator.run();

	return 0;
}