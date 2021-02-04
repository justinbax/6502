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
			static constexpr BYTE ins_lda_abs = 0xAD;	// absolute LOAD A instruction (4 cycles, 3 bytes. Affects zero and negative flags)
			static constexpr BYTE ins_lda_absx = 0xBD;	// absolute X LOAD A instruction (4-5 cycles, 3 bytes. Affects zero and negative flags)
			static constexpr BYTE ins_lda_absy = 0xB9;	// absolute Y LOAD A instruction (4-5 cycles, 3 bytes. Affects zero and negative flags)
			static constexpr BYTE ins_lda_indx = 0xA1;	// indirect X LOAD A instruction (6 cycles, 2 bytes. Affects zero and negative flags)
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

			static constexpr BYTE ins_inc_zp = 0xE6;	// zero-page INCREMENT instruction (2 bytes, 5 cycles. Affects zero and negative flags)
			static constexpr BYTE ins_inc_zpx = 0xF6;	// zero-page X INCREMENT instruction (2 bytes, 6 cycles. Affects zero and negative flags)
			static constexpr BYTE ins_inc_abs = 0xEE;	// absolute INCREMENT instruction (3 bytes, 6 cycles. Affects zero and negative flags)
			static constexpr BYTE ins_inc_absx = 0xFE;	// absolute X INCREMENT instruction (3 bytes, 7 cycles. Affects zero and negative flags)
			static constexpr BYTE ins_inx = 0xE8;		// INCREMENT X instruction (1 byte, 2 cycles. Affects zero and negative flags)
			static constexpr BYTE ins_iny = 0xC8;		// INCREMENT Y instruction (1 byte, 2 cycles. Affects zero and negative flags)

			static constexpr BYTE ins_dec_zp = 0xC6;	// zero-page DECREMENT instruction (2 bytes, 5 cycles. Affects zero and negative flags)
			static constexpr BYTE ins_dec_zpx = 0xD6;	// zero-page X DECREMENT instruction (2 bytes, 6 cycles. Affects zero and negative flags)
			static constexpr BYTE ins_dec_abs = 0xCE;	// absolute DECREMENT instruction (3 bytes, 6 cycles. Affects zero and negative flags)
			static constexpr BYTE ins_dec_absx = 0xDE;	// absolute X DECREMENT instruction (3 bytes, 7 cycles. Affects zero and negative flags)
			static constexpr BYTE ins_dex = 0xCA;		// DECREMENT X instruction (1 byte, 2 cycles. Affects zero and negative flags)
			static constexpr BYTE ins_dey = 0x88;		// DECREMENT Y instruction (1 byte, 2 cycles. Affects zero and negative flags)

			static constexpr BYTE ins_asl_acc = 0x0A;	// A SHIFT LEFT instruction (1 byte, 2 cycles. Affects carry, zero and negative flags)
			static constexpr BYTE ins_asl_zp = 0x06;	// zero-page SHIFT LEFT instruction (2 bytes, 5 cycles. Affects carry, zero and negative flags)
			static constexpr BYTE ins_asl_zpx = 0x16;	// zero-page X SHIFT LEFT instruction (2 bytes, 6 cycles. Affects carry, zero and negative flags)
			static constexpr BYTE ins_asl_abs = 0x0E;	// absolute SHIFT LEFT instruction (3 bytes, 6 cycles. Affects carry, zero and negative flags)
			static constexpr BYTE ins_asl_absx = 0x1E;	// absolute X SHIFT LEFT instruction (3 bytes, 7 cycles. Affects carry, zero and negative flags)

			static constexpr BYTE ins_lsr_acc = 0x4A;	// A SHIFT RIGHT instruction (1 byte, 2 cycles. Affects carry, zero and negative flags)
			static constexpr BYTE ins_lsr_zp = 0x46;	// zero-page SHIFT RIGHT instruction (2 bytes, 5 cycles. Affects carry, zero and negative flags)
			static constexpr BYTE ins_lsr_zpx = 0x56;	// zero-page X SHIFT RIGHT instruction (2 bytes, 6 cycles. Affects carry, zero and negative flags)
			static constexpr BYTE ins_lsr_abs = 0x4E;	// absolute SHIFT RIGHT instruction (3 bytes, 6 cycles. Affects carry, zero and negative flags)
			static constexpr BYTE ins_lsr_absx = 0x5E;	// absolute X SHIFT RIGHT instruction (3 bytes, 7 cycles. Affects carry, zero and negative flags)

			static constexpr BYTE ins_rol_acc = 0x2A;	// A ROTATE LEFT instruction (1 byte, 2 cycles. Affects carry, zero and negative flags)
			static constexpr BYTE ins_rol_zp = 0x26;	// zero-page ROTATE LEFT instruction (2 bytes, 5 cycles. Affects carry, zero and negative flags)
			static constexpr BYTE ins_rol_zpx = 0x36;	// zero-page X ROTATE LEFT instruction (2 bytes, 6 cycles. Affects carry, zero and negative flags)
			static constexpr BYTE ins_rol_abs = 0x2E;	// absolute ROTATE LEFT instruction (3 bytes, 6 cycles. Affects carry, zero and negative flags)
			static constexpr BYTE ins_rol_absx = 0x3E;	// absolute X ROTATE LEFT instruction (3 bytes, 7 cycles. Affects carry, zero and negative flags)

			static constexpr BYTE ins_ror_acc = 0x6A;	// A ROTATE RIGHT instruction (1 byte, 2 cycles. Affects carry, zero and negative flags)
			static constexpr BYTE ins_ror_zp = 0x66;	// zero-page ROTATE RIGHT instruction (2 bytes, 5 cycles. Affects carry, zero and negative flags)
			static constexpr BYTE ins_ror_zpx = 0x76;	// zero-page X ROTATE RIGHT instruction (2 bytes, 6 cycles. Affects carry, zero and negative flags)
			static constexpr BYTE ins_ror_abs = 0x6E;	// absolute ROTATE RIGHT instruction (3 bytes, 6 cycles. Affects carry, zero and negative flags)
			static constexpr BYTE ins_ror_absx = 0x7E;	// absolute X ROTATE RIGHT instruction (3 bytes, 7 cycles. Affects carry, zero and negative flags)

			static constexpr BYTE ins_jmp_abs = 0x4C;	// absolute JUMP instruction (3 bytes, 3 cycles. Does not affect any flag)
			static constexpr BYTE ins_jmp_ind = 0x6C;	// indirect JUMP instruction (3 bytes, 5 cycles. Does not affect any flag)
			static constexpr BYTE ins_jsr_abs = 0x20;	// absolute JUMP TO SUBROUTINE instruction (3 bytes, 6 cycles. Does not affect any flag)
			static constexpr BYTE ins_rts = 0x60;		// RETURN FROM. SUBROUTINE instruction (1 byte, 6 cycles. Does not affect any flag)

			static constexpr BYTE ins_bcc = 0x90;		// BRANCH IF CARRY CLEAR instruction (2 bytes, 2-4 cycles. Does not affect any flag)
			static constexpr BYTE ins_bcs = 0xB0;		// BRANCH IF CARRY SET instruction (2 bytes, 2-4 cycles. Does not affect any flag)
			static constexpr BYTE ins_beq = 0xF0;		// BRANCH IF EQUAL instruction (2 bytes, 2-4 cycles. Does not affect any flag)
			static constexpr BYTE ins_bmi = 0x30;		// BRANCH IF MINUS instruction (2 bytes, 2-4 cycles. Does not affect any flag)
			static constexpr BYTE ins_bne = 0xD0;		// BRANCH IF NOT EQUAL instruction (2 bytes, 2-4 cycles. Does not affect any flag)
			static constexpr BYTE ins_bpl = 0x10;		// BRANCH IF POSITIVE instruction (2 bytes, 2-4 cycles. Does not affect any flag)
			static constexpr BYTE ins_bvc = 0x50;		// BRANCH IF OVERFLOW CLEAR instruction (2 bytes, 2-4 cycles. Does not affect any flag)
			static constexpr BYTE ins_bvs = 0x70;		// BRANCH IF OVERFLOW SET instruction (2 bytes, 2-4 cycles. Does not affect any flag)
			
			static constexpr BYTE ins_clc = 0x18;		// CLEAR CARRY instruction (1 byte, 2 cycles. Affects carry flag)
			static constexpr BYTE ins_cld = 0xD8;		// CLEAR DECIMAL instruction (1 byte, 2 cycles. Affects decimal flag)
			static constexpr BYTE ins_cli = 0x58;		// CLEAR INTERRUPT DISABLE instruction (1 byte, 2 cycles. Affects interrupt flag)
			static constexpr BYTE ins_clv = 0xB8;		// CLEAR OVERFLOW instruction (1 byte, 2 cycles. Affects overflow flag)
			static constexpr BYTE ins_sec = 0x38;		// SET CARRY instruction (1 byte, 2 cycles. Affects carry flag)
			static constexpr BYTE ins_sed = 0xF8;		// SET DECIMAL instruction (1 byte, 2 cycles. Affects decimal flag)
			static constexpr BYTE ins_sei = 0x78;		// SET INTERRUPT DISABLE instruction (1 byte, 2 cycles. Affects interrupt flag)

			static constexpr BYTE ins_brk = 0x00;		// BREAK instruction (2 bytes, 7 cycles. Does not affect any flag)
			static constexpr BYTE ins_nop = 0xEA;		// NO OPERATION instruction (1 byte, 2 cycles. Does not affect any flag)
			static constexpr BYTE ins_rti = 0x40;		// RETURN FROM INTERRUPT instruction (1 byte, 6 cycles. Affects all flags)

			// sends a reset signal to reset computer state (7 cycles)
			void reset(uint32_t &cycles, MEMORY &mem) {
				reg_programCounter = 0x0000;
				reg_stackPointer = 0x00;
				cycles--;
				cycles--;
				fl_carry = fl_zero = fl_interr = fl_dec = fl_oflow = fl_neg = false;
				reg_acc = reg_x = reg_y = 0;
				// three fake stack accesses to set the stack pointer to 0xFDs
				rw(mem, reg_stackPointer | 0x0100, READ);
				reg_stackPointer--;
				rw(mem, reg_stackPointer | 0x0100, READ);
				reg_stackPointer--;
				rw(mem, reg_stackPointer | 0x0100, READ);
				reg_stackPointer--;
				// stores contents at 0xFFFC and 0xFFFD (little-endian) in program counter (contains location of reset routine)
				reg_programCounter = littleEndianWord(rw(mem, 0xFFFC, READ), rw(mem, 0xFFFD, READ));
			}

			// executes instructions at programCounter while cycles is greater than 0
			void execute(uint32_t &cycles, MEMORY &mem) {
				while (cycles > 0) {
					BYTE instruction = fetch(mem);
					std::cout << std::hex << (int)(instruction) << "   (address " << reg_programCounter << ")" << std::endl;
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
								rw(mem, fetch(mem), WRITE, reg_acc);
							}
							break;
						case ins_sta_zpx:
							{
								rw(mem, zeroPageXAddressing(cycles, fetch(mem)), WRITE, reg_acc);
							}
							break;
						case ins_sta_abs:
							{
								BYTE addressLowByte = fetch(mem);
								rw(mem, littleEndianWord(addressLowByte, fetch(mem)), WRITE, reg_acc);
							}
							break;
						case ins_sta_absx:
							{
								BYTE addressLowByte = fetch(mem);
								rw(mem, absoluteXAddressing(mem, littleEndianWord(addressLowByte, fetch(mem)), true), WRITE, reg_acc);
							}
							break;
						case ins_sta_absy:
							{
								BYTE addressLowByte = fetch(mem);
								rw(mem, absoluteYAddressing(mem, littleEndianWord(addressLowByte, fetch(mem)), true), WRITE, reg_acc);
							}
							break;
						case ins_sta_indx:
							{
								rw(mem, indirectXAddressing(cycles, mem, fetch(mem)), WRITE, reg_acc);
							}
							break;
						case ins_sta_indy:
							{
								rw(mem, indirectYAddressing(mem, fetch(mem), true), WRITE, reg_acc);
							}
							break;
						case ins_stx_zp:
							{
								rw(mem, fetch(mem), WRITE, reg_x);
							}
							break;
						case ins_stx_zpy:
							{
								rw(mem, zeroPageYAddressing(cycles, fetch(mem)), WRITE, reg_x);
							}
							break;
						case ins_stx_abs:
							{
								BYTE addressLowByte = fetch(mem);
								rw(mem, littleEndianWord(addressLowByte, fetch(mem)), WRITE, reg_x);
							}
							break;
						case ins_sty_zp:
							{
								rw(mem, fetch(mem), WRITE, reg_y);
							}
							break;
						case ins_sty_zpx:
							{
								rw(mem, zeroPageXAddressing(cycles, fetch(mem)), WRITE, reg_y);
							}
							break;
						case ins_sty_abs:
							{
								BYTE addressLowByte = fetch(mem);
								rw(mem, littleEndianWord(addressLowByte, fetch(mem)), WRITE, reg_y);
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
						case ins_inc_zp:
							{
								BYTE address = fetch(mem);
								BYTE value = rw(mem, address, READ);
								value++;
								cycles--;
								rw(mem, address, WRITE, value);
								setLoadFlags(value);
							}
							break;
						case ins_inc_zpx:
							{
								BYTE address = zeroPageXAddressing(cycles, fetch(mem));
								BYTE value = rw(mem, address, READ);
								value++;
								cycles--;
								rw(mem, address, WRITE, value);
								setLoadFlags(value);
							}
							break;
						case ins_inc_abs:
							{
								BYTE addressLowByte = fetch(mem);
								BYTE address = littleEndianWord(addressLowByte, fetch(mem));
								BYTE value = rw(mem, address, READ);
								value++;
								cycles--;
								rw(mem, address, WRITE, value);
								setLoadFlags(value);
							}
							break;
						case ins_inc_absx:
							{
								BYTE addressLowByte = fetch(mem);
								BYTE address = absoluteXAddressing(mem, littleEndianWord(addressLowByte, fetch(mem)), true);
								BYTE value = rw(mem, address, READ);
								value++;
								cycles--;
								rw(mem, address, WRITE, value);
								setLoadFlags(value);
							}
							break;
						case ins_inx:
							{
								reg_x++;
								cycles--;
								setLoadFlags(reg_x);
							}
							break;
						case ins_iny:
							{
								reg_y++;cycles--;
								setLoadFlags(reg_y);
							}
							break;
						case ins_dec_zp:
							{
								BYTE address = fetch(mem);
								BYTE value = rw(mem, address, READ);
								value--;
								cycles--;
								rw(mem, address, WRITE, value);
								setLoadFlags(value);
							}
							break;
						case ins_dec_zpx:
							{
								BYTE address = zeroPageXAddressing(cycles, fetch(mem));
								BYTE value = rw(mem, address, READ);
								value--;
								cycles--;
								rw(mem, address, WRITE, value);
								setLoadFlags(value);
							}
							break;
						case ins_dec_abs:
							{
								BYTE addressLowByte = fetch(mem);
								BYTE address = littleEndianWord(addressLowByte, fetch(mem));
								BYTE value = rw(mem, address, READ);
								value--;
								cycles--;
								rw(mem, address, WRITE, value);
								setLoadFlags(value);
							}
							break;
						case ins_dec_absx:
							{
								BYTE addressLowByte = fetch(mem);
								BYTE address = absoluteXAddressing(mem, littleEndianWord(addressLowByte, fetch(mem)), true);
								BYTE value = rw(mem, address, READ);
								value--;
								cycles--;
								rw(mem, address, WRITE, value);
								setLoadFlags(value);
							}
							break;
						case ins_dex:
							{
								reg_x--;
								cycles--;
								setLoadFlags(reg_x);
							}
							break;
						case ins_dey:
							{
								reg_y--;
								cycles--;
								setLoadFlags(reg_y);
							}
							break;
						case ins_asl_acc:
							{
								fl_carry = (reg_acc & 0b10000000 > 0);
								reg_acc <<= 1;
								cycles--;
								setLoadFlags(reg_acc);
							}
							break;
						case ins_asl_zp:
							{
								BYTE address = fetch(mem);
								BYTE value = rw(mem, address, READ);
								fl_carry = (value & 0b10000000 > 0);
								value <<= 1;
								cycles--;
								rw(mem, address, WRITE, value);
								setLoadFlags(value);
							}
							break;
						case ins_asl_zpx:
							{
								BYTE address = zeroPageXAddressing(cycles, fetch(mem));
								BYTE value = rw(mem, address, READ);
								fl_carry = (value & 0b10000000 > 0);
								value <<= 1;
								cycles--;
								rw(mem, address, WRITE, value);
								setLoadFlags(value);
							}
							break;
						case ins_asl_abs:
							{
								BYTE addressLowByte = fetch(mem);
								BYTE address = littleEndianWord(addressLowByte, fetch(mem));
								BYTE value = rw(mem, address, READ);
								fl_carry = (value & 0b10000000 > 0);
								value <<= 1;
								cycles--;
								rw(mem, address, WRITE, value);
								setLoadFlags(value);
							}
							break;
						case ins_asl_absx:
							{
								BYTE addressLowByte = fetch(mem);
								BYTE address = absoluteXAddressing(mem, littleEndianWord(addressLowByte, fetch(mem)));
								BYTE value = rw(mem, address, READ);
								fl_carry = (value & 0b10000000 > 0);
								value <<= 1;
								cycles--;
								rw(mem, address, WRITE, value);
								setLoadFlags(value);
							}
							break;
						case ins_lsr_acc:
							{
								fl_carry = (reg_acc & 0b00000001 > 0);
								reg_acc >>= 1;
								cycles--;
								setLoadFlags(reg_acc);
							}
							break;
						case ins_lsr_zp:
							{
								BYTE address = fetch(mem);
								BYTE value = rw(mem, address, READ);
								fl_carry = (value & 0b00000001 > 0);
								value >>= 1;
								cycles--;
								rw(mem, address, WRITE, value);
								setLoadFlags(value);
							}
							break;
						case ins_lsr_zpx:
							{
								BYTE address = zeroPageXAddressing(cycles, fetch(mem));
								BYTE value = rw(mem, address, READ);
								fl_carry = (value & 0b00000001 > 0);
								value >>= 1;
								cycles--;
								rw(mem, address, WRITE, value);
								setLoadFlags(value);
							}
							break;
						case ins_lsr_abs:
							{
								BYTE addressLowByte = fetch(mem);
								BYTE address = littleEndianWord(addressLowByte, fetch(mem));
								BYTE value = rw(mem, address, READ);
								fl_carry = (value & 0b00000001 > 0);
								value >>= 1;
								cycles--;
								rw(mem, address, WRITE, value);
								setLoadFlags(value);
							}
							break;
						case ins_lsr_absx:
							{
								BYTE addressLowByte = fetch(mem);
								BYTE address = absoluteXAddressing(mem, littleEndianWord(addressLowByte, fetch(mem)));
								BYTE value = rw(mem, address, READ);
								fl_carry = (value & 0b00000001 > 0);
								value >>= 1;
								cycles--;
								rw(mem, address, WRITE, value);
								setLoadFlags(value);
							}
							break;
						case ins_rol_acc:
							{
								bool carry = fl_carry;
								fl_carry = (reg_acc & 0b10000000 > 0);
								reg_acc <<= 1;
								reg_acc += carry;
								cycles--;
								setLoadFlags(reg_acc);
							}
							break;
						case ins_rol_zp:
							{
								bool carry = fl_carry;
								BYTE address = fetch(mem);
								BYTE value = rw(mem, address, READ);
								fl_carry = (value & 0b10000000 > 0);
								value <<= 1;
								value += carry;
								cycles--;
								setLoadFlags(value);
								rw(mem, address, WRITE, value);
							}
							break;
						case ins_rol_zpx:
							{
								bool carry = fl_carry;

								BYTE address = zeroPageXAddressing(cycles, fetch(mem));
								BYTE value = rw(mem, address, READ);
								fl_carry = (value & 0b10000000 > 0);

								value <<= 1;
								value += carry;
								cycles--;
								setLoadFlags(value);
								rw(mem, address, WRITE, value);
							}
							break;
						case ins_rol_abs:
							{
								bool carry = fl_carry;
								BYTE addressLowByte = fetch(mem);
								BYTE address = littleEndianWord(addressLowByte, fetch(mem));
								BYTE value = rw(mem, address, READ);
								fl_carry = (value & 0b10000000 > 0);
								value <<= 1;
								value += carry;
								cycles--;
								setLoadFlags(value);
								rw(mem, address, WRITE, value);
							}
							break;
						case ins_rol_absx:
							{
								bool carry = fl_carry;
								BYTE addressLowByte = fetch(mem);
								BYTE address = absoluteXAddressing(mem, littleEndianWord(addressLowByte, fetch(mem)));
								BYTE value = rw(mem, address, READ);
								fl_carry = (value & 0b10000000 > 0);
								value <<= 1;
								value += carry;
								cycles--;
								setLoadFlags(value);
								rw(mem, address, WRITE, value);
							}
							break;
						case ins_ror_acc:
							{
								bool carry = fl_carry;
								fl_carry = (reg_acc & 0b00000001 > 0);
								reg_acc >>= 1;
								reg_acc |= (carry ? 0b10000000 : 0);
								cycles--;
								setLoadFlags(reg_acc);
							}
							break;
						case ins_ror_zp:
							{
								bool carry = fl_carry;
								BYTE address = fetch(mem);
								BYTE value = rw(mem, address, READ);
								fl_carry = (value & 0b00000001 > 0);
								value >>= 1;
								value |= (carry ? 0b10000000 : 0);
								cycles--;
								rw(mem, address, WRITE, value);
								setLoadFlags(reg_acc);
							}
							break;
						case ins_ror_zpx:
							{
								bool carry = fl_carry;
								BYTE address = zeroPageXAddressing(cycles, fetch(mem));
								BYTE value = rw(mem, address, READ);
								fl_carry = (value & 0b00000001 > 0);
								value >>= 1;
								value |= (carry ? 0b10000000 : 0);
								cycles--;
								rw(mem, address, WRITE, value);
								setLoadFlags(reg_acc);
							}
							break;
						case ins_ror_abs:
							{
								bool carry = fl_carry;
								BYTE addressLowByte = fetch(mem);
								BYTE address = littleEndianWord(addressLowByte, fetch(mem));
								BYTE value = rw(mem, address, READ);
								fl_carry = (value & 0b00000001 > 0);
								value >>= 1;
								value |= (carry ? 0b10000000 : 0);
								cycles--;
								rw(mem, address, WRITE, value);
								setLoadFlags(reg_acc);
							}
							break;
						case ins_ror_absx:
							{
								bool carry = fl_carry;
								BYTE addressLowByte = fetch(mem);
								BYTE address = absoluteXAddressing(mem, littleEndianWord(addressLowByte, fetch(mem)));
								BYTE value = rw(mem, address, READ);
								fl_carry = (value & 0b00000001 > 0);
								value >>= 1;
								value |= (carry ? 0b10000000 : 0);
								cycles--;
								rw(mem, address, WRITE, value);
								setLoadFlags(reg_acc);
							}
							break;
						case ins_jmp_abs:
							{
								BYTE addressLowByte = fetch(mem);
								reg_programCounter = littleEndianWord(addressLowByte, fetch(mem));
							}
							break;
						case ins_jmp_ind:
							{
								// on an original 6502, indirect addressing on a page boundary (first byte on 0xxxFF) results in the effective address being taken from FF of that page and 00 of the same page, and not 00 of the next page
								BYTE addressLowByte = fetch(mem);
								BYTE addressHighByte = fetch(mem);
								BYTE effectiveAddressLowByte = rw(mem, addressLowByte | (WORD)(addressHighByte << 8), READ);
								reg_programCounter = littleEndianWord(effectiveAddressLowByte, rw(mem, addressLowByte | (WORD)(addressHighByte++ << 8), READ));
							}
							break;
						case ins_jsr_abs:
							{
								BYTE addressLowByte = fetch(mem);
								BYTE addressHighByte = fetch(mem);
								pushStack(cycles, mem, (reg_stackPointer + 1) & 0b11110000);
								pushStack(cycles, mem, (reg_stackPointer + 1) & 0b00001111);
								reg_stackPointer = littleEndianWord(addressLowByte, addressHighByte);
								// for some reason the 6502 manages to do the instruction in 6 cycles, yet this does it in 7, to incrementing the cycle count
								cycles++;
							}
							break;
						case ins_rts:
							{
								BYTE addressLowByte = pullStack(cycles, mem);
								reg_stackPointer = littleEndianWord(addressLowByte, pullStack(cycles, mem));
								reg_stackPointer++;
								// extra cycle to arrive at 6 cycles
								cycles--;
							}
							break;
						case ins_bcc:
							{
								branch(cycles, fetch(mem), fl_carry, false);
							}
							break;
						case ins_bcs:
							{
								branch(cycles, fetch(mem), fl_carry, true);
							}
							break;
						case ins_beq:
							{
								branch(cycles, fetch(mem), fl_zero, true);
							}
							break;
						case ins_bmi:
							{
								branch(cycles, fetch(mem), fl_neg, true);
							}
							break;
						case ins_bne:
							{
								branch(cycles, fetch(mem), fl_zero, false);
							}
							break;
						case ins_bpl:
							{
								branch(cycles, fetch(mem), fl_neg, false);
							}
							break;
						case ins_bvc:
							{
								branch(cycles, fetch(mem), fl_oflow, false);
							}
							break;
						case ins_bvs:
							{
								branch(cycles, fetch(mem), fl_oflow, true);
							}
							break;
						case ins_clc:
							{
								fl_carry = false;
							}
							break;
						case ins_cld:
							{
								fl_dec = false;
							}
							break;
						case ins_cli:
							{
								fl_interr = false;
							}
							break;
						case ins_clv:
							{
								fl_oflow = false;
							}
							break;
						case ins_sec:
							{
								fl_carry = true;
							}
							break;
						case ins_sed:
							{
								fl_dec = true;
							}
							break;
						case ins_sei:
							{
								fl_interr = true;
							}
							break;
						case ins_brk:
							{
								cycles--;
								cycles--;
								// pushes program counter and status flags on the stack
								rw(mem, reg_stackPointer | 0x0100, WRITE, (reg_programCounter + 1) & 0b11110000);
								reg_stackPointer--;
								rw(mem, reg_stackPointer | 0x0100, WRITE, (reg_programCounter + 1) & 0b00001111);
								reg_stackPointer--;
								rw(mem, reg_stackPointer | 0x0100, WRITE, (fl_carry | fl_zero << 1 | fl_interr << 2 | fl_dec << 3 | 0b00110000 | fl_oflow << 6 | fl_neg << 7));
								// stores contents of 0xFFFE and 0xFFFF in the program counter
								BYTE programCounterLowByte = rw(mem, 0xFFFE, READ);
								reg_programCounter = littleEndianWord(programCounterLowByte, rw(mem, 0xFFFF, READ));
							}
							break;
						case ins_nop:
							{
								cycles--;
							}
							break;
						case ins_rti:
							{
								cycles--;
								cycles--;
								cycles--;
								// sets program counter and status flags from stack
								BYTE flags = rw(mem, reg_stackPointer | 0x0100, READ);
								reg_stackPointer++;
								fl_carry = (flags & 0b00000001);
								fl_zero = (flags & 0b00000010);
								fl_interr = (flags & 0b00000100);
								fl_dec = (flags & 0b00001000);
								fl_oflow = (flags &0b01000000);
								fl_neg = (flags & 0b10000000);
								BYTE programCounterLowByte = rw(mem, reg_stackPointer | 0x0100, READ);
								reg_stackPointer++;
								reg_programCounter = littleEndianWord(programCounterLowByte, rw(mem, reg_stackPointer | 0x0100, READ));
								reg_stackPointer++;
							}
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
				BYTE data = rw(mem, reg_programCounter, READ);
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
				rw(mem, reg_stackPointer | 0x0100, WRITE, value);
				reg_stackPointer--;
				cycles--;
				return value;
			}

			// pulls and returns value from stack (address reg_stackPointer | 0x0100) and decrements stack pointer (2 cycles)
			BYTE pullStack(uint32_t &cycles, MEMORY &mem) {
				BYTE value = rw(mem, reg_stackPointer | 0x0100, READ);
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
					rw(mem, address, READ);
				}
				return address;
			}

			// returns effective address of absolute Y addressing mode (1 cycle in case of page cross, 0 otherwise)
			WORD absoluteYAddressing(MEMORY &mem, WORD address, bool extraCycle = false) {
				address += reg_y;
				if ((address & 0x00ff) < reg_y || extraCycle) {
					// extra cycle when page boundary is crossed
					rw(mem, address, READ);
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
				address++;
				WORD effectiveAddress = littleEndianWord(lowByte, rw(mem, address, READ));
				effectiveAddress += reg_y;
				if ((effectiveAddress & 0x00ff) < reg_y || extraCycle) {
					// extra cycle when page boundary is crossed
					rw(mem, effectiveAddress, READ);
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
				setLoadFlags(reg);
				fl_carry = (reg <= input);
			}

			// transfers contents of a register to another (1 cycle)
			void transfer(uint32_t &cycles, BYTE firstReg, BYTE &secondReg) {
				secondReg = firstReg;
				cycles--;
			}

			// branches program counter to a new relative location if condition is met (0-2 cycles)
			bool branch(uint32_t &cycles, BYTE offset, bool flag, bool condition) {
				if (flag == condition) {
					BYTE oldPage = reg_programCounter >> 8;
					// unsigned to signed integer with convertion to two's complement if highest bit is set
					offset = (offset & 0b10000000 > 0 ? offset - 256 : offset);
					reg_programCounter += offset;
					cycles--;
					if (oldPage != (reg_programCounter >> 8)) {
						// extra cycle if page is crossed
						cycles--;
					}
					return true;
				}
				return false;
			}

			// returns a low endian word formed from two bytes
			WORD littleEndianWord(BYTE lowByte, BYTE highByte) {
				return lowByte | (WORD)(highByte) << 8;
			}
	}; // struct CPU
} // namespace m6502

#endif // ifndef _6502_H