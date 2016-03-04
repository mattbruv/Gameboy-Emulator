#include "disassembler.h"

void Disassembler::load_rom(string file_dir)
{
	ifstream rom(file_dir, ios::binary);
	this->buffer = vector<unsigned char>((istreambuf_iterator<char>(rom)), istreambuf_iterator<char>());

	cout << "Loaded ROM: " << file_dir << endl;
}

void Disassembler::disassemble()
{
	int pc = 0;
	int size = sizeof(this->buffer);

	while (pc < size * 8)
	{
		if (pc != 0 && pc % 7 == 0)
			cout << endl;

		this->read_opcode(pc++);
	}
}

int Disassembler::read_opcode(int pc)
{
	unsigned char code = this->buffer[pc];

	cout << hex << (int) code << " ";

	return 0;
}
