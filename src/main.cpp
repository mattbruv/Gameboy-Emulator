#include "emulator.h"
#include "cpu.h"
#include "display.h"

int main(int argc, char *args[])
{
	Emulator emulator;

	//string name = "cpu/cpu_instrs";
	//string name = "instr_timing";

	//emulator.memory.load_rom("tests/" + name + ".gb");
	//emulator.memory.load_rom("roms/Dr. Mario.gb");
	//emulator.memory.load_rom("roms/kirby.gb");
	//emulator.memory.load_rom("roms/tetris.gb");
	//emulator.memory.load_rom("roms/minesweeper.gb");
	//emulator.memory.load_rom("roms/Super Mario Land.gb");
	//emulator.memory.load_rom("roms/cASTELIAN.gb");
	//emulator.memory.load_rom("roms/Serpent.gb");
	emulator.memory.load_rom("roms/yupferris.gb");

	emulator.run();

	return 0;
}
