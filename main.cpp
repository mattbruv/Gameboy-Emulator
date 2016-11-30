#include "emulator.h"
#include "cpu.h"
#include "display.h"

int main(int argc, char *args[])
{
	Emulator emulator;
	emulator.memory.load_rom("C:\\Users\\Matt\\Desktop\\GameBoy Emulator Project\\tetris.gb");
	emulator.run(0xFFFFF);

	return 0;
}