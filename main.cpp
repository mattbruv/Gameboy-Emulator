#include "cpu.h"

int main()
{
	Memory ram;
	ram.load_rom("C:\\Users\\Matt\\Desktop\\GameBoy Emulator Project\\tetris.gb");

	CPU cpu(ram);

	cpu.execute(0xFFFFF);

	//cpu.debug();
}