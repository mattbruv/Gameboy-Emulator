#include "emulator.h"
#include "cpu.h"
#include "display.h"

int main(int argc, char *args[])
{
	Emulator emulator;

	//string name = "09-op r,r";
	string name = "cpu_instrs";

	//emulator.memory.load_rom("tests/" + name + ".gb");
	emulator.memory.load_rom("roms/Tetris.gb");

	emulator.run();

	return 0;
}