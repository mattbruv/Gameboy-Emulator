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
	int size = sizeof(this->buffer) * 8;

	while (pc < size)
	{
		pc += this->read_opcode(pc);
	}
}

int Disassembler::read_opcode(int pc)
{
	unsigned char *code = &this->buffer[pc];
	int opbytes = 1;

	cout << uppercase << hex << setfill('0') << setw(2) << pc << " | " << setfill('0') << setw(2) << int(*code) << " | ";

	switch (*code)
	{
		case 0x00: cout << "NOP"; break;

		// 65 ... 8 bit loads
		case 0x06: cout << "LD B," << hex << int(code[1]); opbytes = 2; break;
		case 0x0E: cout << "LD C," << hex << int(code[1]); opbytes = 2; break;
		case 0x16: cout << "LD D," << hex << int(code[1]); opbytes = 2; break;
		case 0x1E: cout << "LD E," << hex << int(code[1]); opbytes = 2; break;
		case 0x26: cout << "LD H," << hex << int(code[1]); opbytes = 2; break;
		case 0x2E: cout << "LD L," << hex << int(code[1]); opbytes = 2; break;

		// 66
		case 0x7F: cout << "LD A,A"; break;
		case 0x78: cout << "LD A,B"; break;
		case 0x79: cout << "LD A,C"; break;
		case 0x7A: cout << "LD A,D"; break;
		case 0x7B: cout << "LD A,E"; break;
		case 0x7C: cout << "LD A,H"; break;
		case 0x7D: cout << "LD A,L"; break;
		case 0x7E: cout << "LD A,(HL)"; break;
		case 0x40: cout << "LD B,B"; break;
		case 0x41: cout << "LD B,C"; break;
		case 0x42: cout << "LD B,D"; break;
		case 0x43: cout << "LD B,E"; break;
		case 0x44: cout << "LD B,H"; break;
		case 0x45: cout << "LD B,L"; break;
		case 0x46: cout << "LD B,(HL)"; break;
		case 0x48: cout << "LD C,B"; break;
		case 0x49: cout << "LD C,C"; break;
		case 0x4A: cout << "LD C,D"; break;
		case 0x4B: cout << "LD C,E"; break;
		case 0x4C: cout << "LD C,H"; break;
		case 0x4D: cout << "LD C,L"; break;
		case 0x4E: cout << "LD C,(HL)"; break;
		case 0x50: cout << "LD D,B"; break;
		case 0x51: cout << "LD D,C"; break;

		// 67
		case 0x52: cout << "LD D,D"; break;
		case 0x53: cout << "LD D,E"; break;
		case 0x54: cout << "LD D,H"; break;
		case 0x55: cout << "LD D,L"; break;
		case 0x56: cout << "LD D,(HL)"; break;
		case 0x58: cout << "LD E,B"; break;
		case 0x59: cout << "LD E,C"; break;
		case 0x5A: cout << "LD E,D"; break;
		case 0x5B: cout << "LD E,E"; break;
		case 0x5C: cout << "LD E,H"; break;
		case 0x5D: cout << "LD E,L"; break;
		case 0x5E: cout << "LD E,(HL)"; break;

		case 0x60: cout << "LD H,B"; break;
		case 0x61: cout << "LD H,C"; break;
		case 0x62: cout << "LD H,D"; break;
		case 0x63: cout << "LD H,E"; break;
		case 0x64: cout << "LD H,H"; break;
		case 0x65: cout << "LD H,L"; break;
		case 0x66: cout << "LD H,(HL)"; break;
		case 0x68: cout << "LD L,B"; break;
		case 0x69: cout << "LD L,C"; break;
		case 0x6A: cout << "LD L,D"; break;
		case 0x6B: cout << "LD L,E"; break;
		case 0x6C: cout << "LD L,H"; break;
		case 0x6D: cout << "LD L,L"; break;
		case 0x6E: cout << "LD L,(HL)"; break;
		case 0x70: cout << "LD (HL),B"; break;
		case 0x71: cout << "LD (HL),C"; break;
		case 0x72: cout << "LD (HL),D"; break;
		case 0x73: cout << "LD (HL),E"; break;
		case 0x74: cout << "LD (HL),H"; break;
		case 0x75: cout << "LD (HL),L"; break;
		case 0x36: cout << "LD (HL),n"; break; // Questionable

		// 68
		case 0x0A: cout << "LD A,(BC)"; break;
		case 0x1A: cout << "LD A,(DE)"; break;
		case 0xFA: cout << "LD A,(nn)"; opbytes = 3; break; // Questionable
		case 0x3E: cout << "LD A,#"; break; // Questionable

		// 69
		case 0x47: cout << "LD B,A"; break;
		case 0x4F: cout << "LD C,A"; break;
		case 0x57: cout << "LD D,A"; break;
		case 0x5F: cout << "LD E,A"; break;
		case 0x67: cout << "LD H,A"; break;
		case 0x6F: cout << "LD L,A"; break;
		case 0x02: cout << "LD (BC),A"; break;
		case 0x12: cout << "LD (DE),A"; break;
		case 0x77: cout << "LD (HL),A"; break;
		case 0xEA: cout << "LD (nn),A"; opbytes = 3; break; // is (nn) a 2 byte memory address to write A to? (LS byte first)

		// 70
		case 0xF2: cout << "LD A,($FF00+C)"; break;
		case 0xE2: cout << "LD ($FF00+C),A"; break;

		// 71
		case 0x3A: cout << "LD A,(HL) ... DEC HL"; break;

		// 72
		case 0x32: cout << "LD (HL),A ... DEC HL"; break;

		// 73
		case 0x2A: cout << "LD A,(HL) ... INC HL"; break;

		// 74
		case 0x22: cout << "LD (HL),A ... INC HL"; break;

		// 75
		case 0xE0: cout << "LD ($FF00+n),A"; opbytes = 2; break;
		case 0xF0: cout << "LD A,($FF00+n)"; opbytes = 2; break;

		// 76 ... 16 bit loads
		case 0x01: cout << "LD BC, nn"; opbytes = 3; break;
		case 0x11: cout << "LD DE, nn"; opbytes = 3; break;
		case 0x21: cout << "LD HL, nn"; opbytes = 3; break;
		case 0x31: cout << "LD SP, nn"; opbytes = 3; break;
		case 0xF9: cout << "LD SP, HL"; break;

		// 77
		case 0xF8: cout << "LD HL,SP+n"; opbytes = 2; break; // signed value

		// 78
		case 0x08: cout << "LD (nn),SP"; opbytes = 3; break;
		case 0xF5: cout << "PUSH AF"; break;
		case 0xC5: cout << "PUSH BC"; break;
		case 0xD5: cout << "PUSH DE"; break;
		case 0xE5: cout << "PUSH HL"; break;

		// 79
		case 0xF1: cout << "POP AF"; break;
		case 0xC1: cout << "POP BC"; break;
		case 0xD1: cout << "POP DE"; break;
		case 0xE1: cout << "POP HL"; break;

		// 80 ... 8 bit ALU

		// 116
		case 0xC7: cout << "RST $00"; break;
		case 0xCF: cout << "RST $08"; break;
		case 0xD7: cout << "RST $10"; break;
		case 0xDF: cout << "RST $18"; break;
		case 0xE7: cout << "RST $20"; break;
		case 0xEF: cout << "RST $28"; break;
		case 0xF7: cout << "RST $30"; break;
		case 0xFF: cout << "RST $38"; break;
	}

	cout << endl;

	//cout << uppercase << setfill('0') << hex << setw(2) << (int) code << " ";

	return opbytes;
}
