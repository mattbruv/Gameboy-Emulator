#include "emulator.h"

Emulator::Emulator()
{
	cpu.init(&memory);
	display.init(&memory);
}

// Start emulation of CPU
void Emulator::run()
{
	sf::Time time;

	while(display.window.isOpen())
	{
		// CPU cycles to emulate per frame draw
		float cycles_per_frame = cpu.CLOCK_SPEED / framerate;
		float time_between_frames = 1000 / framerate;
		// Current cycle in frame
		int current_cycle = 0;

		handle_events();

		while (current_cycle < cycles_per_frame)
		{
			Opcode code = memory.read(cpu.reg_PC);

			cpu.parse_opcode(code);
			current_cycle += cpu.num_cycles;

			update_timers(cpu.num_cycles);
			update_scanline(cpu.num_cycles);
			do_interrupts();
			
			cpu.num_cycles = 0;
		}

		//display.render();
		//cout << "frame " << current_cycle << endl;
		current_cycle = 0;

		int frame_time = time.asMilliseconds();

		float sleep_time = time_between_frames - frame_time;
		if (frame_time < time_between_frames)
			sf::sleep(sf::milliseconds(sleep_time));
		time = time.Zero;
		//cout << display.scanlines_rendered << endl;
		display.scanlines_rendered = 0;
	}
}

// Hanlde window events and IO
void Emulator::handle_events()
{
	sf::Event event;

	while (display.window.pollEvent(event))
	{
		switch (event.type)
		{
			case sf::Event::Closed:
				display.window.close();
				break;
			case sf::Event::KeyPressed:
				key_pressed(event.key.code);
				break;
			case sf::Event::KeyReleased:
				key_released(event.key.code);
				break;
		}
	}
}

void Emulator::key_pressed(Key key)
{
	if (key == Key::P)
	{
		display.debug_enabled = !display.debug_enabled;
		cout << "debug enabled: " << display.debug_enabled << endl;
	}

	// super ghetto, will make much nicer later
	if (key == Key::F1)
	{
		if (sf::Keyboard::isKeyPressed(Key::LShift))
		{
			save_state(1);
		}
		else
			load_state(1);
	}
	
	if (key == Key::Space)
	{
		cpu.CLOCK_SPEED *= 100;
	}

	int key_id = get_key_id(key);

	if (key_id < 0)
		return;

	bool directional = false;

	if (key == Key::Up || key == Key::Down || key == Key::Left || key == Key::Right)
	{
		directional = true;
	}

	Byte joypad = (directional) ? memory.joypad_arrows : memory.joypad_buttons;
	bool unpressed = is_bit_set(joypad, key_id);
	
	if (!unpressed)
		return;

	if (directional)
		memory.joypad_arrows = clear_bit(joypad, key_id);
	else
		memory.joypad_buttons = clear_bit(joypad, key_id);

	request_interrupt(INTERRUPT_JOYPAD);
}

void Emulator::key_released(Key key)
{
	if (key == Key::Space)
	{
		cpu.CLOCK_SPEED /= 100;
	}

	int key_id = get_key_id(key);

	if (key_id < 0)
		return;

	bool directional = false;

	if (key == Key::Up || key == Key::Down || key == Key::Left || key == Key::Right)
	{
		directional = true;
	}

	Byte joypad = (directional) ? memory.joypad_arrows : memory.joypad_buttons;
	bool unpressed = is_bit_set(joypad, key_id);

	if (unpressed)
		return;

	if (directional)
		memory.joypad_arrows = set_bit(joypad, key_id);
	else
		memory.joypad_buttons = set_bit(joypad, key_id);
}

int Emulator::get_key_id(Key key)
{
	switch (key)
	{
		case Key::A:
		case Key::Right:
			return BIT_0;
		case Key::S: // B
		case Key::Left:
			return BIT_1;
		case Key::X: // select
		case Key::Up:
			return BIT_2;
		case Key::Z:
		case Key::Down:
			return BIT_3;
		default:
			return -1;
	}
}

void Emulator::update_divider(int cycles)
{
	divider_counter += cycles;

	if (divider_counter >= 256) // 16384 Hz
	{
		divider_counter = 0;
		memory.DIV.set(memory.DIV.get() + 1);
	}
}

// Opcode cycle number may need adjusted, used Nintendo values
void Emulator::update_timers(int cycles)
{
	update_divider(cycles);

	// This can be optimized if needed
	Byte new_freq = get_timer_frequency();

	if (timer_frequency != new_freq)
	{
		set_timer_frequency();
		timer_frequency = new_freq;
	}

	if (timer_enabled())
	{
		timer_counter -= cycles;

		// enough cpu clock cycles have happened to update timer
		if (timer_counter <= 0)
		{
			Byte timer_value = memory.TIMA.get();
			set_timer_frequency();

			// Timer will overflow, generate interrupt
			if (timer_value == 255)
			{
				memory.TIMA.set(memory.TMA.get());
				request_interrupt(INTERRUPT_TIMER);
			}
			else
			{
				memory.TIMA.set(timer_value + 1);
			}
		}
	}
}

bool Emulator::timer_enabled()
{
	return memory.TAC.is_bit_set(BIT_2);
}

Byte Emulator::get_timer_frequency()
{
	return (memory.TAC.get() & 0x3);
}

void Emulator::set_timer_frequency()
{
	Byte frequency = get_timer_frequency();
	timer_frequency = frequency;

	switch (frequency)
	{
		// timer_counter calculated by (Clock Speed / selected frequency)
	case 0: timer_counter = 1024; break; // 4096 Hz
	case 1: timer_counter = 16; break; // 262144 Hz
	case 2: timer_counter = 64; break; // 65536 Hz
	case 3: timer_counter = 256; break; // 16384 Hz
	}
}

void Emulator::request_interrupt(Byte id)
{
	memory.IF.set_bit(id);
}

void Emulator::do_interrupts()
{
	// If there are any interrupts set
	if (memory.IF.get() > 0)
	{
		// Resume CPU state if halted and interrupts are pending
		if (memory.IE.get() > 0)
		{
			if (cpu.halted)
			{
				cpu.halted = false;
				cpu.reg_PC += 1;
			}
		}
		// Loop through each bit and call interrupt for lowest . highest priority bits set
		for (int i = 0; i < 5; i++)
		{
			if (memory.IF.is_bit_set(i))
			{
				if (memory.IE.is_bit_set(i))
				{
					// IME only disables the servicing of interrupts,
					// not all interrupt functionality 
					if (cpu.interrupt_master_enable)
					{
						service_interrupt(i);
					}
				}
			}
		}
	}
}

void Emulator::service_interrupt(Byte id)
{
	cpu.interrupt_master_enable = false;
	memory.IF.clear_bit(id);

	// Push current execution address to stack
	memory.write(--cpu.reg_SP, high_byte(cpu.reg_PC));
	memory.write(--cpu.reg_SP, low_byte(cpu.reg_PC));

	switch (id)
	{
		case INTERRUPT_VBLANK: cpu.reg_PC = 0x40; break;
		case INTERRUPT_LCDC:   cpu.reg_PC = 0x48; break;
		case INTERRUPT_TIMER:  cpu.reg_PC = 0x50; break;
		case INTERRUPT_SERIAL: cpu.reg_PC = 0x58; break;
		case INTERRUPT_JOYPAD: cpu.reg_PC = 0x60; break;
	}
}

void Emulator::set_lcd_status()
{
	Byte status = memory.STAT.get();

	Byte current_line = memory.LY.get();
	// extract current LCD mode
	Byte current_mode = status & 0x03;

	Byte mode = 0;
	bool do_interrupt = false;

	// in VBLANK, set mode to 1
	if (current_line >= 144)
	{
		mode = 1; // In vertical blanking period
		// 1 binary
		status = set_bit(status, BIT_0);
		status = clear_bit(status, BIT_1);
		do_interrupt = is_bit_set(status, BIT_4);

	}
	else
	{
		int mode2_threshold = 456 - 80;
		int mode3_threshold = mode2_threshold - 172;

		if (scanline_counter >= mode2_threshold)
		{
			mode = 2; // Searching OAM RAM
			// 2 binary
			status = set_bit(status, BIT_1);
			status = clear_bit(status, BIT_0);
			do_interrupt = is_bit_set(status, BIT_5);
		}
		else if (scanline_counter >= mode3_threshold)
		{
			mode = 3; // Transferring data to LCD driver
			// 3 binary
			status = set_bit(status, BIT_1);
			status = set_bit(status, BIT_0);
		}
		else
		{
			mode = 0; // CPU has access to all display RAM
			// 0 binary
			status = clear_bit(status, BIT_1);
			status = clear_bit(status, BIT_0);
			do_interrupt = is_bit_set(status, BIT_3);
		}
	}

	// Entered new mode, request interrupt
	if (do_interrupt && (mode != current_mode))
		request_interrupt(INTERRUPT_LCDC);

	// check coincidence flag, set bit 2 if it matches
	if (memory.LY.get() == memory.LYC.get())
	{
		status = set_bit(status, BIT_2);

		if (is_bit_set(status, BIT_6))
			request_interrupt(INTERRUPT_LCDC);
	}
	// clear bit 2 if not
	else
		status = clear_bit(status, BIT_2);

	memory.STAT.set(status);
	memory.video_mode = mode;
}

void Emulator::update_scanline(int cycles)
{
	set_lcd_status();

	scanline_counter -= cycles;

	if (memory.LY.get() > 153)
		memory.LY.clear();

	// Enough time has passed to draw the next scanline
	if (scanline_counter <= 0)
	{
		Byte current_scanline = memory.LY.get();

		// draw current scanline to screen
		if (current_scanline < 144 && display.scanlines_rendered <= 144)
			display.update_scanline(current_scanline);

		// increment scanline and reset counter
		memory.LY.set(++current_scanline);
		scanline_counter = 456;

		// Entered VBLANK period
		if (current_scanline == 144)
		{
			request_interrupt(INTERRUPT_VBLANK);
			if (display.scanlines_rendered <= 144)
				display.render();
		}
		// Reset counter if past maximum
		else if (current_scanline > 153)
			memory.LY.clear();
	}
}

void Emulator::save_state(int id)
{
	// write following data to a file
	// CPU registers
	// All memory
	// controller ERAM

	ofstream file;

	file.open("testing.bin", ios::binary);

	if (!file.bad())
	{
		cpu.save_state(file);
		memory.save_state(file);
		file.close();

		cout << "wrote save state " << id << endl;
	}
}

void Emulator::load_state(int id)
{
	ifstream file("testing.bin", ios::binary);

	if (file.is_open())
	{
		cpu.load_state(file);
		memory.load_state(file);
		file.close();
		cout << "loaded state " << id << endl;
	}
}