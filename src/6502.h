#ifndef _6502_H
#define _6502_H

#include <iostream>
#include <vector>

namespace m6502 {

	typedef uint8_t BYTE;	// uint8_t (1 byte)
	typedef uint16_t WORD;	// uint16_t (2 bytes)

	// computer memory struct
	struct MEMORY {
		public:
			// returns reference to data[address] (1 cycle)
			BYTE &operator[](WORD address) {
				(*cycles)--;
				return this->data[address];
			}
			
			// fills memory with 0. Does not affect cycle count (done before CPU starts)
			void init(uint32_t *nCycles) {
				for (WORD i = 0; i < MAX_MEM; i++) {
					data[i] = 0;
				}
				cycles = nCycles;
			}

			// fills memory with given byte array. Does not affect cycle count (done before CPU starts)
			void fill(std::vector<BYTE> nData) {
				for (WORD i = 0; i < nData.size(); i++) {
					data[i] = nData[i];
					if (i == MAX_MEM) {
						break;
					}
				}
			}
		private:
			static constexpr WORD MAX_MEM = 0xFFFF;
			BYTE data[MAX_MEM];	// memory data (64 KiB)
			uint32_t *cycles;	// pointer to cycle count
	}; // struct MEMORY

	// computer central processing unit struct
	struct CPU {
		public:
			static constexpr bool READ = true;
			static constexpr bool WRITE = false;

			static constexpr BYTE ins_lda_im = 0xA9;	// immediate LOAD A instruction (2 cycles, 2 bytes. Affects zero and negative flags)
			static constexpr BYTE ins_lda_zp = 0xA5;	// zero-page LOAD A instruction (3 cycles, 2 bytes. Affects zero and negative flags)
			static constexpr BYTE ins_lda_zpx = 0xB5;	// zero-page X LOAD A instruction (4 cycles, 2 bytes. Affects zero and negative flags)
			static constexpr BYTE ins_lda_abs = 0xAD; 	// absolute LOAD A instruction (4 cycles, 3 bytes. Affects zero and negative flags)
			static constexpr BYTE ins_lda_absx = 0xBD; 	// absolute X LOAD A instruction (4-5 cycles, 3 bytes. Affects zero and negative flags)
			static constexpr BYTE ins_lda_absy = 0xB9; 	// absolute Y LOAD A instruction (4-5 cycles, 3 bytes. Affects zero and negative flags)
			static constexpr BYTE ins_lda_indx = 0xA1; 	// indirect X LOAD A instruction (6 cycles, 2 bytes. Affects zero and negative flags)
			static constexpr BYTE ins_lda_indy = 0xB1;	// indirect Y LOAD A instruction (5-6 cycles, 2 bytes. Affects zero and negative flags)

			static constexpr BYTE ins_ldx_im = 0xA2;	// immediate LOAD X instruction (2 cycles, 2 bytes. Affects zero and negative flags)
			static constexpr BYTE ins_ldx_zp = 0xA6;	// zero-page LOAD X instruction (3 cycles, 2 bytes. Affects zero and negative flags)
			static constexpr BYTE ins_ldx_zpy = 0xB6;	// zero-page Y LOAD X instruction (4 cycles, 2 bytes. Affects zero and negative flags)
			static constexpr BYTE ins_ldx_abs = 0xAE;	// absolute LOAD X instruction (4 cycles, 3 bytes. Affects zero and negative flags)
			static constexpr BYTE ins_ldx_absy = 0xBE;	// absolute Y LOAD X instruction (4-5 cycles, 3 bytes. Affects zero and negative flags)

			static constexpr BYTE ins_ldy_im = 0xA0;	// immediate LOAD Y instruction (2 cycles, 2 bytes. Affects zero and negative flags)
			static constexpr BYTE ins_ldy_zp = 0xA4;	// zero-page LOAD Y instruction (3 cycles, 2 bytes. Affects zero and negative flags)
			static constexpr BYTE ins_ldy_zpx = 0xB4;	// zero-page X LOAD Y instruction (4 cycles, 2 bytes. Affects zero and negative flags)
			static constexpr BYTE ins_ldy_abs = 0xAC;	// absolute LOAD Y instruction (4 cycles, 3 bytes. Affects zero and negative flags)
			static constexpr BYTE ins_ldy_absx = 0xBC;	// absolute X LOAD Y instruction (4-5 cycles, 3 bytes. Affects zero and negative flags)

			static constexpr BYTE ins_sta_zp = 0x85;	// zero-page STORE A instruction (3 cycles, 2 bytes. Does not affect any flag)
			static constexpr BYTE ins_sta_zpx = 0x95;	// zero-page X STORE A instruction (4 cycles, 2 bytes. Does not affect any flag)
			static constexpr BYTE ins_sta_abs = 0x8D;	// absolute STORE A instruction (4 cycles, 3 bytes. Does not affect any flag)
			static constexpr BYTE ins_sta_absx = 0x9D;	// absolute X STORE A instruction (5 cycles, 3 bytes. Does not affect any flag)
			static constexpr BYTE ins_sta_absy = 0x99;	// absolute Y STORE A instruction (5 cycles, 3 bytes. Does not affect any flag)
			static constexpr BYTE ins_sta_indx = 0x81;	// indirect X STORE A instruction (6 cycles, 2 bytes. Does not affect any flag)
			static constexpr BYTE ins_sta_indy = 0x91;	// indirect Y STORE A instruction (6 cycles, 2 bytes. Does not affect any flag)

			static constexpr BYTE ins_stx_zp = 0x86;	// zero-page STORE X instruction (3 cycles, 2 bytes. Does not affect any flag)
			static constexpr BYTE ins_stx_zpy = 0x96;	// zero-page Y STORE X instruction (4 cycles, 2 bytes. Does not affect any flag)
			static constexpr BYTE ins_stx_abs = 0x8E;	// absolute STORE X instruction (4 cycles, 3 bytes. Does not affect any flag)

			static constexpr BYTE ins_sty_zp = 0x84;	// zero-page STORE Y instruction (3 cycles, 2 bytes. Does not affect any flag)
			static constexpr BYTE ins_sty_zpx = 0x94;	// zero-page X STORE Y instruction (4 cycles, 2 bytes. Does not affect any flag)
			static constexpr BYTE ins_sty_abs = 0x8C;	// absolute STORE Y instruction (4 cycles, 3 bytes. Does not affect any flag)

			static constexpr BYTE ins_tax = 0xAA;		// TRANSFER A TO X instruction (1 byte, 2 cycles. Affects zero and negative flags)
			static constexpr BYTE ins_tay = 0xA8;		// TRANSFER A TO Y instruction (1 byte, 2 cycles. Affects zero and negative flags)
			static constexpr BYTE ins_txa = 0x8A;		// TRANSFER X TO A instruction (1 byte, 2 cycles. Affects zero and negative flags)
			static constexpr BYTE ins_tya = 0x98;		// TRANSFER Y TO A instruction (1 byte, 2 cycles. Affects zero and negative flags)
			
			static constexpr BYTE ins_tsx = 0xBA;		// TRANSFER SP TO X instruction (1 byte, 2 cycles. Affects zero and negative flags)
			static constexpr BYTE ins_txs = 0x9A;		// TRANSFER X TO SP instruction (1 byte, 2 cycles. Does not affect any flag)
			static constexpr BYTE ins_pha = 0x48;		// PUSH A instruction (1 byte, 2 cycles. Does not affect any flag)
			static constexpr BYTE ins_php = 0x08;		// PUSH PROCESSOR STATUS instruction (1 byte, 3 cycles. Does not affect any flag)
			static constexpr BYTE ins_pla = 0x68;		// PULL ACCUMULATOR instruction (1 byte, 4 cycles. Affects zero and negative flags)
			static constexpr BYTE ins_plp = 0x28;		// PULL PROCESSOR STATUS instruction (1 byte, 4 cycles. Affects all flags (pulled from stack))

			static constexpr BYTE ins_and_im = 0x29;	// immediate AND instruction (2 bytes, 2 cycles. Affects zero and negative flags)
			static constexpr BYTE ins_and_zp = 0x25;	// zero-page AND instruction (2 bytes, 3 cycles. Affects zero and negative flags)
			static constexpr BYTE ins_and_zpx = 0x35;	// zero-page X AND instruction (2 bytes, 4 cycles. Affects zero and negative flags)
			static constexpr BYTE ins_and_abs = 0x2D;	// absolute AND instruction (3 bytes, 4 cycles. Affects zero and negative flags)
			static constexpr BYTE ins_and_absx = 0x3D;	// absolute X AND instruction (3 bytes, 4-5 cycles. Affects zero and negative flags)
			static constexpr BYTE ins_and_absy = 0x39;	// absolute Y AND instruction (3 bytes, 4-5 cycles. Affects zero and negative flags)
			static constexpr BYTE ins_and_indx = 0x21;	// indirect X AND instruction (2 bytes, 6 cycles. Affects zero and negative flags)
			static constexpr BYTE ins_and_indy = 0x31;	// indirect Y AND instruction (2 bytes, 5-6 cycles. Affects zero and negative flags)

			static constexpr BYTE ins_eor_im = 0x49;	// immediate EXCLUSIVE OR instruction (2 bytes, 2 cycles. Affects zero and negative flags)
			static constexpr BYTE ins_eor_zp = 0x45;	// zero-page EXCLUSIVE OR instruction (2 bytes, 3 cycles. Affects zero and negative flags)
			static constexpr BYTE ins_eor_zpx = 0x55;	// zero-page X EXCLUSIVE OR instruction (2 bytes, 4 cycles. Affects zero and negative flags)
			static constexpr BYTE ins_eor_abs = 0x4D;	// absolute EXCLUSIVE OR instruction (3 bytes, 4 cycles. Affects zero and negative flags)
			static constexpr BYTE ins_eor_absx = 0x5D;	// absolute X EXCLUSIVE OR instruction (3 bytes, 4-5 cycles. Affects zero and negative flags)
			static constexpr BYTE ins_eor_absy = 0x59;	// absolute Y EXCLUSIVE OR instruction (3 bytes, 4-5 cycles. Affects zero and negative flags)
			static constexpr BYTE ins_eor_indx = 0x41;	// indirect X EXCLUSIVE OR instruction (2 bytes, 6 cycles. Affects zero and negative flags)
			static constexpr BYTE ins_eor_indy = 0x51;	// indirect Y EXCLUSIVE OR instruction (2 bytes, 5-6 cycles. Affects zero and negative flags)

			static constexpr BYTE ins_ora_im = 0x09;	// immediate INCLUSIVE OR instruction (2 bytes, 2 cycles. Affects zero and negative flags)
			static constexpr BYTE ins_ora_zp = 0x05;	// zero-page INCLUSIVE OR instruction (2 bytes, 3 cycles. Affects zero and negative flags)
			static constexpr BYTE ins_ora_zpx = 0x15;	// zero-page X INCLUSIVE OR instruction (2 bytes, 4 cycles. Affects zero and negative flags)
			static constexpr BYTE ins_ora_abs = 0x0D;	// absolute INCLUSIVE OR instruction (3 bytes, 4 cycles. Affects zero and negative flags)
			static constexpr BYTE ins_ora_absx = 0x1D;	// absolute X INCLUSIVE OR instruction (3 bytes, 4-5 cycles. Affects zero and negative flags)
			static constexpr BYTE ins_ora_absy = 0x19;	// absolute Y INCLUSIVE OR instruction (3 bytes, 4-5 cycles. Affects zero and negative flags)
			static constexpr BYTE ins_ora_indx = 0x01;	// indirect X INCLUSIVE OR instruction (2 bytes, 6 cycles. Affects zero and negative flags)
			static constexpr BYTE ins_ora_indy = 0x11;	// indirect Y INCLUSIVE OR instruction (2 bytes, 5-6 cycles. Affects zero and negative flags)

			static constexpr BYTE ins_bit_zp = 0x24;	// zero-page BIT TEST instruction (2 bytes, 3 cycles. Affects zero, overflow and negative flags)
			static constexpr BYTE ins_bit_abs = 0x2C;	// absolute BIT TEST instruction (3 bytes, 4 cycles. Affects zero, overflow and negative flags)

			static constexpr BYTE ins_adc_im = 0x69;	// immediate ADD CARRY instruction (2 bytes, 2 cycles. Affects carry, zero, overflow and negative flags)
			static constexpr BYTE ins_adc_zp = 0x65;	// zero-page ADD CARRY instruction (2 bytes, 3 cycles. Affects carry, zero, overflow and negative flags)
			static constexpr BYTE ins_adc_zpx = 0x75;	// zero-page X ADD CARRY instruction (2 bytes, 4 cycles. Affects carry, zero, overflow and negative flags)
			static constexpr BYTE ins_adc_abs = 0x6D;	// absolute ADD CARRY instruction (3 bytes, 4 cycles. Affects carry, zero, overflow and negative flags)
			static constexpr BYTE ins_adc_absx = 0x7D;	// absolute X ADD CARRY instruction (3 bytes, 4-5 cycles. Affects carry, zero, overflow and negative flags)
			static constexpr BYTE ins_adc_aby = 0x79;	// absolute Y ADD CARRY instruction (3 bytes, 4-5 cycles. Affects carry, zero, overflow and negative flags)
			static constexpr BYTE ins_adc_indx = 0x61;	// indirect X ADD CARRY instruction (2 bytes, 6 cycles. Affects carry, zero, overflow and negative flags)
			static constexpr BYTE ins_adc_indy = 0x71;	// indirect Y ADD CARRY instruction (2 bytes, 5-6 cycles. Affects carry, zero, overflow and negative flags)

			static constexpr BYTE ins_sbc_im = 0xE9;	// immediate SUBTRACT CARRY instruction (2 bytes, 2 cycles. Affects carry, zero, overflow and negative flags)
			static constexpr BYTE ins_sbc_zp = 0xE5;	// zero-page SUBTRACT CARRY instruction (2 bytes, 3 cycles. Affects carry, zero, overflow and negative flags)
			static constexpr BYTE ins_sbc_zpx = 0xF5;	// zero-page X SUBTRACT CARRY instruction (2 bytes, 4 cycles. Affects carry, zero, overflow and negative flags)
			static constexpr BYTE ins_sbc_abs = 0xED;	// absolute SUBTRACT CARRY instruction (3 bytes, 4 cycles. Affects carry, zero, overflow and negative flags)
			static constexpr BYTE ins_sbc_absx = 0xFD;	// absolute X SUBTRACT CARRY instruction (3 bytes, 4-5 cycles. Affects carry, zero, overflow and negative flags)
			static constexpr BYTE ins_sbc_aby = 0xF9;	// absolute Y SUBTRACT CARRY instruction (3 bytes, 4-5 cycles. Affects carry, zero, overflow and negative flags)
			static constexpr BYTE ins_sbc_indx = 0xE1;	// indirect X SUBTRACT CARRY instruction (2 bytes, 6 cycles. Affects carry, zero, overflow and negative flags)
			static constexpr BYTE ins_sbc_indy = 0xF1;	// indirect Y SUBTRACT CARRY instruction (2 bytes, 5-6 cycles. Affects carry, zero, overflow and negative flags)

			static constexpr BYTE ins_cmp_im = 0xC9;	// immediate COMPARE A instruction (2 bytes, 2 cycles. Affects carry, zero and negative flags)
			static constexpr BYTE ins_cmp_zp = 0xC5;	// zero-page COMPARE A instruction (2 bytes, 3 cycles. Affects carry, zero and negative flags)
			static constexpr BYTE ins_cmp_zpx = 0xD5;	// zero-page X COMPARE A instruction (2 bytes, 4 cycles. Affects carry, zero and negative flags)
			static constexpr BYTE ins_cmp_abs = 0xCD;	// absolute COMPARE A instruction (3 bytes, 4 cycles. Affects carry, zero and negative flags)
			static constexpr BYTE ins_cmp_absx = 0xDD;	// absolute X COMPARE A instruction (3 bytes, 4-5 cycles. Affects carry, zero and negative flags)
			static constexpr BYTE ins_cmp_aby = 0xD9;	// absolute Y COMPARE A instruction (3 bytes, 4-5 cycles. Affects carry, zero and negative flags)
			static constexpr BYTE ins_cmp_indx = 0xC1;	// indirect X COMPARE A instruction (2 bytes, 6 cycles. Affects carry, zero and negative flags)
			static constexpr BYTE ins_cmp_indy = 0xD1;	// indirect Y COMPARE A instruction (2 bytes, 5-6 cycles. Affects carry, zero and negative flags)

			static constexpr BYTE ins_cpx_im = 0xE0;	// immediate COMPARE X instruction (2 bytes, 2 cycles. Affects carry, zero and negative flags)
			static constexpr BYTE ins_cpx_zp = 0xE4;	// zero-page COMPARE X instruction (2 bytes, 3 cycles. Affects carry, zero and negative flags)
			static constexpr BYTE ins_cpx_abs = 0xEC;	// absolute COMPARE X instruction (3 bytes, 4 cycles. Affects carry, zero and negative flags)

			static constexpr BYTE ins_cpy_im = 0xC0;	// immediate COMPARE Y instruction (2 bytes, 2 cycles. Affects carry, zero and negative flags)
			static constexpr BYTE ins_cpy_zp = 0xC4;	// zero-page COMPARE Y instruction (2 bytes, 3 cycles. Affects carry, zero and negative flags)
			static constexpr BYTE ins_cpy_abs = 0xCC;	// absolute COMPARE Y instruction (3 bytes, 4 cycles. Affects carry, zero and negative flags)


			// sends a reset signal to reset computer state (7 cycles)
			void reset(uint32_t &cycles, MEMORY &mem) {
				reg_programCounter = 0x0000;
				reg_stackPointer = 0x00;
				cycles--;
				cycles--;
				fl_carry = fl_zero = fl_interr = fl_dec = fl_oflow = fl_neg = false;
				reg_acc = reg_x = reg_y = 0;
				// three fake stack accesses to set the stack pointer to 0xFDs
				mem[reg_stackPointer | 0x0100];
				reg_stackPointer--;
				mem[reg_stackPointer | 0x0100];
				reg_stackPointer--;
				mem[reg_stackPointer | 0x0100];
				reg_stackPointer--;
				// stores contents at 0xFFFC and 0xFFFD (little-endian) in program counter (contains location of reset routine)
				reg_programCounter = littleEndianWord(rw(mem, 0xFFFC, READ), rw(mem, 0xFFFD, READ));
			}

			// executes instructions at programCounter while cycles is greater than 0
			void execute(uint32_t &cycles, MEMORY &mem) {
				while (cycles > 0) {
					BYTE instruction = fetch(mem);
					switch (instruction) {
						case ins_lda_im:
							{
								reg_acc = fetch(mem);
								setLoadFlags(reg_acc);
							}
							break;
						case ins_lda_zp:
							{
								reg_acc = rw(mem, fetch(mem), READ);
								setLoadFlags(reg_acc);
							}
							break;
						case ins_lda_zpx:
							{
								reg_acc = rw(mem, zeroPageXAddressing(cycles, fetch(mem)), READ);
								setLoadFlags(reg_acc);
							}
							break;
						case ins_lda_abs:
							{
								BYTE addressLowByte = fetch(mem);
								reg_acc = rw(mem, littleEndianWord(addressLowByte, fetch(mem)), READ);
								setLoadFlags(reg_acc);
							}
							break;
						case ins_lda_absx:
							{
								BYTE addressLowByte = fetch(mem);
								reg_acc = rw(mem, absoluteXAddressing(mem, littleEndianWord(addressLowByte, fetch(mem))), READ);
								setLoadFlags(reg_acc);
							}
							break;
						case ins_lda_absy:
							{
								BYTE addressLowByte = fetch(mem);
								reg_acc = rw(mem, absoluteYAddressing(mem, littleEndianWord(addressLowByte, fetch(mem))), READ);
								setLoadFlags(reg_acc);
							}
							break;
						case ins_lda_indx:
							{
								reg_acc = rw(mem, indirectXAddressing(cycles, mem, fetch(mem)), READ);
								setLoadFlags(reg_acc);
							}
							break;
						case ins_lda_indy:
							{
								reg_acc = rw(mem, indirectYAddressing(mem, fetch(mem)), READ);
								setLoadFlags(reg_acc);
							}
							break;
						case ins_ldx_im:
							{
								reg_x = fetch(mem);
								setLoadFlags(reg_x);
							}
							break;
						case ins_ldx_zp:
							{
								reg_x = rw(mem, fetch(mem), READ);
								setLoadFlags(reg_x);
							}
							break;
						case ins_ldx_zpy:
							{
								reg_x = rw(mem, zeroPageYAddressing(cycles, fetch(mem)), READ);
								setLoadFlags(reg_x);
							}
							break;
						case ins_ldx_abs:
							{
								BYTE addressLowByte = fetch(mem);
								reg_x = rw(mem, littleEndianWord(addressLowByte, fetch(mem)), READ);
								setLoadFlags(reg_x);
							}
							break;
						case ins_ldx_absy:
							{
								BYTE addressLowByte = fetch(mem);
								reg_x = rw(mem, absoluteYAddressing(mem, littleEndianWord(addressLowByte, fetch(mem))), READ);
								setLoadFlags(reg_x);
							}
							break;
						case ins_ldy_im:
							{
								reg_y = fetch(mem);
								setLoadFlags(reg_y);
							}
							break;
						case ins_ldy_zp:
							{
								reg_y = rw(mem, fetch(mem), READ);
								setLoadFlags(reg_y);
							}
							break;
						case ins_ldy_zpx:
							{
								reg_y = rw(mem, zeroPageXAddressing(cycles, fetch(mem)), READ);
								setLoadFlags(reg_y);
							}
							break;
						case ins_ldy_abs:
							{
								BYTE addressLowByte = fetch(mem);
								reg_y = rw(mem, littleEndianWord(addressLowByte, fetch(mem)), READ);
								setLoadFlags(reg_y);
							}
							break;
						case ins_ldy_absx:
							{
								BYTE addressLowByte = fetch(mem);
								reg_y = rw(mem, absoluteXAddressing(mem, littleEndianWord(addressLowByte, fetch(mem))), READ);
								setLoadFlags(reg_y);
							}
							break;
						case ins_sta_zp:
							{
								mem[fetch(mem)] = reg_acc;
							}
							break;
						case ins_sta_zpx:
							{
								mem[zeroPageXAddressing(cycles, fetch(mem))] = reg_acc;
							}
							break;
						case ins_sta_abs:
							{
								BYTE addressLowByte = fetch(mem);
								mem[littleEndianWord(addressLowByte, fetch(mem))] = reg_acc;
							}
							break;
						case ins_sta_absx:
							{
								BYTE addressLowByte = fetch(mem);
								mem[absoluteXAddressing(mem, littleEndianWord(addressLowByte, fetch(mem)), true)] = reg_acc;
							}
							break;
						case ins_sta_absy:
							{
								BYTE addressLowByte = fetch(mem);
								mem[absoluteYAddressing(mem, littleEndianWord(addressLowByte, fetch(mem)), true)] = reg_acc;
							}
							break;
						case ins_sta_indx:
							{
								mem[indirectXAddressing(cycles, mem, fetch(mem))] = reg_acc;
							}
							break;
						case ins_sta_indy:
							{
								mem[indirectYAddressing(mem, fetch(mem), true)] = reg_acc;
							}
							break;
						case ins_stx_zp:
							{
								mem[fetch(mem)] = reg_x;
							}
							break;
						case ins_stx_zpy:
							{
								mem[zeroPageYAddressing(cycles, fetch(mem))] = reg_x;
							}
							break;
						case ins_stx_abs:
							{
								BYTE addressLowByte = fetch(mem);
								mem[littleEndianWord(addressLowByte, fetch(mem))] = reg_x;
							}
							break;
						case ins_sty_zp:
							{
								mem[fetch(mem)] = reg_y;
							}
							break;
						case ins_sty_zpx:
							{
								mem[zeroPageXAddressing(cycles, fetch(mem))] = reg_y;
							}
							break;
						case ins_sty_abs:
							{
								BYTE addressLowByte = fetch(mem);
								mem[littleEndianWord(addressLowByte, fetch(mem))] = reg_y;
							}
							break;
						case ins_tax:
							{
								transfer(cycles, reg_acc, reg_x);
								setLoadFlags(reg_x);
							}
							break;
						case ins_tay:
							{
								transfer(cycles, reg_acc, reg_x);
								setLoadFlags(reg_y);
							}
							break;
						case ins_txa:
							{
								transfer(cycles, reg_x, reg_acc);
								setLoadFlags(reg_acc);
							}
							break;
						case ins_tya:
							{
								transfer(cycles, reg_y, reg_acc);
								setLoadFlags(reg_acc);
							}
							break;
						case ins_tsx:
							{
								transfer(cycles, reg_stackPointer, reg_x);
								setLoadFlags(reg_x);
							}
							break;
						case ins_txs:
							{
								transfer(cycles, reg_x, reg_stackPointer);
								setLoadFlags(reg_stackPointer);
							}
							break;
						case ins_pha:
							{
								pushStack(cycles, mem, reg_acc);
							}
							break;
						case ins_php:
							{
								// pushes byte with representation NV11DIZC (from flag names, V represents overflow)
								pushStack(cycles, mem, fl_carry | fl_zero << 1 | fl_interr << 2 | fl_dec << 3 | 0b00110000 | fl_oflow << 6 | fl_neg << 7);
							}
							break;
						case ins_pla:
							{
								transfer(cycles, pullStack(cycles, mem), reg_acc);
								setLoadFlags(reg_acc);
							}
							break;
						case ins_plp:
							{
								BYTE status = pullStack(cycles, mem);
								fl_carry = ((status & 0b00000001) > 0);
								fl_zero = ((status & 0b00000010) > 0);
								fl_interr = ((status & 0b00000100) > 0);
								fl_dec = ((status & 0b00001000) > 0);
								fl_oflow = ((status & 0b01000000) > 0);
								fl_neg = ((status & 0b10000000) > 0);
								cycles--;
							}
							break;
						case ins_and_im:
							{
								reg_acc &= fetch(mem);
								setLoadFlags(reg_acc);
							}
							break;
						case ins_and_zp:
							{
								reg_acc &= rw(mem, fetch(mem), READ);
								setLoadFlags(reg_acc);
							}
							break;
						case ins_and_zpx:
							{
								reg_acc &= rw(mem, zeroPageXAddressing(cycles, fetch(mem)), READ);
								setLoadFlags(reg_acc);
							}
							break;
						case ins_and_abs:
							{
								BYTE addressLowByte = fetch(mem);
								reg_acc &= rw(mem, littleEndianWord(addressLowByte, fetch(mem)), READ);
								setLoadFlags(reg_acc);
							}
							break;
						case ins_and_absx:
							{
								BYTE addressLowByte = fetch(mem);
								reg_acc &= rw(mem, absoluteXAddressing(mem, littleEndianWord(addressLowByte, fetch(mem))), READ);
								setLoadFlags(reg_acc);
							}
							break;
						case ins_and_absy:
							{
								BYTE addressLowByte = fetch(mem);
								reg_acc &= rw(mem, absoluteYAddressing(mem, littleEndianWord(addressLowByte, fetch(mem))), READ);
								setLoadFlags(reg_acc);
							}
							break;
						case ins_and_indx:
							{
								reg_acc &= rw(mem, indirectXAddressing(cycles, mem, fetch(mem)), READ);
								setLoadFlags(reg_acc);
							}
							break;
						case ins_and_indy:
							{
								reg_acc &= rw(mem, indirectYAddressing(mem, fetch(mem)), READ);
								setLoadFlags(reg_acc);
							}
							break;
						case ins_eor_im:
							{
								reg_acc ^= fetch(mem);
								setLoadFlags(reg_acc);
							}
							break;
						case ins_eor_zp:
							{
								reg_acc ^= rw(mem, fetch(mem), READ);
								setLoadFlags(reg_acc);
							}
							break;
						case ins_eor_zpx:
							{
								reg_acc ^= rw(mem, zeroPageXAddressing(cycles, fetch(mem)), READ);
								setLoadFlags(reg_acc);
							}
							break;
						case ins_eor_abs:
							{
								BYTE addressLowByte = fetch(mem);
								reg_acc ^= rw(mem, littleEndianWord(addressLowByte, fetch(mem)), READ);
								setLoadFlags(reg_acc);
							}
							break;
						case ins_eor_absx:
							{
								BYTE addressLowByte = fetch(mem);
								reg_acc ^= rw(mem, absoluteXAddressing(mem, littleEndianWord(addressLowByte, fetch(mem))), READ);
								setLoadFlags(reg_acc);
							}
							break;
						case ins_eor_absy:
							{
								BYTE addressLowByte = fetch(mem);
								reg_acc ^= rw(mem, absoluteYAddressing(mem, littleEndianWord(addressLowByte, fetch(mem))), READ);
								setLoadFlags(reg_acc);
							}
							break;
						case ins_eor_indx:
							{
								reg_acc ^= rw(mem, indirectXAddressing(cycles, mem, fetch(mem)), READ);
								setLoadFlags(reg_acc);
							}
							break;
						case ins_eor_indy:
							{
								reg_acc ^= rw(mem, indirectYAddressing(mem, fetch(mem)), READ);
								setLoadFlags(reg_acc);
							}
							break;
						case ins_ora_im:
							{
								reg_acc |= fetch(mem);
								setLoadFlags(reg_acc);
							}
							break;
						case ins_ora_zp:
							{
								reg_acc |= rw(mem, fetch(mem), READ);
								setLoadFlags(reg_acc);
							}
							break;
						case ins_ora_zpx:
							{
								reg_acc |= rw(mem, zeroPageXAddressing(cycles, fetch(mem)), READ);
								setLoadFlags(reg_acc);
							}
							break;
						case ins_ora_abs:
							{
								BYTE addressLowByte = fetch(mem);
								reg_acc |= rw(mem, littleEndianWord(addressLowByte, fetch(mem)), READ);
								setLoadFlags(reg_acc);
							}
							break;
						case ins_ora_absx:
							{
								BYTE addressLowByte = fetch(mem);
								reg_acc |= rw(mem, absoluteXAddressing(mem, littleEndianWord(addressLowByte, fetch(mem))), READ);
								setLoadFlags(reg_acc);
							}
							break;
						case ins_ora_absy:
							{
								BYTE addressLowByte = fetch(mem);
								reg_acc |= rw(mem, absoluteYAddressing(mem, littleEndianWord(addressLowByte, fetch(mem))), READ);
								setLoadFlags(reg_acc);
							}
							break;
						case ins_ora_indx:
							{
								reg_acc |= rw(mem, indirectXAddressing(cycles, mem, fetch(mem)), READ);
								setLoadFlags(reg_acc);
							}
							break;
						case ins_ora_indy:
							{
								reg_acc |= rw(mem, indirectYAddressing(mem, fetch(mem)), READ);
								setLoadFlags(reg_acc);
							}
							break;
						case ins_bit_zp:
							{
								BYTE value = fetch(mem);
								fl_zero = (reg_acc & value == 0);
								fl_oflow = (value & 0b01000000 > 0);
								fl_neg = (value & 0b10000000 > 0);
								cycles--;
							}
							break;
						case ins_bit_abs:
							{
								BYTE addressLowByte = fetch(mem);
								BYTE value = rw(mem, littleEndianWord(addressLowByte, fetch(mem)), READ);
								fl_zero = (reg_acc & value == 0);
								fl_oflow = (value & 0b01000000 > 0);
								fl_neg = (value & 0b10000000 > 0);
								cycles--;
							}
							break;
						case ins_adc_im:
							{
								addSetFlags(reg_acc, fetch(mem));
							}
							break;
						case ins_adc_zp:
							{
								addSetFlags(reg_acc, rw(mem, fetch(mem), READ));
							}
							break;
						case ins_adc_zpx:
							{
								addSetFlags(reg_acc, rw(mem, zeroPageXAddressing(cycles, fetch(mem)), READ));
							}
							break;
						case ins_adc_abs:
							{
								BYTE addressLowByte = fetch(mem);
								addSetFlags(reg_acc, rw(mem, littleEndianWord(addressLowByte, fetch(mem)), READ));
							}
							break;
						case ins_adc_absx:
							{
								BYTE addressLowByte = fetch(mem);
								addSetFlags(reg_acc, rw(mem, absoluteXAddressing(mem, littleEndianWord(addressLowByte, fetch(mem))), READ));
							}
							break;
						case ins_adc_indx:
							{
								addSetFlags(reg_acc, rw(mem, indirectXAddressing(cycles, mem, fetch(mem)), READ));
							}
							break;
						case ins_adc_indy:
							{
								addSetFlags(reg_acc, rw(mem, indirectYAddressing(mem, fetch(mem)), READ));
							}
							break;
						case ins_sbc_im:
							{
								subtractSetFlags(reg_acc, fetch(mem));
							}
							break;
						case ins_sbc_zp:
							{
								subtractSetFlags(reg_acc, rw(mem, fetch(mem), READ));
							}
							break;
						case ins_sbc_zpx:
							{
								subtractSetFlags(reg_acc, rw(mem, zeroPageXAddressing(cycles, fetch(mem)), READ));
							}
							break;
						case ins_sbc_abs:
							{
								BYTE addressLowByte = fetch(mem);
								subtractSetFlags(reg_acc, rw(mem, littleEndianWord(addressLowByte, fetch(mem)), READ));
							}
							break;
						case ins_sbc_absx:
							{
								BYTE addressLowByte = fetch(mem);
								subtractSetFlags(reg_acc, rw(mem, absoluteXAddressing(mem, littleEndianWord(addressLowByte, fetch(mem))), READ));
							}
							break;
						case ins_sbc_indx:
							{
								subtractSetFlags(reg_acc, rw(mem, indirectXAddressing(cycles, mem, fetch(mem)), READ));
							}
							break;
						case ins_sbc_indy:
							{
								subtractSetFlags(reg_acc, rw(mem, indirectYAddressing(mem, fetch(mem)), READ));
							}
							break;
						case ins_cmp_im:
							{
								compare(reg_acc, fetch(mem));
							}
							break;
						case ins_cmp_zp:
							{
								compare(reg_acc, rw(mem, fetch(mem), READ));
							}
							break;
						case ins_cmp_zpx:
							{
								compare(reg_acc, rw(mem, zeroPageXAddressing(cycles, fetch(mem)), READ));
							}
							break;
						case ins_cmp_abs:
							{
								BYTE addressLowByte = fetch(mem);
								compare(reg_acc, rw(mem, littleEndianWord(addressLowByte, fetch(mem)), READ));
							}
							break;
						case ins_cmp_absx:
							{
								BYTE addressLowByte = fetch(mem);
								compare(reg_acc, rw(mem, absoluteXAddressing(mem, littleEndianWord(addressLowByte, fetch(mem))), READ));
							}
							break;
						case ins_cmp_indx:
							{
								compare(reg_acc, rw(mem, indirectXAddressing(cycles, mem, fetch(mem)), READ));
							}
							break;
						case ins_cmp_indy:
							{
								compare(reg_acc, rw(mem, indirectYAddressing(mem, fetch(mem)), READ));
							}
							break;
						case ins_cpx_im:
							{
								compare(reg_x, fetch(mem));
							}
							break;
						case ins_cpx_zp:
							{
								compare(reg_x, rw(mem, fetch(mem), READ));
							}
							break;
						case ins_cpx_abs:
							{
								BYTE addressLowByte = fetch(mem);
								compare(reg_x, rw(mem, littleEndianWord(addressLowByte, fetch(mem)), READ));
							}
							break;
						case ins_cpy_im:
							{
								compare(reg_y, fetch(mem));
							}
							break;
						case ins_cpy_zp:
							{
								compare(reg_y, rw(mem, fetch(mem), READ));
							}
							break;
						case ins_cpy_abs:
							{
								BYTE addressLowByte = fetch(mem);
								compare(reg_y, rw(mem, littleEndianWord(addressLowByte, fetch(mem)), READ));
							}
							break;
					}
				}
			}

			WORD reg_programCounter;	// 16-bit program counter register
			BYTE reg_stackPointer;		// 8-bit stack pointer register
			BYTE reg_acc;				// 8-bit accumulator register
			BYTE reg_x;					// 8-bit x register
			BYTE reg_y;					// 8-bit y register

			BYTE fl_carry:1;			// 1-bit carry flag
			BYTE fl_zero:1;				// 1-bit zero flag
			BYTE fl_interr:1;			// 1-bit interrupt flag
			BYTE fl_dec:1;				// 1-bit decimal flag
										// during a processor status stack push, bit 4 is set to 1 if pushed from a PHP or BRK instruction or to 0 if pushed from an /IRQ or /NMI signal being pulled low
										// during a processor status stack push, bit 5 is always set to 1
			BYTE fl_oflow:1;			// 1-bit overflow flag
			BYTE fl_neg:1;				// 1-bit negative flag

			// reads and returns next byte at programCounter. Increments programCounter (1 cycle)
			BYTE fetch(MEMORY &mem) {
				BYTE data = mem[reg_programCounter];
				reg_programCounter++;
				return data;
			}

			// reads/writes and returns byte at absolute address (from 0x0000 to 0xFFFF) (1 cycle)
			BYTE rw(MEMORY &mem, WORD address, bool rw, BYTE data = 0x00) {
				BYTE value;
				if (rw == READ) {
					value = mem[address];
				} else {
					mem[address] = data;
					value = data;
				}
				return value;
			}

			// pushes value to stack (address reg_stackPointer | 0x0100) and increments stack pointer (2 cycles)
			BYTE pushStack(uint32_t &cycles, MEMORY &mem, BYTE value) {
				mem[reg_stackPointer | 0x0100] = value;
				reg_stackPointer--;
				cycles--;
				return value;
			}

			// pulls and returns value from stack (address reg_stackPointer | 0x0100) and decrements stack pointer (2 cycles)
			BYTE pullStack(uint32_t &cycles, MEMORY &mem) {
				BYTE value = mem[reg_stackPointer | 0x0100];
				reg_stackPointer++;
				cycles--;
				return value;
			}

			// returns effective address of zero-page X addressing mode (1 cycle)
			WORD zeroPageXAddressing(uint32_t &cycles, BYTE address) {
				cycles--;
				return (address + reg_x) & 0x00ff;
			}

			// returns effective address of zero-page Y addressing mode (1 cycle)
			WORD zeroPageYAddressing(uint32_t &cycles, BYTE address) {
				cycles--;
				return (address + reg_y) & 0x00ff;
			}

			// returns effective address of absolute X addressing mode (1 cycle in case of page cross, 0 otherwise)
			WORD absoluteXAddressing(MEMORY &mem, WORD address, bool extraCycle = false) {
				address += reg_x;
				if ((address & 0x00ff) < reg_x || extraCycle) {
					// extra cycle when page boundary is crossed
					mem[address];
				}
				return address;
			}

			// returns effective address of absolute Y addressing mode (1 cycle in case of page cross, 0 otherwise)
			WORD absoluteYAddressing(MEMORY &mem, WORD address, bool extraCycle = false) {
				address += reg_y;
				if ((address & 0x00ff) < reg_y || extraCycle) {
					// extra cycle when page boundary is crossed
					mem[address];
				}
				return address;
			}

			// returns effective address of indirect X (indexed indirect) addressing more (3 cycles)
			WORD indirectXAddressing(uint32_t &cycles, MEMORY &mem, BYTE address) {
				address += reg_x;
				cycles--;
				BYTE lowByte = rw(mem, address, READ);
				address++;
				return littleEndianWord(lowByte, rw(mem, address, READ));
			}

			// returns effective address of indirect Y (indirect indexed) addressing more (3 cycles in case of page cross, 2 otherwise)
			WORD indirectYAddressing(MEMORY &mem, BYTE address, bool extraCycle = false) {
				BYTE lowByte = rw(mem, address, READ);
				address++; // ISSUE : 
				WORD effectiveAddress = littleEndianWord(lowByte, rw(mem, address, READ));
				effectiveAddress += reg_y;
				if ((effectiveAddress & 0x00ff) < reg_y || extraCycle) {
					// extra cycle when page boundary is crossed
					mem[effectiveAddress];
				}
				return effectiveAddress;
			}

			// loads a register with a defined value and sets appropriate flags (0 cycles)
			void setLoadFlags(BYTE reg) {
				fl_zero = (reg == 0);
				fl_neg = (0b10000000 & reg) > 0;
			}

			// adds a register and a value together and sets appropriate flags (0 cycles)
			void addSetFlags(BYTE &reg, BYTE input) {
				BYTE result = reg + input + fl_carry;
				fl_carry = (result < reg);
				// sets overflow flag if bit 7 of both inputs are different from bit 7 of result (therefore, bit 7 of both inputs are the same while bit 7 of result is different)
				fl_oflow = ((reg ^ result) & (input ^ result) & 0b10000000) > 0;
				reg = result;
				setLoadFlags(reg);
			}

			// subtracts a value from a register and sets appropriate flags (0 cycles)			
			void subtractSetFlags(BYTE &reg, BYTE input) {
				BYTE result = reg - input - (~fl_carry);
				// sets the carry flag if no borrow was needed
				fl_carry = (reg_acc >= input);
				// sets overflow flag with the same condition as addSetFlags, except we replace the input with (255 - input) (adding two inputs and subtracting (256 - second input) from the first input are the same in 8-bit values; the final jump between 255 and 256 is done by the borrow (~carry) bit.)
				fl_oflow = ((reg ^ result) & ((255 - input) ^ result) & 0b10000000) > 0;
				reg = result;
				setLoadFlags(reg);
			}

			// compares a register with a value to set appropriate flags (0 cycles)
			void compare(BYTE reg, BYTE input) {
				reg -= input;
				setLoadFlags(result);
				fl_carry = (result <= input);
			}

			// transfers contents of a register to another
			void transfer(uint32_t &cycles, BYTE firstReg, BYTE &secondReg) {
				secondReg = firstReg;
				cycles--;
			}

			// returns a low endian word formed from two bytes
			WORD littleEndianWord(BYTE lowByte, BYTE highByte) {
				return lowByte | (WORD)(highByte) << 8;
			}
	}; // struct CPU
} // namespace m6502

#endif // ifndef _6502_H