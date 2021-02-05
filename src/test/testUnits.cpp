#include <cassert>

#include "../6502.h"

std::vector<m6502::BYTE> constructProgram(std::vector<m6502::BYTE> program, std::vector<m6502::BYTE> zp) {
	std::vector<m6502::BYTE> data;
	for (m6502::WORD i = 0; i < 0xFFFF; i++) {
		data.push_back(0xEA);
	}
	data.push_back(0xEA);
	data[0xFFFC] = 0x00;
	data[0xFFFD] = 0x20;
	data[0xFFFE] = 0x00;
	data[0xFFFF] = 0xFF;
	for (m6502::BYTE i = 0x00; i < zp.size(); i++) {
		data[i] = zp[i];
		if (i = 0xFF) {
			break;
		}
	}
	for (m6502::WORD i = 0x2000; i < program.size() + 0x2000; i++) {
		if (i == 0xFF00) {
			break;
		}
		data[i] = program[i - 0x2000];
	}
	return data;
}

// base class for test units
class testUnit {
	public:
		testUnit() {
			mem.init(&cycles);
			cpu.reset(cycles, mem);
			cycles = 0x0001FFFF;
		}

		// start the test unit
		virtual void test() {
			std::cout << "base test unit started" << std::endl;
			std::cout << "base test unit completed" << std::endl;
		}
	protected:
		m6502::MEMORY mem;
		m6502::CPU cpu;
		uint32_t cycles;
}; // class testUnit

// test unit for memory initialization
class A : public testUnit {
	public:
		void test() {
			std::cout << "test unit A started" << std::endl;
			for (int i = 0; i < 0xFFFF; i++) {
				assert(mem[i] == 0);
			}
			std::cout << "test unit A : first assert passed" << std::endl;
			std::cout << "test unit A completed" << std::endl;
		}
}; // class A : public testUnit

// test unit for memory filling
class B : public testUnit {
	public:
		void test() {
			std::cout << "test unit B started" << std::endl;
			std::vector<m6502::BYTE> data = {0xaa, 0xbb, 0xcc, 0x42};
			mem.fill(data);
			for (m6502::WORD i = 0; i < data.size(); i++) {
				assert(mem[i] == data[i]);
			}
			std::cout << "test unit B : first assert passed" << std::endl;
			std::cout << "test unit B completed" << std::endl;
		}
}; // class B : public testUnit

// test unit for m6502::MEMORY::operator[]
class C : public testUnit {
	public:
		void test() {
			std::cout << "test unit C started" << std::endl;
			m6502::BYTE data;
			for (m6502::WORD i = 0; i < 0x7FFF; i++) {
				data = mem[i];
			}
			for (m6502::WORD i = 0; i < 0x7000; i++) {
				mem[i] = 42;
			}
			assert(cycles == 0x00011000);
			std::cout << "test unit C : first assert passed" << std::endl;
			for (m6502::WORD i = 0; i < 0x7000; i++) {
				assert(mem[i] == 42);
			}
			std::cout << "test unit C : second asert passed" << std::endl;
			std::cout << "test unit C completed" << std::endl;
		}
}; // class C : public testUnit

// test unit for littleEndianWord
class D : public testUnit {
	public:
		void test() {
			std::cout << "test D started" << std::endl;
			m6502::BYTE lowByte = 0xAD;
			m6502::BYTE highByte = 0xDE;
			assert(cpu.littleEndianWord(lowByte, highByte) == 0xDEAD);
			std::cout << "test unit D : first assert passed" << std::endl;
			std::cout << "test D completed" << std::endl;
		}
}; // class D : public testUnit

// test unit for CPU reset
class E : public testUnit {
	public:
		void test() {
			std::cout << "test E started" << std::endl;
			mem[0xFFFC] = 0xAD;
			mem[0xFFFD] = 0xDE;
			cpu.reset(cycles, mem);
			assert(cycles == 0x0001fff6);
			std::cout << "test E : first assert passed" << std::endl;
			assert(cpu.reg_stackPointer == 0xFD);
			std::cout << "test E : second assert passed" << std::endl;
			assert(cpu.reg_programCounter == 0xDEAD);
			std::cout << "test E : third assert passed" << std::endl;
			std::cout << "test E completed" << std::endl;
		}
}; // class E : public testUnit

// test unit for zero-page x, zero-page y, absolute x and y addressing
class F : public testUnit {
	public:
		void test() {
			std::cout << "test F started" << std::endl;
			cpu.reg_programCounter = 0x2000;
			cpu.reg_x = 0x33;
			cpu.reg_y = 0xF0;
			assert(cpu.zeroPageXAddressing(cycles, 0xE0) == 0x0013);
			std::cout << "test F : first assert passed" << std::endl;
			assert(cpu.absoluteXAddressing(mem, 0xF044) == 0xF077);
			std::cout << "test F : second assert passed" << std::endl;
			assert(cpu.absoluteYAddressing(mem, 0x4321) == 0x4411);
			std::cout << "test F : third assert passed" << std::endl;
			assert(cycles == 0x0001FFFD);
			std::cout << "test F completed" << std::endl;
		}
}; // class F : public testUnit

// test unit for indirect x and indirect y addressing
class G : public testUnit {
	public:
		void test() {
			std::cout << "test G started" << std::endl;
			cpu.reg_programCounter = 0x2000;
			cpu.reg_x = 0x21;
			cpu.reg_y = 0xF0;
			mem[0x42] = 0xAD;
			mem[0x43] = 0xDE;
			assert(cpu.indirectXAddressing(cycles, mem, 0x21) == 0xDEAD);
			std::cout << "test G : first assert passed" << std::endl;
			mem[0xAA] = 0xAB;
			mem[0xAB] = 0xCD;
			assert(cpu.indirectYAddressing(mem, 0xAA) == 0xCE9B);
			std::cout << "test G : second assert passed" << std::endl;
			assert(cycles == 0x0001FFEF);
			std::cout << "test G : third assert passed" << std::endl;
			std::cout << "test G completed" << std::endl;
		}
}; // class G : public testUnit


class H : public testUnit {
	public:
		void test() {
			std::cout << "test H started" << std::endl;
			std::cout << "test H completed" << std::endl;
		}
}; // class H : public testUnit

int main() {
	A a;
	B b;
	C c;
	D d;
	E e;
	F f;
	a.test();
	b.test();
	c.test();
	d.test();
	e.test();
	f.test();
	return 0;
}