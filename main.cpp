#include "emulator.h"
#include "cpu.h"
#include "display.h"

int main(int argc, char *args[])
{
	Emulator emulator;

	string name = "04-op r,imm";

	//emulator.memory.load_rom("tests/" + name + ".gb");
	emulator.memory.load_rom("tetris.gb");

	//emulator.cpu.debug();
	emulator.run();

	return 0;
}