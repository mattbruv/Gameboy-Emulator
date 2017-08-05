#include "memory.h"

Memory::Memory()
{
	// Initialize GB Memory
	WRAM = vector<Byte>(0x2000); // $C000 - $DFFF, 8kB Working RAM
	ZRAM = vector<Byte>(0x0100); // $FF00 - $FFFF, 256 bytes of RAM
	VRAM = vector<Byte>(0x2000); // $8000 - $9FFF, 8kB Video RAM
	OAM  = vector<Byte>(0x0100); // $FE00 - $FEFF, OAM Sprite RAM, IO RAM

	// Initialize Color Gameboy Memory
	CGB_WRAM = vector<Byte>(0x8000); // $D000 - $DFFF 32kB of 8kB RAM Banks
	CGB_VRAM = vector<Byte>(0x2000); // 8kB of additional video RAM
	color_palettes = vector<Byte>(0x40); // 64 bytes of color data

	// Initialize Memory Register objects for easy reference
	P1   = MemoryRegister(&ZRAM[0x00]);
	DIV  = MemoryRegister(&ZRAM[0x04]);
	TIMA = MemoryRegister(&ZRAM[0x05]);
	TMA  = MemoryRegister(&ZRAM[0x06]);
	TAC  = MemoryRegister(&ZRAM[0x07]);
	LCDC = MemoryRegister(&ZRAM[0x40]);
	STAT = MemoryRegister(&ZRAM[0x41]);
	SCY  = MemoryRegister(&ZRAM[0x42]);
	SCX  = MemoryRegister(&ZRAM[0x43]);
	LY   = MemoryRegister(&ZRAM[0x44]);
	LYC  = MemoryRegister(&ZRAM[0x45]);
	DMA  = MemoryRegister(&ZRAM[0x46]);
	BGP  = MemoryRegister(&ZRAM[0x47]);
	OBP0 = MemoryRegister(&ZRAM[0x48]);
	OBP1 = MemoryRegister(&ZRAM[0x49]);
	WY   = MemoryRegister(&ZRAM[0x4A]);
	WX   = MemoryRegister(&ZRAM[0x4B]);
	IF   = MemoryRegister(&ZRAM[0x0F]);
	IE   = MemoryRegister(&ZRAM[0xFF]);

	// Color Gameboy
	SVBK = MemoryRegister(&ZRAM[0x70]);
	VBK  = MemoryRegister(&ZRAM[0x4F]);
	BGPI = MemoryRegister(&ZRAM[0x68]);
	BGPD = MemoryRegister(&ZRAM[0x69]);

	HDMA1 = MemoryRegister(&ZRAM[0x51]);
	HDMA2 = MemoryRegister(&ZRAM[0x52]);
	HDMA3 = MemoryRegister(&ZRAM[0x53]);
	HDMA4 = MemoryRegister(&ZRAM[0x54]);
	HDMA5 = MemoryRegister(&ZRAM[0x55]);

	reset();
}

void Memory::reset()
{
	fill(WRAM.begin(), WRAM.end(), 0);
	fill(ZRAM.begin(), ZRAM.end(), 0);
	fill(VRAM.begin(), VRAM.end(), 0);
	fill(OAM.begin(), OAM.end(), 0);

	// The following memory locations are set to the following values after gameboy BIOS runs
	P1.set(0x00);
	DIV.set(0x00);
	TIMA.set(0x00);
	TMA.set(0x00);
	TAC.set(0x00);
	LCDC.set(0x91);
	SCY.set(0x00);
	SCX.set(0x00);
	LYC.set(0x00);
	BGP.set(0xFC);
	OBP0.set(0xFF);
	OBP1.set(0xFF);
	WY.set(0x00);
	WX.set(0x00);
	IF.set(0x00);
	IE.set(0x00);

	// Initialize input to HIGH state (unpressed)
	joypad_buttons = 0xF;
	joypad_arrows  = 0xF;
}

void Memory::load_rom(std::string location)
{
	ifstream input(location, ios::binary);
	vector<Byte> buffer((istreambuf_iterator<char>(input)), (istreambuf_iterator<char>()));

	// print cartrige data
	string title = "";

	for (int i = 0x0134; i <= 0x142; i++)
	{
		Byte character = buffer[i];
		if (character == 0)
			break;
		else
			title.push_back(tolower(character));
	}

	rom_name = title;

	cout << "Title: " << title << endl;
	Byte gb_type = buffer[0x0143];

	cout << "Gameboy Type: " << ((gb_type == 0x80) ? "GB Color" : "GB") << endl;
	// Let the memory class know which mode the GB is in
	color_mode = (gb_type == 0x80) ? true : false;

	Byte functions = buffer[0x0146];
	cout << "Use " << ((functions == 0x3) ? "Super " : "") << "Gameboy functions" << endl;

	string cart_types[0x100];
	cart_types[0x0] = "ROM ONLY";
	cart_types[0x1] = "ROM+MBC1";
	cart_types[0x2] = "ROM+MBC1+RAM";
	cart_types[0x3] = "ROM+MBC1+RAM+BATT";
	cart_types[0x5] = "ROM+MBC2";
	cart_types[0x6] = "ROM+MBC2+BATTERY";
	cart_types[0x8] = "ROM+RAM";
	cart_types[0x9] = "ROM+RAM+BATTERY";
	cart_types[0xB] = "ROM+MMM01";
	cart_types[0xC] = "ROM+MMM01+SRAM";
	cart_types[0xD] = "ROM+MMM01+SRAM+BATT";
	cart_types[0xF] = "ROM+MBC3+TIMER+BATT";
	cart_types[0x10] = "ROM+MBC3+TIMER+RAM+BATT";
	cart_types[0x11] = "ROM+MBC3";
	cart_types[0x12] = "ROM+MBC3+RAM";
	cart_types[0x13] = "ROM+MBC3+RAM+BATT";
	cart_types[0x19] = "ROM+MBC5";
	cart_types[0x1A] = "ROM+MBC5+RAM";
	cart_types[0x1B] = "ROM+MBC5+RAM+BATT";
	cart_types[0x1C] = "ROM+MBC5+RUMBLE";
	cart_types[0x1D] = "ROM+MBC5+RUMBLE+SRAM";
	cart_types[0x1E] = "ROM+MBC5+RUMBLE+SRAM+BATT";
	cart_types[0x1F] = "Pocket Camera";
	cart_types[0xFD] = "Bandai TAMA5";
	cart_types[0xFE] = "Hudson HuC-3";
	cart_types[0xFF] = "Hudson HuC-1";

	Byte cart = buffer[0x0147];
	cout << "Cartridge Type: " << cart_types[cart] << endl;

	// delete controller;

	// Assign memory controller based on cartridge specification
	switch (cart)
	{
		case 0x01:
		case 0x02:
		case 0x03:
			controller = new MemoryController1();
			break;
		case 0x05:
		case 0x06:
			cout << "CONTROLLER NOT IMPLEMENTED" << endl;
			controller = new MemoryController2();
			break;
		case 0x0F:
		case 0x10:
		case 0x11:
		case 0x12:
		case 0x13:
		case 0x1B: // PokeYellow MCB5 quick fix
			controller = new MemoryController3();
			break;
		default:
			controller = new MemoryController0();
			break;
	}

	// Initialize controller with cartridge data
	controller->init(buffer);

	Byte rsize = buffer[0x0148];
	cout << "ROM Size: " << (32 << rsize) << "kB " << pow(2, rsize + 1) << " banks" << endl;
	int size, banks;
	switch (buffer[0x149])
	{
		case 1: size = 2; banks = 1;
		case 2: size = 8; banks = 1;
		case 3: size = 32; banks = 4;
		case 4: size = 128; banks = 16;
		default: size = 0; banks = 0;
	}
	cout << "RAM Size: " << size << "kB " << banks << " banks" << endl;
	cout << "Destination Code: " << (buffer[0x014A] == 1 ? "Non-" : "") << "Japanese" << endl;
}

void Memory::save_state(ofstream &file)
{
	write_vector(file, VRAM);
	write_vector(file, OAM);
	write_vector(file, WRAM);
	write_vector(file, ZRAM);

	if (color_mode)
	{
		write_vector(file, CGB_VRAM);
		write_vector(file, CGB_WRAM);
	}

	// save ERAM
	vector<Byte> eram = controller->get_ram();
	write_vector(file, eram);
	controller->save_state(file);
}

void Memory::load_state(ifstream &file)
{
	load_vector(file, VRAM);
	load_vector(file, OAM);
	load_vector(file, WRAM);
	load_vector(file, ZRAM);

	if (color_mode)
	{
		load_vector(file, CGB_VRAM);
		load_vector(file, CGB_WRAM);
	}

	// Load ERAM
	vector<Byte> eram(0x8000);

	load_vector(file, eram);
	controller->set_ram(eram);
	controller->load_state(file);
}

void Memory::write_vector(ofstream &file, vector<Byte> &vec)
{
	file.write((char*) &vec[0], vec.size());
}

void Memory::load_vector(ifstream &file, vector<Byte> &vec)
{
	file.read((char*) &vec[0], vec.size());
}

void Memory::do_dma_transfer()
{
	Byte_2 address = DMA.get() << 8; // multiply by 100

	for (int i = 0; i < 0xA0; i++)
	{
		write((0xFE00 + i), read(address + i));
	}
}

void Memory::do_hdma_transfer()
{
	//if (!color_mode)
	//	return;

	// High, Low Source - D000
	Byte h1 = HDMA1.get();
	Byte h2 = HDMA2.get();

	// High, Low Destination - 1600
	Byte h3 = HDMA3.get();
	Byte h4 = HDMA4.get();

	Byte h5 = HDMA5.get();

	Address source = (h1 << 8) | (h2 & 0xF0);
	Address destination = ((h3 & 0x1F) << 8) | (h4 & 0xF0);
	source &= 0xFFF0;
	destination |= 0x8000; // Destination is always in VRAM

	int length = 0x10 + ((h5 & 0x7F) * 0x10);

	for (int i = 0; i < length; i++)
	{
		write((destination + i), read(source + i));
	}

	// All done!
	HDMA5.set_bit(BIT_7);
}

Byte Memory::get_joypad_state()
{
	Byte request = P1.get();

	switch (request)
	{
		case 0x10:
			return joypad_buttons;
		case 0x20:
			return joypad_arrows;
		default:
			return 0xFF;
	}
}

Byte Memory::read(Address location)
{
	// RAM shadow adjustment
	if (location >= 0xE000 && location <= 0xFDFF)
	{
		location = location - 0x2000;
	}

	switch (location & 0xF000)
	{
	// ROM
	case 0x0000:
	case 0x1000:
	case 0x2000:
	case 0x3000:
	case 0x4000:
	case 0x5000:
	case 0x6000:
	case 0x7000:
		return controller->read(location);

	// Graphics VRAM
	case 0x8000:
	case 0x9000:
		// return correct memory bank if GBC mode
		if (color_mode)
		{
			// If VBK is non zero, return 2nd memory bank
			if (VBK.get() != 0)
				return CGB_VRAM[location & 0x1FFF];
		}

		return VRAM[location & 0x1FFF];

	// External RAM
	case 0xA000:
	case 0xB000:
		return controller->read(location);

	// Working RAM (8kB) and RAM Shadow
	case 0xC000:
	case 0xD000:
		// return correct memory bank if GBC mode
		if (color_mode && (location >= 0xD000 && location <= 0xDFFF))
		{
			// if SVBK is non zero, return calculated bank
			int bank = (SVBK.get() & 0x7);

			// Value of 00h will select Bank 1
			bank = (bank == 0) ? 1 : bank;

			if (bank > 0)
			{
				int offset = location - 0xD000;
				int lookup = (bank * 0x1000) + offset;
				return CGB_WRAM[lookup];
			}
		}
		return WRAM[location & 0x1FFF];

	// Remaining RAM: I/O, Zero page
	case 0xF000:
		switch (location & 0x0F00)
		{
			// Sprite OAM
			case 0xE00:
					return OAM[location & 0xFF];

			case 0xF00:
				if (location == 0xFF00)
					return get_joypad_state();

				// STAT register fix
				// If LCD is off, set STAT mode and LY to 0 matt
				// if (location == 0xFF41 || location == 0xFF44)
				// {
				// 	// If the LCD is off
				// 	if (LCDC.is_bit_set(BIT_7) == false)
				// 	{
				// 		if (location == 0xFF41)
				// 			return (STAT.get() & 0xFC);
				// 		else
				// 			return 0;
				// 	}
				// }

				// COLOR GAMEBOY REGISTERS
				if (location == 0xFF69)
				{
					Byte index = BGPI.get();
					return color_palettes[index];
				}

				else
					return ZRAM[location & 0xFF];
		}
	default:
		throw;
		return 0xFF;
	}
}

void Memory::write(Address location, Byte data)
{
	if (//location == 0xFF51 ||
		//location == 0xFF52 ||
		//location == 0xFF53 ||
		//location == 0xFF54 ||
		location == 0xFF55)
	{
		bool breakpoint = true;
	}
	// RAM shadow adjustment
	if (location >= 0xE000 && location <= 0xFDFF)
	{
		location = location - 0x2000;
	}

	switch (location & 0xF000)
	{
	// ROM
	case 0x0000:
	case 0x1000:
	case 0x2000:
	case 0x3000:
	case 0x4000:
	case 0x5000:
	case 0x6000:
	case 0x7000:
		controller->write(location, data);
		break;

	// Graphics VRAM
	// Cannot write to VRAM during mode 3
	case 0x8000:
	case 0x9000:
		// write to correct memory bank if GBC mode
		if (color_mode)
		{
			// If VBK is non zero, write to 2nd memory bank
			if (VBK.get() != 0)
				CGB_VRAM[location & 0x1FFF] = data;
		}
		VRAM[location & 0x1FFF] = data;
		break;

	// External RAM
	case 0xA000:
	case 0xB000:
		controller->write(location, data);
		break;

	// Working RAM (8kB) and RAM Shadow
	case 0xC000:
	case 0xD000:
		// write to correct memory bank if GBC mode
		if (color_mode && (location >= 0xD000 && location <= 0xDFFF))
		{
			// if SVBK is non zero, return calculated bank
			int bank = (SVBK.get() & 0x7);

			// Value of 00h will select Bank 1
			bank = (bank == 0) ? 1 : bank;

			if (bank > 0)
			{
				int offset = location - 0xD000;
				int lookup = (bank * 0x1000) + offset;
				CGB_WRAM[lookup] = data;
			}
		}
		WRAM[location & 0x1FFF] = data;
		break;

	// Remaining RAM: I/O, Zero page
	case 0xF000:
		switch (location & 0x0F00)
		{
		// Sprite OAM
		case 0xE00:
			OAM[location & 0xFF] = data;
			break;

		case 0xF00:
			write_zero_page(location, data);
			break;
		}
		break;
	default:
		throw;
		break;
	}
}

void Memory::write_zero_page(Address location, Byte data)
{
	switch (location)
	{
	// Joypad Register - only bits 4 & 5 can be written to
	case 0xFF00:
		ZRAM[0x00] = (data & 0x30);
		break;
	// Divider Register - Write as zero no matter content
	case 0xFF04:
		ZRAM[0x04] = 0;
		break;
	// TODO: STAT - writing to match flag resets flag but doesn't change mode
	case 0xFF41:
		ZRAM[0x41] = (data & 0xFC) | (STAT.get() & 0x03);
		break;

	// LY Register - Game cannot write to this register directly
	case 0xFF44:
		ZRAM[0x44] = 0;
		break;
	// DMA transfer request
	case 0xFF46:
		ZRAM[0x46] = data;
		do_dma_transfer();
		break;

	// COLOR GAMEBOY REGISTERS

	// CGB VRAM Bank
	case 0xFF4F:
		ZRAM[0x4F] = data;
		break;

	// HDMA Start
	case 0xFF55:
		ZRAM[0x55] = data;
		do_hdma_transfer();
		break;

	// BG Palette address
	case 0xFF68:
		if (data & 0x80) // if bit 7 is set, increment data
			data++;
		ZRAM[0x68] = data;
		break;

	// BG Palette Data write
	case 0xFF69:
	{
		Byte index = BGPI.get() & 0x3F; // extract bits 0-5 = address
		color_palettes[index] = data; // write data to color palette memory
		break;
	}

	default:
		ZRAM[location & 0xFF] = data;
		break;
	}
}
