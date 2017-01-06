#pragma once

#include "types.h"

// Abstract class that each memory controller must represent
class MemoryController
{
	protected:
		// $0000 - $7FFF, 32kB Cartridge (potentially dynamic)
		vector<Byte> CART_ROM;
		// $A000 - $BFFF, 8kB Cartridge external switchable RAM bank
		vector<Byte> ERAM;

		// Bank selectors
		Byte ROM_bank_id = 1;
		Byte RAM_bank_id = 0;

		bool RAM_bank_enabled = false;
		bool RAM_access_enabled = false;

		// Mode selector
		Byte mode = 0;
		const Byte MODE_ROM = 0;
		const Byte MODE_RAM = 1;

	public:
		void init(vector<Byte> cartridge_buffer);
		virtual Byte read(Address location) = 0;
		virtual void write(Address location, Byte data) = 0;

		// Save states
		vector<Byte> get_ram();
		void set_ram(vector<Byte> data);
		virtual void save_state(ofstream &file);
		virtual void load_state(ifstream &file);
};

// This class represents games that only use the exact 32kB of cartridge space
class MemoryController0 : public MemoryController {
	Byte read(Address location);
	void write(Address location, Byte data);
};

// MBC1 (max 2MByte ROM and/or 32KByte RAM)
class MemoryController1 : public MemoryController {
	Byte read(Address location);
	void write(Address location, Byte data);
	void save_state(ofstream &file);
	void load_state(ifstream &file);
};

// MBC2 (max 256KByte ROM and 512x4 bits RAM)
class MemoryController2 : public MemoryController {
	Byte read(Address location);
	void write(Address location, Byte data);
};

// MBC3(max 2MByte ROM and / or 32KByte RAM and Timer)
class MemoryController3 : public MemoryController {
	
	bool RTC_enabled = false;

	Byte read(Address locatison);
	void write(Address location, Byte data);
	void save_state(ofstream &file);
	void load_state(ifstream &file);
};
