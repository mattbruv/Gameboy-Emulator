#include "emulator.h"
#include "cpu.h"
#include "display.h"

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout<< "Usage: " << argv[0] << "<ROM>\n";
	}
	Emulator emulator;

	emulator.memory.load_rom(argv[1]);

	emulator.run();

	return 0;
}
