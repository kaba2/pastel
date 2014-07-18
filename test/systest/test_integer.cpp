// Description: Testing for Integer
// DocumentationOf: integer.h

#include "test_pastelsys.h"

#include "pastel/sys/integer.h"

using namespace Pastel;

namespace
{

	class Test
		: public TestSuite
	{
	public:
		Test()
			: TestSuite(&testReport())
		{
		}

		virtual void run()
		{
			testConstruction();
			testNegation();
			testIncrementDecrement();
			testComparison();
			testClearBits();
			testArithmetic();
			testBitOperators();
			testCounting();
			testSetBits();
			testMultiplication();
			testNumberOfOneBits();
		}

		void testConstruction()
		{
			using F = Unsigned_Integer<20, uint8>;

			//std::cout << F(0x12345678) << std::endl;
			
			TEST_ENSURE(F(0x12345678) == F({ 0x12, 0x34, 0x56, 0x78 }));
			TEST_ENSURE(F(0x12345678) == F({ 0x34, 0x56, 0x78 }));
			TEST_ENSURE(F(0x12345678) == F({ 0x04, 0x56, 0x78 }));

			{
				TEST_ENSURE(F((int8)-1) == F({0xFF, 0xFF, 0xFF}));
				TEST_ENSURE(F((int8)-2) == F({0xFF, 0xFF, 0xFE}));
				TEST_ENSURE(F((int8)-16) == F({0xFF, 0xFF, 0xF0}));
			}
			{
				F a({ 0x04, 0x56, 0x78 });
				TEST_ENSURE_OP(a.word(0), == , 0x78);
				TEST_ENSURE_OP(a.word(1), == , 0x56);
				TEST_ENSURE_OP(a.word(2), == , 0x04);
			}

			{
				F a(0x12345678);
				TEST_ENSURE_OP(a.word(0), == , 0x78);
				TEST_ENSURE_OP(a.word(1), == , 0x56);
				TEST_ENSURE_OP(a.word(2), == , 0x04);
			}

			TEST_ENSURE(F(0) == F());	

			{
				F a(-1);
				TEST_ENSURE_OP(a.word(0), == , 0xFF);
				TEST_ENSURE_OP(a.word(1), == , 0xFF);
				TEST_ENSURE_OP(a.word(2), == , 0x0F);
			}

			{
				F a(-16);
				TEST_ENSURE_OP(a.word(0), == , 0xF0);
				TEST_ENSURE_OP(a.word(1), == , 0xFF);
				TEST_ENSURE_OP(a.word(2), == , 0x0F);
			}

			{
				using F = Unsigned_Integer<20, uint8>;
				F a(0x12345678);

				TEST_ENSURE(F(a, 0, 0) == F(0x00000000));
				TEST_ENSURE(F(a, 0, 1) == F(0x00000000));
				TEST_ENSURE(F(a, 0, 2) == F(0x00000000));
				TEST_ENSURE(F(a, 0, 3) == F(0x00000000));
				TEST_ENSURE(F(a, 0, 4) == F(0x00000008));
				TEST_ENSURE(F(a, 0, 5) == F(0x00000018));
				TEST_ENSURE(F(a, 0, 6) == F(0x00000038));
				TEST_ENSURE(F(a, 0, 7) == F(0x00000078));

				TEST_ENSURE(F(a, 0, 8) == F(0x00000078));
				TEST_ENSURE(F(a, 0, 9) == F(0x00000078));
				TEST_ENSURE(F(a, 0, 10) == F(0x00000278));
				TEST_ENSURE(F(a, 0, 11) == F(0x00000678));
				TEST_ENSURE(F(a, 0, 12) == F(0x00000678));
				TEST_ENSURE(F(a, 0, 13) == F(0x00001678));
				TEST_ENSURE(F(a, 0, 14) == F(0x00001678));
				TEST_ENSURE(F(a, 0, 15) == F(0x00005678));

				TEST_ENSURE(F(a, 0, 16) == F(0x00005678));
				TEST_ENSURE(F(a, 1, 16) == F(0x00005678));
				TEST_ENSURE(F(a, 2, 16) == F(0x00005678));
				TEST_ENSURE(F(a, 3, 16) == F(0x00005678));
				TEST_ENSURE(F(a, 4, 16) == F(0x00005670));
				TEST_ENSURE(F(a, 5, 16) == F(0x00005660));
				TEST_ENSURE(F(a, 6, 16) == F(0x00005640));
				TEST_ENSURE(F(a, 7, 16) == F(0x00005600));

				TEST_ENSURE(F(a, 8, 16) == F(0x00005600));
				TEST_ENSURE(F(a, 9, 16) == F(0x00005600));
				TEST_ENSURE(F(a, 10, 16) == F(0x00005400));
				TEST_ENSURE(F(a, 11, 16) == F(0x00005000));
				TEST_ENSURE(F(a, 12, 16) == F(0x00005000));
				TEST_ENSURE(F(a, 13, 16) == F(0x00004000));
				TEST_ENSURE(F(a, 14, 16) == F(0x00004000));
				TEST_ENSURE(F(a, 15, 16) == F(0x00000000));

				TEST_ENSURE(F(a, 16, 16) == F(0x00000000));
			}
		}

		void testNegation()
		{
			{
				using F = Signed_Integer<16, uint8>;

				for (integer i = -512; i <= 512; ++i)
				{
					TEST_ENSURE(-F(i) == F(-i));
					TEST_ENSURE(F(i) + (-F(i)) == 0);
				} 
			}

			{
				using F = Unsigned_Integer<16, uint8>;

				for (integer i = -512; i < 512; ++i)
				{
					TEST_ENSURE(-F(i) == F(-i));
					TEST_ENSURE(F(i) + (-F(i)) == 0);
				}
			}
		}

		void testIncrementDecrement()
		{
			using F = Signed_Integer<20, uint8>;

			for (integer i = -512; i <= 512; ++i)
			{
				TEST_ENSURE(--F(i) == i - 1);
				TEST_ENSURE(++F(i) == i + 1);
			}

			TEST_ENSURE(++F(0x7FFFF) == -0x80000);
			TEST_ENSURE(--F(0x80000) == +0x7FFFF);
			TEST_ENSURE(--F(0x10000) == +0x0FFFF);
			TEST_ENSURE(++F(0x0FFFF) == +0x10000);
		}

		void testComparison()
		{
			using F = Signed_Integer<20, uint8>;
			
			TEST_ENSURE(negative(F(-0x00001)));
			TEST_ENSURE(negative(F(-0x23456)));
			TEST_ENSURE(negative(F(+0x83456u)));
			TEST_ENSURE(negative(F(+0x80000u)));
			TEST_ENSURE(positive(F(+0x00001)));
			TEST_ENSURE(positive(F(+0x23456)));
			TEST_ENSURE(positive(F(+0x7FFFF)));
			TEST_ENSURE(positive(F(+0x7FFFFu)));
			TEST_ENSURE(zero(F(0x000000)));
			TEST_ENSURE(zero(F(0x000000u)));

			TEST_ENSURE(F(-0x7FFFF) < F(0x12354));
			TEST_ENSURE(F(-0x20000) < F(0x10000));
			TEST_ENSURE(F(-0x10000) < F(0x00001));
			TEST_ENSURE(F(0x00000) < F(0x00001));
			TEST_ENSURE(F(0x00000) < F(0x04323));
			TEST_ENSURE(F(0x04323) < F(0x10000));
			TEST_ENSURE(F(0x100000) < F(0x20000));
			TEST_ENSURE(F(0x23456) < F(0x34567));
		}

		void testClearBits()
		{
				using F = Unsigned_Integer<16, uint8>;
				auto f = [&](integer begin, integer end) -> F
				{
					return F(0xFFFF).clearBits(begin, end);
				};

				// Clear the front for multiple words.

				TEST_ENSURE(f(0, 0) == F(0xFFFF));
				TEST_ENSURE(f(0, 1) == F(0xFFFE));
				TEST_ENSURE(f(0, 2) == F(0xFFFC));
				TEST_ENSURE(f(0, 3) == F(0xFFF8));
				TEST_ENSURE(f(0, 4) == F(0xFFF0));
				TEST_ENSURE(f(0, 5) == F(0xFFE0));
				TEST_ENSURE(f(0, 6) == F(0xFFC0));
				TEST_ENSURE(f(0, 7) == F(0xFF80));
				TEST_ENSURE(f(0, 8) == F(0xFF00));
				TEST_ENSURE(f(0, 9) == F(0xFE00));
				TEST_ENSURE(f(0, 10) == F(0xFC00));
				TEST_ENSURE(f(0, 11) == F(0xF800));
				TEST_ENSURE(f(0, 12) == F(0xF000));
				TEST_ENSURE(f(0, 13) == F(0xE000));
				TEST_ENSURE(f(0, 14) == F(0xC000));
				TEST_ENSURE(f(0, 15) == F(0x8000));
				TEST_ENSURE(f(0, 16) == F(0x0000));

				// Clear the tail for multiple words.

				TEST_ENSURE(f(16, 16) == F(0xFFFF));
				TEST_ENSURE(f(15, 16) == F(0x7FFF));
				TEST_ENSURE(f(14, 16) == F(0x3FFF));
				TEST_ENSURE(f(13, 16) == F(0x1FFF));
				TEST_ENSURE(f(12, 16) == F(0x0FFF));
				TEST_ENSURE(f(11, 16) == F(0x07FF));
				TEST_ENSURE(f(10, 16) == F(0x03FF));
				TEST_ENSURE(f(9, 16) == F(0x01FF));
				TEST_ENSURE(f(8, 16) == F(0x00FF));
				TEST_ENSURE(f(7, 16) == F(0x007F));
				TEST_ENSURE(f(6, 16) == F(0x003F));
				TEST_ENSURE(f(5, 16) == F(0x001F));
				TEST_ENSURE(f(4, 16) == F(0x000F));
				TEST_ENSURE(f(3, 16) == F(0x0007));
				TEST_ENSURE(f(2, 16) == F(0x0003));
				TEST_ENSURE(f(1, 16) == F(0x0001));
				TEST_ENSURE(f(0, 16) == F(0x0000));

				// Clear the front inside a single word.

				TEST_ENSURE(f(0, 0) == F(0xFFFF));
				TEST_ENSURE(f(0, 1) == F(0xFFFE));
				TEST_ENSURE(f(0, 2) == F(0xFFFC));
				TEST_ENSURE(f(0, 3) == F(0xFFF8));
				TEST_ENSURE(f(0, 4) == F(0xFFF0));
				TEST_ENSURE(f(0, 5) == F(0xFFE0));
				TEST_ENSURE(f(0, 6) == F(0xFFC0));
				TEST_ENSURE(f(0, 7) == F(0xFF80));
				TEST_ENSURE(f(0, 8) == F(0xFF00));

				// Clear the tail inside a single word.

				TEST_ENSURE(f(0, 8) == F(0xFF00));
				TEST_ENSURE(f(1, 8) == F(0xFF01));
				TEST_ENSURE(f(2, 8) == F(0xFF03));
				TEST_ENSURE(f(3, 8) == F(0xFF07));
				TEST_ENSURE(f(4, 8) == F(0xFF0F));
				TEST_ENSURE(f(5, 8) == F(0xFF1F));
				TEST_ENSURE(f(6, 8) == F(0xFF3F));
				TEST_ENSURE(f(7, 8) == F(0xFF7F));
				TEST_ENSURE(f(8, 8) == F(0xFFFF));

				// Clear zero bits.

				TEST_ENSURE(f(0, 0) == F(0xFFFF));
				TEST_ENSURE(f(1, 1) == F(0xFFFF));
				TEST_ENSURE(f(2, 2) == F(0xFFFF));
				TEST_ENSURE(f(3, 3) == F(0xFFFF));
				TEST_ENSURE(f(4, 4) == F(0xFFFF));
				TEST_ENSURE(f(5, 5) == F(0xFFFF));
				TEST_ENSURE(f(6, 6) == F(0xFFFF));
				TEST_ENSURE(f(7, 7) == F(0xFFFF));
				TEST_ENSURE(f(8, 8) == F(0xFFFF));
				TEST_ENSURE(f(9, 9) == F(0xFFFF));
				TEST_ENSURE(f(10, 10) == F(0xFFFF));
				TEST_ENSURE(f(11, 11) == F(0xFFFF));
				TEST_ENSURE(f(12, 12) == F(0xFFFF));
				TEST_ENSURE(f(13, 13) == F(0xFFFF));
				TEST_ENSURE(f(14, 14) == F(0xFFFF));
				TEST_ENSURE(f(15, 15) == F(0xFFFF));
				TEST_ENSURE(f(16, 16) == F(0xFFFF));

				// Clear one bit.

				TEST_ENSURE(f(0, 1) == F(0xFFFE));
				TEST_ENSURE(f(1, 2) == F(0xFFFD));
				TEST_ENSURE(f(2, 3) == F(0xFFFB));
				TEST_ENSURE(f(3, 4) == F(0xFFF7));
				TEST_ENSURE(f(4, 5) == F(0xFFEF));
				TEST_ENSURE(f(5, 6) == F(0xFFDF));
				TEST_ENSURE(f(6, 7) == F(0xFFBF));
				TEST_ENSURE(f(7, 8) == F(0xFF7F));
				TEST_ENSURE(f(8, 9) == F(0xFEFF));
				TEST_ENSURE(f(9, 10) == F(0xFDFF));
				TEST_ENSURE(f(10, 11) == F(0xFBFF));
				TEST_ENSURE(f(11, 12) == F(0xF7FF));
				TEST_ENSURE(f(12, 13) == F(0xEFFF));
				TEST_ENSURE(f(13, 14) == F(0xDFFF));
				TEST_ENSURE(f(14, 15) == F(0xBFFF));
				TEST_ENSURE(f(15, 16) == F(0x7FFF));

				// Clear two bits.

				TEST_ENSURE(f(0, 2) == F(0xFFFC));
				TEST_ENSURE(f(1, 3) == F(0xFFF9));
				TEST_ENSURE(f(2, 4) == F(0xFFF3));
				TEST_ENSURE(f(3, 5) == F(0xFFE7));
				TEST_ENSURE(f(4, 6) == F(0xFFCF));
				TEST_ENSURE(f(5, 7) == F(0xFF9F));
				TEST_ENSURE(f(6, 8) == F(0xFF3F));
				TEST_ENSURE(f(7, 9) == F(0xFE7F));
				TEST_ENSURE(f(8, 10) == F(0xFCFF));
				TEST_ENSURE(f(9, 11) == F(0xF9FF));
				TEST_ENSURE(f(10, 12) == F(0xF3FF));
				TEST_ENSURE(f(11, 13) == F(0xE7FF));
				TEST_ENSURE(f(12, 14) == F(0xCFFF));
				TEST_ENSURE(f(13, 15) == F(0x9FFF));
				TEST_ENSURE(f(14, 16) == F(0x3FFF));
		}

		void testArithmetic()
		{
			{
				using F = Unsigned_Integer<1, uint8>;

				TEST_ENSURE(F(0) + F(0) == F(0));
				TEST_ENSURE(F(0) + F(1) == F(1));
				TEST_ENSURE(F(1) + F(0) == F(1));
				TEST_ENSURE(F(1) + F(1) == F(0));

				TEST_ENSURE(F(0) - F(0) == F(0));
				TEST_ENSURE(F(0) - F(1) == F(-1));
				TEST_ENSURE(F(1) - F(0) == F(1));
				TEST_ENSURE(F(1) - F(1) == F(0));
			}

			{
				using F = Unsigned_Integer<2, uint8>;

				TEST_ENSURE(F(0) + F(0) == F(0));
				TEST_ENSURE(F(0) + F(1) == F(1));
				TEST_ENSURE(F(0) + F(2) == F(2));
				TEST_ENSURE(F(0) + F(3) == F(3));
				TEST_ENSURE(F(1) + F(0) == F(1));
				TEST_ENSURE(F(1) + F(1) == F(2));
				TEST_ENSURE(F(1) + F(2) == F(3));
				TEST_ENSURE(F(1) + F(3) == F(0));
				TEST_ENSURE(F(2) + F(0) == F(2));
				TEST_ENSURE(F(2) + F(1) == F(3));
				TEST_ENSURE(F(2) + F(2) == F(0));
				TEST_ENSURE(F(2) + F(3) == F(1));
				TEST_ENSURE(F(3) + F(0) == F(3));
				TEST_ENSURE(F(3) + F(1) == F(0));
				TEST_ENSURE(F(3) + F(2) == F(1));
				TEST_ENSURE(F(3) + F(3) == F(2));

				TEST_ENSURE(F(0) - F(0) == F(0));
				TEST_ENSURE(F(0) - F(1) == F(-1));
				TEST_ENSURE(F(0) - F(2) == F(-2));
				TEST_ENSURE(F(0) - F(3) == F(-3));
				TEST_ENSURE(F(1) - F(0) == F(1));
				TEST_ENSURE(F(1) - F(1) == F(0));
				TEST_ENSURE(F(1) - F(2) == F(-1));
				TEST_ENSURE(F(1) - F(3) == F(-2));
				TEST_ENSURE(F(2) - F(0) == F(2));
				TEST_ENSURE(F(2) - F(1) == F(1));
				TEST_ENSURE(F(2) - F(2) == F(0));
				TEST_ENSURE(F(2) - F(3) == F(-1));
				TEST_ENSURE(F(3) - F(0) == F(3));
				TEST_ENSURE(F(3) - F(1) == F(2));
				TEST_ENSURE(F(3) - F(2) == F(1));
				TEST_ENSURE(F(3) - F(3) == F(0));
			}

			{
				using F = Unsigned_Integer<20, uint8>;

				auto f = [](integer a, integer b) -> bool
				{
					return (F(a) + F(b)) == (a + b);
				};
				
				TEST_ENSURE(f(0x13413, 0x32432));
				TEST_ENSURE(f(0x35486, 0x72234));
				TEST_ENSURE(f(0x91847, 0xF4824));
			}

			{
				using F = Unsigned_Integer<20, uint8>;

				auto f = [](integer a, integer b) -> bool
				{
					return (F(a) - F(b)) == (a - b);
				};

				TEST_ENSURE(f(0x13413, 0x32432));
				TEST_ENSURE(f(0x35486, 0x72234));
				TEST_ENSURE(f(0x91847, 0xF4824));
			}

			{
				using F = Unsigned_Integer<32, uint8>;

				uint32 a = 0x12345678;
				for (integer i = 0;i < 32;++i)
				{
					// Interesting fact:
					// if i ranges up to 33, then the following
					// tests fail for i == 32. This probably has 
					// to do with the shifts (a << i) and (a >> i) 
					// being implementation-defined in this case.

					TEST_ENSURE(
						(F(a) << i) == F(a << i));

					TEST_ENSURE(
						(F(a) >> i) == F(a >> i));
				}

				// We define the out-of-range shift anyway.
				TEST_ENSURE((F(a) << 33) == F(0));
				TEST_ENSURE((F(a) >> 33) == F(0));
			}
		}

		void testBitOperators()
		{
			using F = Unsigned_Integer<20, uint8>;
			
			TEST_ENSURE((F(0x12345678u) ^ F(0x45635603u)) == (0x12345678u ^ 0x45635603u));
			TEST_ENSURE((F(0x12345678u) | F(0x45635603u)) == (0x12345678u | 0x45635603u));
			TEST_ENSURE((F(0x12345678u) & F(0x45635603u)) == (0x12345678u & 0x45635603u));
			TEST_ENSURE((~F(0x12345678u)) == (~0x12345678u));
		}

		void testCounting()
		{
			{
				using F = Unsigned_Integer<1, uint8>;

				TEST_ENSURE_OP(F(0).oneBits(), == , 0);
				TEST_ENSURE_OP(F(1).oneBits(), == , 1);
			}

			{
				using F = Unsigned_Integer<32, uint8>;

				TEST_ENSURE_OP(F(0x12345678).oneBits(), == ,
					1 + 1 + 2 + 1 + 2 + 2 + 3 + 1);

				TEST_ENSURE_OP(F(0x9ABCDEF0).oneBits(), == ,
					2 + 2 + 3 + 2 + 3 + 3 + 4 + 0);

				TEST_ENSURE(F(0x9ABCDEF0).to_string() ==
					"10011010101111001101111011110000");
			}
		}

		void testSetBits()
		{
			using F = Unsigned_Integer<20, uint8>;

			{
				TEST_ENSURE(!F(0).bit(0));
				TEST_ENSURE(F(1).bit(0));

				TEST_ENSURE(F(0).setBit(0).bit(0));
				TEST_ENSURE(F(0).setBit(0, true).bit(0));
				TEST_ENSURE(!F(1).setBit(0, false).bit(0));
				TEST_ENSURE(!F(1).clearBit(0).bit(0));
			}

			F a;

			TEST_ENSURE(!a.bit(20));
			TEST_ENSURE(!a.bit(21));
			TEST_ENSURE(!a.bit(22));
			TEST_ENSURE(!a.bit(32));
			TEST_ENSURE(!a.bit(64));
			TEST_ENSURE(!a.bit(128));

			a.setBit(0);
			TEST_ENSURE(a == (1 << 0));
			
			a.setBit(1);
			TEST_ENSURE(a == (1 << 0) + (1 << 1));

			a.setBit(2);
			TEST_ENSURE(a == (1 << 0) + (1 << 1) + (1 << 2));

			a.setBit(7);
			TEST_ENSURE(a == (1 << 0) + (1 << 1) + (1 << 2) + (1 << 7));

			a.setBit(12);
			TEST_ENSURE(a == (1 << 0) + (1 << 1) + (1 << 2) + (1 << 7) + (1 << 12));

			a.clearBit(2);
			TEST_ENSURE(a == (1 << 0) + (1 << 1) + (1 << 7) + (1 << 12));

			a.clearBit(7);
			TEST_ENSURE(a == (1 << 0) + (1 << 1) + (1 << 12));

			a.clearBit(12);
			TEST_ENSURE(a == (1 << 0) + (1 << 1));

			a.clearBit(1);
			TEST_ENSURE(a == (1 << 0));

			a.clearBit(0);
			TEST_ENSURE(a == 0);

			a.setBit(19);
			TEST_ENSURE(a == (1 << 19));

			a.clearBit(19);
			TEST_ENSURE(a == 0);

			a = 0x12345678;
			a.clearBits();
			TEST_ENSURE(a == 0);
			
			a = 0x12345678;
			a.setBits();
			TEST_ENSURE(a == 0x000FFFFF);

			{
				Signed_Integer<20, uint8> b(-1);
				TEST_ENSURE(b.bit(20));
				TEST_ENSURE(b.bit(21));
				TEST_ENSURE(b.bit(22));
				TEST_ENSURE(b.bit(32));
				TEST_ENSURE(b.bit(64));
				TEST_ENSURE(b.bit(128));
			}
			{
				Signed_Integer<20, uint8> b(0);
				TEST_ENSURE(!b.bit(20));
				TEST_ENSURE(!b.bit(21));
				TEST_ENSURE(!b.bit(22));
				TEST_ENSURE(!b.bit(32));
				TEST_ENSURE(!b.bit(64));
				TEST_ENSURE(!b.bit(128));
			}
		}

		void testMultiplication()
		{
			{
				using F = Unsigned_Integer<20, uint8>;
				for (integer i = 0; i < 256; ++i)
				{
					for (integer j = 0; j < 256; ++j)
					{
						TEST_ENSURE(F(i) * F(j) == F(i * j));
					}
				}
			}
			{
				using F = Signed_Integer<20, uint8>;
				for (integer i = -127; i <= 128; ++i)
				{
					for (integer j = -127; j <= 128; ++j)
					{
						TEST_ENSURE(F(i) * F(j) == F(i * j));
					}
				}
			}
		}

		void testNumberOfOneBits()
		{
			using F = Unsigned_Integer<20, uint8>;

			auto test = [&](F that, integer correct)
			{
				return numberOfOneBits(that) == correct;
			};

			TEST_ENSURE(test(0xFFF34567ul, 10));
			TEST_ENSURE(test(0x00034567ul, 10));
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("Integer", test);
	}

	CallFunction run(addTest);

}
