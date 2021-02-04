#include "6502.h"
#include <fstream>
#include <vector>
#include <iostream>

int main() {
	std::vector<m6502::BYTE> code;

	for (m6502::WORD i = 0; i < 0xFFFF; i++) {
		code.push_back(0xEA);
	}
	
	uint32_t cycles = 0x000000FF;
	m6502::CPU cpu;
	m6502::MEMORY mem;
	
	mem.init(&cycles);
	mem.fill(code);
	cpu.reset(cycles, mem);
	cpu.execute(cycles, mem);
	return 0;
}