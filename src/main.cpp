#include "6502.h"
#include <fstream>
#include <vector>
#include <iostream>

/*
constexpr char *instructions[0xFF] = {
	"brk", "ora indx", "0x02", "0x03", "0x04", "ora zp", "asl zp", "0x07", "php", "ora im", "asl acc", "0x0B", "0x0C", "ora abs", "asl abs", "0x0F"
	"bpl", "ora indy", "0x12", "0x13", "0x14", "ora zpx", "asl zpx", "0x17", "clc", "ora absy", "0x1A", "0x1B", "0x1C", "ora absx", "asl absx", "0x1F",
	"jsr abs", "and indx", "0x22", "0x23", "bit zp", "and zp", "rol zp", "0x27", "plp", "and im", "rol acc", "0x2B", "bit abs", "and abs", "rol abs", "0x2F",
	"bmi rel", "and indy", "0x32", "0x33", "0x34", "and zpgx", "rol zpx", "0x37", "sec", "and absy", "0x3A", "0x3B", "0x3C", "and absx", "rol abs", "0x3F",
	"rti", "eor indx", "0x42", "0x43", "0x44", "eor zp", "lsr zp", "0x47", "pha", "eor im", "lsr acc", "0x4B", "jmp abs", "eor abs", "lsr abs", "0x4F",
	"bvc", "eor indy", "0x52", "0x53", "0x54", "eor zpx", "lsr zpx", "0x57", "cli", "eor absx", "0x5A", "0x5B", "0x5C", "eor absx", "lsr absx",
	"rts", "adc indx", "0x62", "0x63", "0x64", "adc zp", "ror zp", "0x67", "pla", "adc im", "ror acc", "0x6B", "jmp ind", "adc abs", "ror abs", "0x6F",
	"bvs", "adc indy", "0x72", "0x73", "0x74", "adc zpx", "ror zpx", "0x77", "sei", "adc absy", "0x7A", "0x7B", "0x7C", "adc absx", "ror absx",
	"0x80", "sta indx", "0x82", "0x83", "sty zp", "sta zp", "stx zp", "0x87", "dey", "0x89", "txa", "0x8B", "sty abs", "sta abs", "stx abs", "0x8F",
	"bcc", "sta indy", "0x92", "0x93", "sty zpx", "sta zpx", "stx zpy", "0x97", "tya", "sta absy", "txs", "0x9B", "0x9C", "sta absx", "0x9E", "0x9F",
	"ldy im", "lda indx", "ldx im", "0xA3", "sty zpx", "sta zpx", "stx zpy", "0xA7", "tay", "lda im", "tax", "0xAB", "ldy abs", "lda abs", "ldx abs", "0xAF",
	"bcs", "lda indy", "0xB2", "0xB3", "ldy zpx", "lda zpx", "ldx zpy", "0xB7", "clv", "lda absx", "tsx", "0xBB", "ldy absx", "lda absx", "ldx absy", "0xBF",
	"cpy im", "cmp indx", "0xC2", "0xC3", "cpy zp", "cmp zp", "dec zp", "0xC7", "iny", "cmp im", "dex", "0xCB", "cpy abs", "cmp abs", "dec abs", "0xCF",
	"bne", "cmp indy", "0xD2", "0xD3", "0xD4", "cmp zpx", "dec zpx", "0xD7", "cld", "cmp absy", "0xDA", "0xDB", "0xDC", "cmp absx", "dec absx", "0xDF",
	"cpx im", "sbc indx", "0xE2", "0xE3", "cpx zp", "sbc zp", "inc zp", "0xE7", "inx", "sbc im", "nop", "0xEB", "cpx abs", "sbc abs", "inc abs", "0xEF",
	"beq", "sbc indy", "0xF2", "0xF3", "0xF4", "sbc zpx", "inc zpx", "0xF7", "sed", "sbc absy", "0xFA", "0xFB", "0xFC", "sbc absx", "inc absx", "0xFF"
};
*/

int main() {
	std::vector<m6502::BYTE> code;

	for (m6502::WORD i = 0; i < 0xFFFF; i++) {
		code.push_back(0xEA);
	}
	code.push_back(0xEA);

	code[0xFFFC] = 0x08;
	code[0xFFFD] = 0x20;
	
	// compute first 16 values of the fibonacci sequence until the end of time
	// sets x to zero and jumps to subroutine
	code[0x2008] = m6502::CPU::ins_ldx_im;
	code[0x2009] = 0x00;
	code[0x200A] = m6502::CPU::ins_jsr_abs;
	code[0x200B] = 0x06;
	code[0x200C] = 0x30;
	// if x == 0xFF, continue. Otherwise, branch to jump to subroutine
	code[0x200D] = m6502::CPU::ins_cpx_im;
	code[0x200E] = 0xFF;
	code[0x200F] = m6502::CPU::ins_bne;
	code[0x2010] = 0xF9;
	// sets x to zero and branch to jump to subroutine
	code[0x2011] = m6502::CPU::ins_ldx_im;
	code[0x2012] = 0x00;
	code[0x2013] = m6502::CPU::ins_sec;
	code[0x2014] = m6502::CPU::ins_bcc;
	code[0x2015] = 0xF4;
	
	// subroutine branch if x == 0 or x == 1 : loads 1 into a and stores it at $0,X
	code[0x3000] = m6502::CPU::ins_lda_im;
	code[0x3001] = 0x01;
	code[0x3002] = m6502::CPU::ins_sta_zpx;
	code[0x3003] = 0x00;
	// increments x and returns from subroutine
	code[0x3004] = m6502::CPU::ins_inx;
	code[0x3005] = m6502::CPU::ins_rts;

	// checks if x == 0 or x == 1. If so, branch to $3000
	code[0x3006] = m6502::CPU::ins_cpx_im;
	code[0x3007] = 0x00;
	code[0x3008] = m6502::CPU::ins_beq;
	code[0x3009] = 0xF6;
	code[0x300A] = m6502::CPU::ins_cpx_im;
	code[0x300B] = 0x01;
	code[0x300C] = m6502::CPU::ins_beq;
	code[0x300D] = 0xF2;
	// loads $FF,X into a and adds $FE,X
	code[0x300E] = m6502::CPU::ins_lda_zpx;
	code[0x300F] = 0xFF;
	code[0x3010] = m6502::CPU::ins_clc;
	code[0x3011] = m6502::CPU::ins_adc_zpx;
	code[0x3012] = 0xFE;
	// stores a into $0,X, increments x and returns from subroutine
	code[0x3013] = m6502::CPU::ins_sta_zpx;
	code[0x3014] = 0x00;
	code[0x3015] = m6502::CPU::ins_inx;
	code[0x3016] = m6502::CPU::ins_rts;
	
	uint32_t cycles = 0x00000FFC;
	m6502::CPU cpu;
	m6502::MEMORY mem;
	
	mem.init(&cycles);
	mem.fill(code);
	cpu.reset(cycles, mem);
	cpu.execute(cycles, mem);
	for (m6502::BYTE i = 0; i < 0xFF; i++) {
		std::cout << std::dec << (int)mem[i] << std::endl;
	}
	std::cout << std::dec << (int)mem[0xFF] << std::endl;
	return 0;
}