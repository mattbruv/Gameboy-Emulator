#include "memory_controllers.h"

void MemoryController::init(vector<Byte> cartridge_buffer)
{
	CART_ROM = cartridge_buffer;
	ERAM = vector<Byte>(0x8000); // $A000 - $BFFF, 8kB switchable RAM bank, size liable to change in future
}

vector<Byte> MemoryController::get_ram()
{
	return ERAM;
}

void MemoryController::set_ram(vector<Byte> data)
{
	ERAM = data;
}

void MemoryController::save_state(ofstream &file) {
	cout << "did nothing" << endl;
}
void MemoryController::load_state(ifstream &file) {
	cout << "did nothing" << endl;
}

/*
	MC0 represents games that use exactly 32kB of space
	and don't have memory controllers
*/
Byte MemoryController0::read(Address location)
{
	if (location >= 0x0000 && location <= 0x7FFF)
		return CART_ROM[location];
	else if (location >= 0xA000 && location <= 0xBFFF)
		return ERAM[location & 0x1FFF];
	else
		return 0x00;
}

void MemoryController0::write(Address location, Byte data)
{
	if (location >= 0xA000 && location <= 0xBFFF)
		ERAM[location & 0x1FFF] = data;
}

/*
	Memory Controller 1
*/
Byte MemoryController1::read(Address location)
{
	// ROM bank 0 (read only)
	if (location >= 0x0000 && location <= 0x3FFF)
	{
		return CART_ROM[location];
	}
	// ROM banks 01-7F (read only)
	else if (location >= 0x4000 && location <= 0x7FFF)
	{
		// only ROM banks 0x00 - 0x1F can be used during mode 1
		Byte temp_id = ROM_bank_id;

		int offset = location - 0x4000;
		int lookup = (temp_id * 0x4000) + offset;

		return CART_ROM[lookup];
	}
	// RAM banks 00 - 03, if any (read/write)
	else if (location >= 0xA000 && location <= 0xBFFF)
	{
		if (RAM_access_enabled == false)
			return 0xFF;

		// only RAM bank 0 can be used during ROM mode
		Byte temp_id = (RAM_bank_enabled) ? RAM_bank_id : 0x00;

		int offset = location - 0xA000;
		int lookup = (temp_id * 0x2000) + offset;

		return ERAM[lookup];
	}
}

void MemoryController1::write(Address location, Byte data)
{
	// RAM enable (write only)
	if (location >= 0x0000 && location <= 0x1FFF)
	{
		// Any value with 0x0A in lower 4 bits enables, everything else disables
		RAM_access_enabled = ((data & 0x0A) > 0) ? true : false;
	}
	// ROM bank id low bits select (write only)
	else if (location >= 0x2000 && location <= 0x3FFF)
	{
		// bottom 5 bits represent bank number from 0x00 -> 0x1F
		Byte bank_id = data & 0x1F;

		ROM_bank_id = (ROM_bank_id & 0xE0) | bank_id;

		// Prevent bank zero from being accessed
		// TODO: may need to adjust this to include other banks
		switch (ROM_bank_id)
		{
			case 0x00:
			case 0x20:
			case 0x40:
			case 0x60:
				ROM_bank_id++;
				break;
		}
	}
	// RAM bank id, or upper bits of ROM bank id
	else if (location >= 0x4000 && location <= 0x5FFF)
	{
		// extract bottom 2 bits
		Byte bank_id = data & 0x03;

		// data represents RAM bank ID
		if (RAM_bank_enabled)
		{
			RAM_bank_id = bank_id;
		}
		// data represents top bits of ROM bank ID
		else
		{
			ROM_bank_id = ROM_bank_id | (bank_id << 5);

			// Adjust bank ID to prevent certain banks from being accessed
			switch (ROM_bank_id)
			{
				case 0x00:
				case 0x20:
				case 0x40:
				case 0x60:
					ROM_bank_id++;
					break;
			}
		}
	}
	// Bank selector
	else if (location >= 0x6000 && location <= 0x7FFF)
	{
		RAM_bank_enabled = is_bit_set(data, BIT_0);
	}
	// RAM banks 00 - 03, if any (read/write)
	else if (location >= 0xA000 && location <= 0xBFFF)
	{
		if (RAM_access_enabled)
		{
			int offset = location - 0xA000;
			int lookup = (RAM_bank_id * 0x2000) + offset;

			ERAM[lookup] = data;
		}
	}
}

void MemoryController1::save_state(ofstream &file)
{
	file.write((char*)&ROM_bank_id, sizeof(ROM_bank_id));
	file.write((char*)&RAM_bank_id, sizeof(RAM_bank_id));
	file.write((char*)&RAM_bank_enabled, sizeof(RAM_bank_enabled));
	file.write((char*)&RAM_access_enabled, sizeof(RAM_access_enabled));
	file.write((char*)&mode, sizeof(mode));

	cout << "wrote registers" << endl;
}

void MemoryController1::load_state(ifstream &file)
{
	file.read((char*)&ROM_bank_id, sizeof(ROM_bank_id));
	file.read((char*)&RAM_bank_id, sizeof(RAM_bank_id));
	file.read((char*)&RAM_bank_enabled, sizeof(RAM_bank_enabled));
	file.read((char*)&RAM_access_enabled, sizeof(RAM_access_enabled));
	file.read((char*)&mode, sizeof(mode));
	cout << "read registers" << endl;
}

/*
	Memory Controller 2
*/
Byte MemoryController2::read(Address location) { return 0; }
void MemoryController2::write(Address location, Byte data) {}

/*
	Memory Controller 3
*/
Byte MemoryController3::read(Address location)
{
	// ROM bank 0 (read only)
	if (location >= 0x0000 && location <= 0x3FFF)
	{
		return CART_ROM[location];
	}
	// ROM banks 01-7F (read only)
	else if (location >= 0x4000 && location <= 0x7FFF)
	{
		int offset = location - 0x4000;
		int lookup = (ROM_bank_id * 0x4000) + offset;

		return CART_ROM[lookup];
	}
	// RAM banks 00 - 03, if any (read/write)
	else if (location >= 0xA000 && location <= 0xBFFF)
	{
		if (RTC_enabled)
			return 0x00;

		if (RAM_access_enabled == false)
			return 0xFF;

		int offset = location - 0xA000;
		int lookup = (RAM_bank_id * 0x2000) + offset;

		return ERAM[lookup];
	}
}

void MemoryController3::write(Address location, Byte data)
{
	if (location >= 0x0000 && location <= 0x1FFF)
	{
		// Any value with 0x0A in lower 4 bits enables, everything else disables
		if ((data & 0x0A) > 0)
		{
			RAM_access_enabled = true;
			RTC_enabled = true;
		}
		else
		{
			RAM_access_enabled = false;
			RTC_enabled = false;
		}
	}
	else if (location >= 0x2000 && location <= 0x3FFF)
	{
		// bits 0-6 bits represent bank number from 0x00 -> 0x1F
		ROM_bank_id = data & 0x7F;

		if (ROM_bank_id == 0)
			ROM_bank_id++;
	}
	else if (location >= 0x4000 && location <= 0x5FFF)
	{
		// RAM bank
		if (data <= 0x3)
		{
			RTC_enabled = false;
			RAM_bank_id = data;
		}
		// RTC mapped
		else if (data >= 0x08 && data <= 0x0C)
		{
			RTC_enabled = true;
		}
	}
	else if (location >= 0x6000 && location <= 0x7FFF)
	{
		// TODO: Latch clock data
	}
	else if (location >= 0xA000 && location <= 0xBFFF)
	{
		// writing to RAM
		if (!RTC_enabled)
		{
			if (!RAM_access_enabled)
				return;

			int offset = location - 0xA000;
			int lookup = (RAM_bank_id * 0x2000) + offset;

			ERAM[lookup] = data;
		}
		else
		{
			// TODO: RTC writing
		}
	}
}

void MemoryController3::save_state(ofstream &file)
{
	file.write((char*)&ROM_bank_id, sizeof(ROM_bank_id));
	file.write((char*)&RAM_bank_id, sizeof(RAM_bank_id));
	file.write((char*)&RAM_bank_enabled, sizeof(RAM_bank_enabled));
	file.write((char*)&RAM_access_enabled, sizeof(RAM_access_enabled));
	file.write((char*)&mode, sizeof(mode));
	file.write((char*)&RTC_enabled, sizeof(RTC_enabled));
	cout << "wrote registers" << endl;
}

void MemoryController3::load_state(ifstream &file)
{
	file.read((char*)&ROM_bank_id, sizeof(ROM_bank_id));
	file.read((char*)&RAM_bank_id, sizeof(RAM_bank_id));
	file.read((char*)&RAM_bank_enabled, sizeof(RAM_bank_enabled));
	file.read((char*)&RAM_access_enabled, sizeof(RAM_access_enabled));
	file.read((char*)&mode, sizeof(mode));
	file.read((char*)&RTC_enabled, sizeof(RTC_enabled));
	cout << "read registers" << endl;
}