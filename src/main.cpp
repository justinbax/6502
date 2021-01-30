#include <iostream>

typedef uint8_t BYTE;	// uint8_t (1 byte)
typedef uint16_t WORD;	// uint16_t (2 bytes)

// computer memory struct
struct MEMORY {
	public:
		static constexpr WORD MAX_MEM = 0xFFFF;

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
		void fill(BYTE *nData) {
			WORD i = 0;
			while (i < sizeof(nData) / sizeof(BYTE)) {
				data[i] = nData[i];
				if (++i > MAX_MEM) {
					return;
				}
			}
		}
	private:
		BYTE data[MAX_MEM];	// memory data (64 KiB)
		uint32_t *cycles;	// pointer to cycle count
};

// computer central processing unit struct
struct CPU {
	public:
		static constexpr bool READ = true;
		static constexpr bool WRITE = false;

		static constexpr BYTE ins_lda_im = 0xA9;	// immediate LDA instruction (2 cycles, 2 bytes. Affects zero and negative flags)
		static constexpr BYTE ins_lda_zp = 0xA5;	// zero-page LDA instruction (3 cycles, 2 bytes. Affects zero and negative flags)
		static constexpr BYTE ins_lda_zpx = 0xB5;	// zero-page X LDA instruction (4 cycles, 2 bytes. Affects zero and negative flags)

		// sends a reset signal to reset computer state (7 cycles)
		void reset(uint32_t &cycles, MEMORY &mem) {
			reg_programCounter = 0x0000;
			reg_stackPointer = 0x00;
			cycles--;
			cycles--;
			fl_carry = fl_zero = fl_interr = fl_dec = fl_break = fl_oflow = fl_neg = false;
			reg_acc = reg_x = reg_y = 0;
			// sets stack pointer register to 0xFD
			rwStack(mem, READ);
			rwStack(mem, READ);
			rwStack(mem, READ);
			// stores contents of 0xFFFD in the low byte of the program counter register and decrements stack pointer
			reg_programCounter = rwPage(mem, reg_stackPointer, 0xFF, READ, 0x00);
			reg_stackPointer--;
			// stores contents of 0xFFFC in the high byte of the program counter register and decrements stack pointer
			reg_programCounter += (WORD)(rwPage(mem, reg_stackPointer, 0xFF, READ, 0x00)) >> 8;
			reg_stackPointer--;
		}

		// executes instructions at programCounter while cycles is greater than 0
		void execute(uint32_t cycles, MEMORY &mem) {
			while (cycles > 0) {
				BYTE instruction = fetchNext(cycles, mem);
				switch (instruction) {
					case ins_lda_im:
						{
							reg_acc = fetchNext(cycles, mem);
							fl_zero = (reg_acc == 0);
							fl_neg = (reg_acc & 0b01000000) > 0;
						}
						break;
					case ins_lda_zp:
						{
							reg_acc = rwZeroPage(mem, fetchNext(cycles, mem), READ);
							fl_zero = (reg_acc == 0);
							fl_neg = (reg_acc & 0b01000000) > 0;
						}
						break;
					case ins_lda_zpx:
						{
							reg_acc = rwZeroPage(mem, fetchNext(cycles, mem), READ) + reg_x;
							fl_zero = (reg_acc == 0);
							fl_neg = (reg_acc & 0b01000000) > 0;
						}
				}
			}
		}
	private:
		WORD reg_programCounter;	// 16-bit program counter register
		BYTE reg_stackPointer;		// 8-bit stack pointer register
		BYTE reg_acc;				// 8-bit accumulator register
		BYTE reg_x;					// 8-bit x register
		BYTE reg_y;					// 8-bit y register
		BYTE fl_carry:1;			// 1-bit carry flag
		BYTE fl_zero:1;				// 1-bit carry flag
		BYTE fl_interr:1;			// 1-bit zero flag
		BYTE fl_dec:1;				// 1-bit decimal flag
		BYTE fl_break:1;			// 1-bit break flag
		BYTE fl_oflow:1;			// 1-bit overflow flag
		BYTE fl_neg:1;				// 1-bit negative flag

		// reads/writes and returns byte at address in given page (1 cycle)
		BYTE rwPage(MEMORY &mem, BYTE address, BYTE page, bool rw, BYTE data) {
			BYTE value;
			if (rw == READ) {
				value = mem[address | ((WORD)(page) >> 8)];
			} else {
				mem[address | ((WORD)(page) >> 8)] = data;
				value = data;
			}
			return value;
		}

		// reads and returns next byte at programCounter. Increments programCounter (1 cycle)
		BYTE fetchNext(uint32_t &cycles, MEMORY &mem) {
			BYTE data = mem[reg_programCounter];
			reg_programCounter++;
			cycles--;
			return data;
		}

		// reads/writes and returns byte at address in zero-page (range 0x0000 to 0x00FF) (1 cycle)
		BYTE rwZeroPage(MEMORY &mem, BYTE address, bool rw, BYTE data = 0x00) {
			return rwPage(mem, address, 0x00, rw, data);
		}

		// reads/writes and returns byte at stack pointer in page one (range 0x0100 to 0x01FF) (1 cycle)
		BYTE rwStack(MEMORY &mem, bool rw, BYTE data = 0x00) {
			BYTE value = rwPage(mem, reg_stackPointer, 0x01, rw, data);
			reg_stackPointer--;
			return value;
		}
};

int main() {
	uint32_t cycles = 20;
	MEMORY mem;
	CPU cpu;
	mem.init(&cycles);
	cpu.reset(cycles, mem);
	return 0;
}