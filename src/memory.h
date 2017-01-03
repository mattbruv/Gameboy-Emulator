#pragma once

#include "types.h"
#include "memory_controllers.h"

class Memory
{
	private:

		// Dynamic Memory Controller
		MemoryController* controller;

		// Memory Regions
		vector<Byte> VRAM;		// $8000 - $9FFF, 8kB Video RAM
		vector<Byte> OAM;		// $FE00 - $FEA0, OAM Sprite RAM
		vector<Byte> WRAM;		// $C000 - $DFFF, 8kB Working RAM
		vector<Byte> ZRAM;		// $FF80 - $FFFF, 128 bytes of RAM

		void do_dma_transfer();
		Byte get_joypad_state();

	public:

		MemoryRegister
			P1,
			DIV, TIMA, TMA, TAC,
			LCDC, STAT, SCY, SCX, LYC, LY, DMA,
			BGP, OBP0, OBP1, WY, WX,
			IF, IE;

		Byte video_mode;
		Byte joypad_buttons;
		Byte joypad_arrows;

		string rom_name;

		Memory::Memory();
		void reset();
		void load_rom(std::string location);

		Byte read(Address location);

		void write_vector(ofstream &file, vector<Byte> &vec);
		void load_vector(ifstream &file, vector<Byte> &vec);
		void save_state(ofstream &file);
		void load_state(ifstream &file);

		void write(Address location, Byte data);
		void write_zero_page(Address location, Byte data);
};
