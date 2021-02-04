#include "6502.h"
#include <fstream>
#include <vector>
#include <iostream>

int main() {
	std::vector<m6502::BYTE> code;

	for (m6502::WORD i = 0; i < 0xFFFF; i++) {
		code.push_back(0xEA);
	}
	code.push_back(0xEA);

	code[0xFFFC] = 0x00;
	code[0xFFFD] = 0x20;
	code[0x2000] = m6502::CPU::ins_ldx_im;
	code[0x2001] = 0xa0;
	code[0x2002] = m6502::CPU::ins_stx_zp;
	code[0x2003] = 0xff;
	code[0x2004] = m6502::CPU::ins_lda_zp;
	code[0x2005] = 0xff;
	code[0x2006] = m6502::CPU::ins_brk;
	code[0xFFFE] = 0x00;
	code[0xFFFF] = 0xFE;
	code[0xFE00] = m6502::CPU::ins_jmp_abs;
	code[0xFE01] = 0x00;
	code[0xFE02] = 0xFF;
	code[0xFF00] = m6502::CPU::ins_rti;

	
	uint32_t cycles = 0x000000FF;
	m6502::CPU cpu;
	m6502::MEMORY mem;
	
	mem.init(&cycles);
	mem.fill(code);
	cpu.reset(cycles, mem);
	cpu.execute(cycles, mem);
	return 0;
}