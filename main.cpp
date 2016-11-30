#include "cpu.h"
#include "display.h"

int main(int argc, char *args[])
{
	Memory memory;
	Display display;
	CPU cpu;

	display.init(&memory);
	cpu.init(&memory, &display);

	memory.load_rom("C:\\Users\\Matt\\Desktop\\GameBoy Emulator Project\\tetris.gb");

	cpu.execute(0xFFFFF);


	return 0;
}