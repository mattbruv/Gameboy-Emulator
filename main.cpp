#include "cpu.h"
#include "display.h"

int main(int argc, char *args[])
{
	Memory ram;
	ram.load_rom("C:\\Users\\Matt\\Desktop\\GameBoy Emulator Project\\tetris.gb");

	CPU cpu(ram);

	cpu.execute(0xFFFFF);

	return 0;
	
	/*cpu.debug(); */
}