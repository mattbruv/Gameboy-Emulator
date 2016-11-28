#include "cpu.h"
#include "display.h"

int main(int argc, char *args[])
{
	Memory ram;
	ram.load_rom("C:\\Users\\Matt\\Desktop\\GameBoy Emulator Project\\tetris.gb");
	CPU cpu(ram);

	/*
	
		TODO:

		Change CPU, Memory, and Display to use Singleton pattern
		That way I can refrence each by creating a new variable
		and not having to worry about passing by reference, etc.

	
	
	*/

	cpu.execute(0xFFFF);

	return 0;
}