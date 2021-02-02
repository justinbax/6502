#include <cassert>

#include "../6502.h"

namespace m6502 {

} // namespace m6502

// base class for test units
class testUnit {
	public:
		testUnit() {
			mem.init(&cycles);
			cpu.reset(cycles, mem);
			cycles = 0x0001ffff;
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
			for (int i = 0; i < 0xffff; i++) {
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
			cycles = 0x0001ffff;
			std::cout << "test unit C started" << std::endl;
			m6502::BYTE data;
			for (m6502::WORD i = 0; i < 0x7fff; i++) {
				data = mem[i];
			}
			for (m6502::WORD i = 0; i < 0x7000; i++) {
				mem[i] = 42;
			}
			assert(cycles == 0x00011000);
			std::cout << "test unit C : first assert passed" << std::endl;
			for (m6502::WORD i = 0; i < 0x6000; i++) {
				assert(mem[i] == 42);
			}
			std::cout << "test unit C : second asert passed" << std::endl;
			std::cout << "test unit C completed" << std::endl;
		}
}; // class C : public testUnit

class D : public testUnit {
	public:
		void test() {
			std::cout << "test D started" << std::endl;
			mem[0xFFFC] = 0xAD;
			mem[0xFFFD] = 0xDE;
			cpu.reset(cycles, mem);
			assert(cycles == 0x0001ffff);
			std::cout << "test D : first assert passed" << std::endl;
			assert(cpu.reg_stackPointer == 0xFD);
			std::cout << "test D : second assert passed" << std::endl;
			assert(cpu.reg_programCounter == 0xDEAD);
			std::cout << "test D : third assert passed" << std::endl;
			std::cout << "test D completed" << std::endl;
		}
}; // class D : public testUnit

// test unit for CPU reset
class E : public testUnit {
	public:
		void test() {
			std::cout << "test unit E started" << std::endl;

			std::cout << "test unit E completed" << std::endl;
		}
}; // class E : public testUnit

int main() {
	A a;
	B b;
	C c;
	a.test();
	b.test();
	c.test();
	return 0;
}