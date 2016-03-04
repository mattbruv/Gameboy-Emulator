#pragma once

#include <string>
#include <iostream>

#include <fstream>
#include <vector>

using namespace std;

class Disassembler
{
	public:
		void load_rom(string file_dir);
		void disassemble();
		int read_opcode(int pc);

	private:
		vector<unsigned char> buffer;
};