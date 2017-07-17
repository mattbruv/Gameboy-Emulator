#include "emulator.h"
#include "cpu.h"
#include "display.h"

int main(int argc, char *args[])
{
	Emulator emulator;

	if (argc > 1)
	{
		emulator.memory.load_rom(args[1]);
		emulator.run();
	}
	else
		cout << "Invalid ROM path..." << endl;
		
	return 0;
}
