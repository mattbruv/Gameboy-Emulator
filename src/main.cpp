#include "emulator.h"
#include "cpu.h"
#include "display.h"
#include <iostream>

int main(int argc, char* args[])
{
    if (argc > 1)
    {
        Emulator* emulator = new Emulator();

        emulator->memory.load_rom(args[1]);
        emulator->run();
    }

    return 0;
}
