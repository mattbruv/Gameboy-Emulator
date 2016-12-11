#include "emulator.h"
#include "cpu.h"
#include "display.h"

int main(int argc, char *args[])
{
	Emulator emulator;

	string name = "03-op sp,hl";

	emulator.memory.load_rom("tests/" + name + ".gb");
	emulator.run(0xFFFFF);

	return 0;
}