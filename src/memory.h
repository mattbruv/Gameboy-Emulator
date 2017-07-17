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
		vector<Byte> ZRAM;		// $FF00 - $FFFF, 256 bytes of RAM

		// CGB Memory Banks
		vector<Byte> CGB_VRAM; // 8kB Video Ram Bank 1 for CGB
		vector<Byte> CGB_WRAM; // 32kB of additional Work RAM.. 8 banks of 4 KBytes each
		// Bank 0 is always at $C000-CFFF
		// Banks 1-7 are selected into address space $D000-DFFF

		void do_dma_transfer();
		void do_hdma_transfer();
		Byte get_joypad_state();

	public:

		MemoryRegister
			P1,
			DIV, TIMA, TMA, TAC,
			LCDC, STAT, SCY, SCX, LYC, LY, DMA,
			BGP, OBP0, OBP1, WY, WX,
			IF, IE;

		// Gameboy Color Memory Variables
		bool color_mode = false;
		vector<Byte> color_palettes;

		MemoryRegister
			SVBK, // WRAM Bank
			VBK,  // VRAM Bank
			BGPI, // Background palette index
			BGPD, // Background palette data

			// HDMA Registers
			HDMA1, // DMA Source, High
			HDMA2, // DMA Source, Low
			HDMA3, // DMA Destination, High
			HDMA4, // DMA Destination, Low
			HDMA5; // DMA Length/Mode/Start



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
