// Description: Testing for MultiInteger
// DocumentationOf: multi_integer.h

#include "test/test_init.h"

#include "pastel/sys/integer/multi_integer.h"

#include <iomanip>

using namespace Pastel;

namespace
{

	class Test
	{
	public:
		virtual void run()
		{
			testInfinity();
			testAsString();
			testAsNative();
			testConstruction();
			testNegation();
			testIncrementDecrement();
			testComparison();
			testClearBits();
			testAddition();
			testArithmetic();
			testBitOperators();
			testCounting();
			testSetBits();
			testMultiplication();
			testDivision();
			testModulo();
			testNumberOfOneBits();
		}

		void testInfinity()
		{
			{
				using F = Unsigned_Integer<32, uint8>;
				REQUIRE(infinity<F>() == 0xFFFFFFFF);
				REQUIRE(positive(infinity<F>()));
				REQUIRE(zero(++infinity<F>()));
			}
			{
				using F = Signed_Integer<32, uint8>;
				REQUIRE(infinity<F>() == 0x7FFFFFFF);
				REQUIRE(positive(infinity<F>()));
				REQUIRE(negative(++infinity<F>()));
			}
			{
				using F = Unsigned_Integer<8, uint8>;
				REQUIRE(infinity<F>() == 255);
				REQUIRE(positive(infinity<F>()));
				REQUIRE(zero(++infinity<F>()));
			}
			{
				using F = Signed_Integer<8, uint8>;
				REQUIRE(infinity<F>() == 127);
				REQUIRE(positive(infinity<F>()));
				REQUIRE(negative(++infinity<F>()));
			}
			{
				using F = Unsigned_Integer<7, uint8>;
				REQUIRE(infinity<F>() == 127);
				REQUIRE(positive(infinity<F>()));
				REQUIRE(zero(++infinity<F>()));
			}
			{
				using F = Signed_Integer<7, uint8>;
				REQUIRE(infinity<F>() == 63);
				REQUIRE(positive(infinity<F>()));
				REQUIRE(negative(++infinity<F>()));
			}
		}

		void testAsString()
		{
			{
				using F = Unsigned_Integer<32, uint8>;

				REQUIRE(F(0x12345678).asString(16) == "12345678");
				REQUIRE(F(132).asString(10) == "132");
				REQUIRE(F(432874).asString(10) == "432874");

				REQUIRE(F(0x0).asString(16) == "0");
				REQUIRE(F(0xFFFFFFFF).asString(16) == "ffffffff");

				REQUIRE(F(0x9ABCDEF0).asString(2) ==
					"10011010101111001101111011110000");
			}
			{
				using F = Signed_Integer<32, uint8>;

				/*
				std::cout << std::hex << F(-0x123) << std::endl;
				std::cout << std::dec << F(-123) << std::endl;
				std::cout << std::oct << F(-0123) << std::endl;

				std::cout << std::hex << std::showbase << F(-0x123) << std::endl;
				std::cout << std::dec << std::showbase << F(-123) << std::endl;
				std::cout << std::oct << std::showbase << F(-0123) << std::endl;
				*/

				REQUIRE(F(-132).asString(10) == "-132");
				REQUIRE(F(-0x12345678).asString(16) == "-12345678");
				REQUIRE(F(-1).asString(2) == "-1");
				REQUIRE(F(-2).asString(2) == "-10");
				REQUIRE(F(-3).asString(2) == "-11");
				REQUIRE(F(-4).asString(2) == "-100");
			}
			{
				using F = Unsigned_Integer<32, uint8>;
				REQUIRE(F(0x12345678).asString(16) == "12345678");

				REQUIRE(F(0x9ABCDEF0).asString(2) ==
					"10011010101111001101111011110000");
			}
		}

		void testAsNative()
		{
			{
				using F = Unsigned_Integer<32, uint8>;
				REQUIRE(static_cast<uint64>(F(true)) == 1);
			}
			{
				using F = Unsigned_Integer<32, uint8>;
				REQUIRE(static_cast<uint64>(F(false)) == 0);
			}
			{
				using F = Unsigned_Integer<32, uint8>;
				REQUIRE(static_cast<uint64>(F(0x12345678)) == 0x12345678);
			}
			{
				using F = Unsigned_Integer<31, uint8>;
				REQUIRE((uint64)F(0x12345678) == 0x12345678);
			}
			{
				using F = Unsigned_Integer<30, uint8>;
				REQUIRE((uint64)F(0x12345678) == 0x12345678);
			}
			{
				using F = Unsigned_Integer<29, uint8>;
				REQUIRE((uint64)F(0x12345678) == 0x12345678);
			}
			{
				using F = Unsigned_Integer<28, uint8>;
				REQUIRE((uint64)F(0x12345678) == 0x02345678);
			}
			{
				using F = Unsigned_Integer<20, uint8>;
				REQUIRE((uint64)F(0x12345678) == 0x00045678);
			}
			{
				using F = Unsigned_Integer<19, uint8>;
				REQUIRE((uint64)F(0x12345678) == 0x00045678);
			}
			{
				using F = Unsigned_Integer<18, uint8>;
				REQUIRE((uint64)F(0x12345678) == 0x00005678);
			}
			{
				using F = Unsigned_Integer<17, uint8>;
				REQUIRE((uint64)F(0x12345678) == 0x00005678);
			}
			{
				using F = Unsigned_Integer<16, uint8>;
				REQUIRE((uint64)F(0x12345678) == 0x00005678);
			}
			{
				using F = Unsigned_Integer<15, uint8>;
				REQUIRE((uint64)F(0x12345678) == 0x00005678);
			}
			{
				using F = Unsigned_Integer<14, uint8>;
				REQUIRE((uint64)F(0x12345678) == 0x00001678);
			}
			{
				using F = Unsigned_Integer<13, uint8>;
				REQUIRE((uint64)F(0x12345678) == 0x00001678);
			}
			{
				using F = Unsigned_Integer<12, uint8>;
				REQUIRE((uint64)F(0x12345678) == 0x00000678);
			}
			{
				using F = Unsigned_Integer<11, uint8>;
				REQUIRE((uint64)F(0x12345678) == 0x00000678);
			}
			{
				using F = Unsigned_Integer<10, uint8>;
				REQUIRE((uint64)F(0x12345678) == 0x00000278);
			}
			{
				using F = Unsigned_Integer<9, uint8>;
				REQUIRE((uint64)F(0x12345678) == 0x00000078);
			}
			{
				using F = Unsigned_Integer<8, uint8>;
				REQUIRE((uint64)F(0x12345678) == 0x00000078);
			}
			{
				using F = Unsigned_Integer<7, uint8>;
				REQUIRE((uint64)F(0x12345678) == 0x00000078);
			}
			{
				using F = Unsigned_Integer<6, uint8>;
				REQUIRE((uint64)F(0x12345678) == 0x00000038);
			}
			{
				using F = Unsigned_Integer<5, uint8>;
				REQUIRE((uint64)F(0x12345678) == 0x00000018);
			}
			{
				using F = Unsigned_Integer<4, uint8>;
				REQUIRE((uint64)F(0x12345678) == 0x00000008);
			}
			{
				using F = Unsigned_Integer<3, uint8>;
				REQUIRE((uint64)F(0x12345678) == 0x00000000);
			}
			{
				using F = Unsigned_Integer<2, uint8>;
				REQUIRE((uint64)F(0x12345678) == 0x00000000);
			}
			{
				using F = Unsigned_Integer<1, uint8>;
				REQUIRE((uint64)F(0x12345678) == 0x00000000);
			}
			{
				using F = Unsigned_Integer<32, uint8>;
				REQUIRE((uint64)F(0x87654321) == 0x87654321);
			}
			{
				using F = Signed_Integer<32, uint8>;
				REQUIRE(static_cast<int64>(F(-0x12345678)) == -0x12345678);
			}
			{
				using F = Signed_Integer<16, uint8>;
				for (integer i = -40000; i < 40000; i += 13)
				{
					REQUIRE(static_cast<int64>(F(i)) == mod(i + 32768, (integer)65536) - 32768);
				}
			}
			{
				using F = Signed_Integer<15, uint8>;
				for (integer i = -40000; i < 40000; i += 13)
				{
					REQUIRE(static_cast<int64>(F(i)) == mod(i + 16384, (integer)32768) - 16384);
				}
			}
			{
				using F = Signed_Integer<6, uint8>;
				for (integer i = -100; i < 100; ++i)
				{
					REQUIRE(static_cast<int64>(F(i)) == mod(i + 32, (integer)64) - 32);
				}
			}
			{
				using F = Signed_Integer<6, uint16>;
				for (integer i = -100; i < 100; ++i)
				{
					REQUIRE(static_cast<int64>(F(i)) == mod(i + 32, (integer)64) - 32);
				}
			}
		}

		void testConstruction()
		{
			using F = Unsigned_Integer<20, uint8>;
			PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

			REQUIRE(F(0x12345678) == F({ 0x12, 0x34, 0x56, 0x78 }));
			REQUIRE(F(0x12345678) == F({ 0x34, 0x56, 0x78 }));
			REQUIRE(F(0x12345678) == F({ 0x04, 0x56, 0x78 }));

			{
				REQUIRE(F((int8)-1) == F({0xFF, 0xFF, 0xFF}));
				REQUIRE(F((int8)-2) == F({0xFF, 0xFF, 0xFE}));
				REQUIRE(F((int8)-16) == F({0xFF, 0xFF, 0xF0}));
			}
			{
				F a({ 0x04, 0x56, 0x78 });
				REQUIRE(a.word(0) == 0x78);
				REQUIRE(a.word(1) == 0x56);
				REQUIRE(a.word(2) == 0x04);
			}

			{
				F a(0x12345678);
				REQUIRE(a.word(0) == 0x78);
				REQUIRE(a.word(1) == 0x56);
				REQUIRE(a.word(2) == 0x04);
			}

			REQUIRE(F(0) == F());	

			{
				F a(-1);
				REQUIRE(a.word(0) == 0xFF);
				REQUIRE(a.word(1) == 0xFF);
				REQUIRE(a.word(2) == 0x0F);
			}

			{
				F a(-16);
				REQUIRE(a.word(0) == 0xF0);
				REQUIRE(a.word(1) == 0xFF);
				REQUIRE(a.word(2) == 0x0F);
			}

			{
				using F = Unsigned_Integer<20, uint8>;
				PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

				F a(0x12345678);

				REQUIRE(F(a, 0, 0) == F(0x00000000));
				REQUIRE(F(a, 0, 1) == F(0x00000000));
				REQUIRE(F(a, 0, 2) == F(0x00000000));
				REQUIRE(F(a, 0, 3) == F(0x00000000));
				REQUIRE(F(a, 0, 4) == F(0x00000008));
				REQUIRE(F(a, 0, 5) == F(0x00000018));
				REQUIRE(F(a, 0, 6) == F(0x00000038));
				REQUIRE(F(a, 0, 7) == F(0x00000078));

				REQUIRE(F(a, 0, 8) == F(0x00000078));
				REQUIRE(F(a, 0, 9) == F(0x00000078));
				REQUIRE(F(a, 0, 10) == F(0x00000278));
				REQUIRE(F(a, 0, 11) == F(0x00000678));
				REQUIRE(F(a, 0, 12) == F(0x00000678));
				REQUIRE(F(a, 0, 13) == F(0x00001678));
				REQUIRE(F(a, 0, 14) == F(0x00001678));
				REQUIRE(F(a, 0, 15) == F(0x00005678));

				REQUIRE(F(a, 0, 16) == F(0x00005678));
				REQUIRE(F(a, 1, 16) == F(0x00005678));
				REQUIRE(F(a, 2, 16) == F(0x00005678));
				REQUIRE(F(a, 3, 16) == F(0x00005678));
				REQUIRE(F(a, 4, 16) == F(0x00005670));
				REQUIRE(F(a, 5, 16) == F(0x00005660));
				REQUIRE(F(a, 6, 16) == F(0x00005640));
				REQUIRE(F(a, 7, 16) == F(0x00005600));

				REQUIRE(F(a, 8, 16) == F(0x00005600));
				REQUIRE(F(a, 9, 16) == F(0x00005600));
				REQUIRE(F(a, 10, 16) == F(0x00005400));
				REQUIRE(F(a, 11, 16) == F(0x00005000));
				REQUIRE(F(a, 12, 16) == F(0x00005000));
				REQUIRE(F(a, 13, 16) == F(0x00004000));
				REQUIRE(F(a, 14, 16) == F(0x00004000));
				REQUIRE(F(a, 15, 16) == F(0x00000000));

				REQUIRE(F(a, 16, 16) == F(0x00000000));
			}
		}

		void testNegation()
		{
			{
				using F = Signed_Integer<16, uint8>;
				PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

				for (integer i = -512; i <= 512; ++i)
				{
					REQUIRE(-F(i) == F(-i));
					REQUIRE(F(i) + (-F(i)) == 0);
				} 
			}

			{
				using F = Unsigned_Integer<16, uint8>;
				PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

				for (integer i = -512; i < 512; ++i)
				{
					REQUIRE(-F(i) == F(-i));
					REQUIRE(F(i) + (-F(i)) == 0);
				}
			}
		}

		void testIncrementDecrement()
		{
			using F = Signed_Integer<20, uint8>;
			PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

			for (integer i = -512; i <= 512; ++i)
			{
				REQUIRE(--F(i) == i - 1);
				REQUIRE(++F(i) == i + 1);
			}

			REQUIRE(++F(0x7FFFF) == -0x80000);
			REQUIRE(--F(0x80000) == +0x7FFFF);
			REQUIRE(--F(0x10000) == +0x0FFFF);
			REQUIRE(++F(0x0FFFF) == +0x10000);
		}

		void testComparison()
		{
			using F = Signed_Integer<20, uint8>;
			PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

			REQUIRE(negative(F(-0x00001)));
			REQUIRE(negative(F(-0x23456)));
			REQUIRE(negative(F(+0x83456u)));
			REQUIRE(negative(F(+0x80000u)));
			REQUIRE(positive(F(+0x00001)));
			REQUIRE(positive(F(+0x23456)));
			REQUIRE(positive(F(+0x7FFFF)));
			REQUIRE(positive(F(+0x7FFFFu)));
			REQUIRE(zero(F(0x000000)));
			REQUIRE(zero(F(0x000000u)));

			REQUIRE(F(-0x7FFFF) < F(0x12354));
			REQUIRE(F(-0x20000) < F(0x10000));
			REQUIRE(F(-0x10000) < F(0x00001));
			REQUIRE(F(0x00000) < F(0x00001));
			REQUIRE(F(0x00000) < F(0x04323));
			REQUIRE(F(0x04323) < F(0x10000));
			REQUIRE(F(0x100000) < F(0x20000));
			REQUIRE(F(0x23456) < F(0x34567));
		}

		void testClearBits()
		{
				using F = Unsigned_Integer<16, uint8>;
				PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

				auto f = [&](integer begin, integer end) -> F
				{
					return F(0xFFFF).clearBits(begin, end);
				};

				// Clear the front for multiple words.

				REQUIRE(f(0, 0) == F(0xFFFF));
				REQUIRE(f(0, 1) == F(0xFFFE));
				REQUIRE(f(0, 2) == F(0xFFFC));
				REQUIRE(f(0, 3) == F(0xFFF8));
				REQUIRE(f(0, 4) == F(0xFFF0));
				REQUIRE(f(0, 5) == F(0xFFE0));
				REQUIRE(f(0, 6) == F(0xFFC0));
				REQUIRE(f(0, 7) == F(0xFF80));
				REQUIRE(f(0, 8) == F(0xFF00));
				REQUIRE(f(0, 9) == F(0xFE00));
				REQUIRE(f(0, 10) == F(0xFC00));
				REQUIRE(f(0, 11) == F(0xF800));
				REQUIRE(f(0, 12) == F(0xF000));
				REQUIRE(f(0, 13) == F(0xE000));
				REQUIRE(f(0, 14) == F(0xC000));
				REQUIRE(f(0, 15) == F(0x8000));
				REQUIRE(f(0, 16) == F(0x0000));

				// Clear the tail for multiple words.

				REQUIRE(f(16, 16) == F(0xFFFF));
				REQUIRE(f(15, 16) == F(0x7FFF));
				REQUIRE(f(14, 16) == F(0x3FFF));
				REQUIRE(f(13, 16) == F(0x1FFF));
				REQUIRE(f(12, 16) == F(0x0FFF));
				REQUIRE(f(11, 16) == F(0x07FF));
				REQUIRE(f(10, 16) == F(0x03FF));
				REQUIRE(f(9, 16) == F(0x01FF));
				REQUIRE(f(8, 16) == F(0x00FF));
				REQUIRE(f(7, 16) == F(0x007F));
				REQUIRE(f(6, 16) == F(0x003F));
				REQUIRE(f(5, 16) == F(0x001F));
				REQUIRE(f(4, 16) == F(0x000F));
				REQUIRE(f(3, 16) == F(0x0007));
				REQUIRE(f(2, 16) == F(0x0003));
				REQUIRE(f(1, 16) == F(0x0001));
				REQUIRE(f(0, 16) == F(0x0000));

				// Clear the front inside a single word.

				REQUIRE(f(0, 0) == F(0xFFFF));
				REQUIRE(f(0, 1) == F(0xFFFE));
				REQUIRE(f(0, 2) == F(0xFFFC));
				REQUIRE(f(0, 3) == F(0xFFF8));
				REQUIRE(f(0, 4) == F(0xFFF0));
				REQUIRE(f(0, 5) == F(0xFFE0));
				REQUIRE(f(0, 6) == F(0xFFC0));
				REQUIRE(f(0, 7) == F(0xFF80));
				REQUIRE(f(0, 8) == F(0xFF00));

				// Clear the tail inside a single word.

				REQUIRE(f(0, 8) == F(0xFF00));
				REQUIRE(f(1, 8) == F(0xFF01));
				REQUIRE(f(2, 8) == F(0xFF03));
				REQUIRE(f(3, 8) == F(0xFF07));
				REQUIRE(f(4, 8) == F(0xFF0F));
				REQUIRE(f(5, 8) == F(0xFF1F));
				REQUIRE(f(6, 8) == F(0xFF3F));
				REQUIRE(f(7, 8) == F(0xFF7F));
				REQUIRE(f(8, 8) == F(0xFFFF));

				// Clear zero bits.

				REQUIRE(f(0, 0) == F(0xFFFF));
				REQUIRE(f(1, 1) == F(0xFFFF));
				REQUIRE(f(2, 2) == F(0xFFFF));
				REQUIRE(f(3, 3) == F(0xFFFF));
				REQUIRE(f(4, 4) == F(0xFFFF));
				REQUIRE(f(5, 5) == F(0xFFFF));
				REQUIRE(f(6, 6) == F(0xFFFF));
				REQUIRE(f(7, 7) == F(0xFFFF));
				REQUIRE(f(8, 8) == F(0xFFFF));
				REQUIRE(f(9, 9) == F(0xFFFF));
				REQUIRE(f(10, 10) == F(0xFFFF));
				REQUIRE(f(11, 11) == F(0xFFFF));
				REQUIRE(f(12, 12) == F(0xFFFF));
				REQUIRE(f(13, 13) == F(0xFFFF));
				REQUIRE(f(14, 14) == F(0xFFFF));
				REQUIRE(f(15, 15) == F(0xFFFF));
				REQUIRE(f(16, 16) == F(0xFFFF));

				// Clear one bit.

				REQUIRE(f(0, 1) == F(0xFFFE));
				REQUIRE(f(1, 2) == F(0xFFFD));
				REQUIRE(f(2, 3) == F(0xFFFB));
				REQUIRE(f(3, 4) == F(0xFFF7));
				REQUIRE(f(4, 5) == F(0xFFEF));
				REQUIRE(f(5, 6) == F(0xFFDF));
				REQUIRE(f(6, 7) == F(0xFFBF));
				REQUIRE(f(7, 8) == F(0xFF7F));
				REQUIRE(f(8, 9) == F(0xFEFF));
				REQUIRE(f(9, 10) == F(0xFDFF));
				REQUIRE(f(10, 11) == F(0xFBFF));
				REQUIRE(f(11, 12) == F(0xF7FF));
				REQUIRE(f(12, 13) == F(0xEFFF));
				REQUIRE(f(13, 14) == F(0xDFFF));
				REQUIRE(f(14, 15) == F(0xBFFF));
				REQUIRE(f(15, 16) == F(0x7FFF));

				// Clear two bits.

				REQUIRE(f(0, 2) == F(0xFFFC));
				REQUIRE(f(1, 3) == F(0xFFF9));
				REQUIRE(f(2, 4) == F(0xFFF3));
				REQUIRE(f(3, 5) == F(0xFFE7));
				REQUIRE(f(4, 6) == F(0xFFCF));
				REQUIRE(f(5, 7) == F(0xFF9F));
				REQUIRE(f(6, 8) == F(0xFF3F));
				REQUIRE(f(7, 9) == F(0xFE7F));
				REQUIRE(f(8, 10) == F(0xFCFF));
				REQUIRE(f(9, 11) == F(0xF9FF));
				REQUIRE(f(10, 12) == F(0xF3FF));
				REQUIRE(f(11, 13) == F(0xE7FF));
				REQUIRE(f(12, 14) == F(0xCFFF));
				REQUIRE(f(13, 15) == F(0x9FFF));
				REQUIRE(f(14, 16) == F(0x3FFF));
		}

		void testAddition()
		{
			{
				using F = Signed_Integer<64, uint8>;
				PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

				REQUIRE(F(-10) + F(-15) == F(-25));
			}
		}

		void testArithmetic()
		{
			{
				using F = Unsigned_Integer<1, uint8>;
				PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

				REQUIRE(F(0) + F(0) == F(0));
				REQUIRE(F(0) + F(1) == F(1));
				REQUIRE(F(1) + F(0) == F(1));
				REQUIRE(F(1) + F(1) == F(0));

				REQUIRE(F(0) - F(0) == F(0));
				REQUIRE(F(0) - F(1) == F(-1));
				REQUIRE(F(1) - F(0) == F(1));
				REQUIRE(F(1) - F(1) == F(0));
			}

			{
				using F = Unsigned_Integer<2, uint8>;
				PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

				REQUIRE(F(0) + F(0) == F(0));
				REQUIRE(F(0) + F(1) == F(1));
				REQUIRE(F(0) + F(2) == F(2));
				REQUIRE(F(0) + F(3) == F(3));
				REQUIRE(F(1) + F(0) == F(1));
				REQUIRE(F(1) + F(1) == F(2));
				REQUIRE(F(1) + F(2) == F(3));
				REQUIRE(F(1) + F(3) == F(0));
				REQUIRE(F(2) + F(0) == F(2));
				REQUIRE(F(2) + F(1) == F(3));
				REQUIRE(F(2) + F(2) == F(0));
				REQUIRE(F(2) + F(3) == F(1));
				REQUIRE(F(3) + F(0) == F(3));
				REQUIRE(F(3) + F(1) == F(0));
				REQUIRE(F(3) + F(2) == F(1));
				REQUIRE(F(3) + F(3) == F(2));

				REQUIRE(F(0) - F(0) == F(0));
				REQUIRE(F(0) - F(1) == F(-1));
				REQUIRE(F(0) - F(2) == F(-2));
				REQUIRE(F(0) - F(3) == F(-3));
				REQUIRE(F(1) - F(0) == F(1));
				REQUIRE(F(1) - F(1) == F(0));
				REQUIRE(F(1) - F(2) == F(-1));
				REQUIRE(F(1) - F(3) == F(-2));
				REQUIRE(F(2) - F(0) == F(2));
				REQUIRE(F(2) - F(1) == F(1));
				REQUIRE(F(2) - F(2) == F(0));
				REQUIRE(F(2) - F(3) == F(-1));
				REQUIRE(F(3) - F(0) == F(3));
				REQUIRE(F(3) - F(1) == F(2));
				REQUIRE(F(3) - F(2) == F(1));
				REQUIRE(F(3) - F(3) == F(0));
			}

			{
				using F = Unsigned_Integer<20, uint8>;
				PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

				auto f = [](integer a, integer b) -> bool
				{
					return (F(a) + F(b)) == (a + b);
				};

				REQUIRE(f(0x13413, 0x32432));
				REQUIRE(f(0x35486, 0x72234));
				REQUIRE(f(0x91847, 0xF4824));
			}

			{
				using F = Unsigned_Integer<20, uint8>;
				PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

				auto f = [](integer a, integer b) -> bool
				{
					return (F(a) - F(b)) == (a - b);
				};

				REQUIRE(f(0x13413, 0x32432));
				REQUIRE(f(0x35486, 0x72234));
				REQUIRE(f(0x91847, 0xF4824));
			}

			{
				using F = Unsigned_Integer<32, uint8>;
				PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

				uint32 a = 0x12345678;
				for (integer i = 0;i < 32;++i)
				{
					// Interesting fact:
					// if i ranges up to 33, then the following
					// tests fail for i == 32. This probably has 
					// to do with the shifts (a << i) and (a >> i) 
					// being implementation-defined in this case.

					REQUIRE(
						(F(a) << i) == F(a << i));

					REQUIRE(
						(F(a) >> i) == F(a >> i));
				}

				// We define the out-of-range shift anyway.
				REQUIRE((F(a) << 33) == F(0));
				REQUIRE((F(a) >> 33) == F(0));
			}
		}

		void testBitOperators()
		{
			using F = Unsigned_Integer<20, uint8>;
			PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

			REQUIRE((F(0x12345678u) ^ F(0x45635603u)) == (0x12345678u ^ 0x45635603u));
			REQUIRE((F(0x12345678u) | F(0x45635603u)) == (0x12345678u | 0x45635603u));
			REQUIRE((F(0x12345678u) & F(0x45635603u)) == (0x12345678u & 0x45635603u));
			REQUIRE((~F(0x12345678u)) == (~0x12345678u));
		}

		void testCounting()
		{
			{
				using F = Unsigned_Integer<1, uint8>;
				PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

				REQUIRE(F(0).oneBits() == 0);
				REQUIRE(F(1).oneBits() == 1);
			}

			{
				using F = Unsigned_Integer<32, uint8>;
				PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

				REQUIRE(F(0x12345678).oneBits() == 1 + 1 + 2 + 1 + 2 + 2 + 3 + 1);

				REQUIRE(F(0x9ABCDEF0).oneBits() == 2 + 2 + 3 + 2 + 3 + 3 + 4 + 0);
			}
		}

		void testSetBits()
		{
			using F = Unsigned_Integer<20, uint8>;
			PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

			{
				REQUIRE(!F(0).bit(0));
				REQUIRE(F(1).bit(0));

				REQUIRE(F(0).setBit(0).bit(0));
				REQUIRE(F(0).setBit(0, true).bit(0));
				REQUIRE(!F(1).setBit(0, false).bit(0));
				REQUIRE(!F(1).clearBit(0).bit(0));
			}

			F a;

			REQUIRE(!a.bit(20));
			REQUIRE(!a.bit(21));
			REQUIRE(!a.bit(22));
			REQUIRE(!a.bit(32));
			REQUIRE(!a.bit(64));
			REQUIRE(!a.bit(128));

			a.setBit(0);
			REQUIRE(a == (1 << 0));

			a.setBit(1);
			REQUIRE(a == (1 << 0) + (1 << 1));

			a.setBit(2);
			REQUIRE(a == (1 << 0) + (1 << 1) + (1 << 2));

			a.setBit(7);
			REQUIRE(a == (1 << 0) + (1 << 1) + (1 << 2) + (1 << 7));

			a.setBit(12);
			REQUIRE(a == (1 << 0) + (1 << 1) + (1 << 2) + (1 << 7) + (1 << 12));

			a.clearBit(2);
			REQUIRE(a == (1 << 0) + (1 << 1) + (1 << 7) + (1 << 12));

			a.clearBit(7);
			REQUIRE(a == (1 << 0) + (1 << 1) + (1 << 12));

			a.clearBit(12);
			REQUIRE(a == (1 << 0) + (1 << 1));

			a.clearBit(1);
			REQUIRE(a == (1 << 0));

			a.clearBit(0);
			REQUIRE(a == 0);

			a.setBit(19);
			REQUIRE(a == (1 << 19));

			a.clearBit(19);
			REQUIRE(a == 0);

			a = 0x12345678;
			a.clearBits();
			REQUIRE(a == 0);

			a = 0x12345678;
			a.setBits();
			REQUIRE(a == 0x000FFFFF);

			{
				Signed_Integer<20, uint8> b(-1);
				REQUIRE(b.bit(20));
				REQUIRE(b.bit(21));
				REQUIRE(b.bit(22));
				REQUIRE(b.bit(32));
				REQUIRE(b.bit(64));
				REQUIRE(b.bit(128));
			}
			{
				Signed_Integer<20, uint8> b(0);
				REQUIRE(!b.bit(20));
				REQUIRE(!b.bit(21));
				REQUIRE(!b.bit(22));
				REQUIRE(!b.bit(32));
				REQUIRE(!b.bit(64));
				REQUIRE(!b.bit(128));
			}
		}

		void testMultiplication()
		{
			{
				using F = Unsigned_Integer<20, uint8>;
				PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

				for (integer i = 0; i < 256; ++i)
				{
					for (integer j = 0; j < 256; ++j)
					{
						REQUIRE(F(i) * F(j) == F(i * j));
					}
				}
			}
			{
				using F = Signed_Integer<20, uint8>;
				PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

				for (integer i = -127; i <= 128; ++i)
				{
					for (integer j = -127; j <= 128; ++j)
					{
						REQUIRE(F(i) * F(j) == F(i * j));
					}
				}
			}
		}

		void testDivision()
		{
			{
				using F = Unsigned_Integer<23, uint8>;
				PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

				for (integer i = 0; i < 300; i += 7)
				{
					for (integer j = 1; j < 300; j += 5)
					{
						REQUIRE(F(i * 213) / F(j) == F((i * 213) / j));
					}
				}
			}
			{
				using F = Signed_Integer<23, uint8>;
				PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

				for (integer i = -300; i < 300; i += 3)
				{
					for (integer j = -300; j < 300; j += 11)
					{
						if (j != 0)
						{
							REQUIRE(F(i * 213) / F(j) == F((i * 213) / j));
						}
					}
				}
			}
		}

		void testModulo()
		{
			{
				using F = Unsigned_Integer<23, uint8>;
				PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

				for (integer i = 0; i < 300; i += 7)
				{
					for (integer j = 1; j < 300; j += 5)
					{
						REQUIRE(F(i * 213) % F(j) == F((i * 213) % j));
					}
				}
			}
			{
				using F = Signed_Integer<23, uint8>;
				PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

				for (integer i = -300; i < 300; i += 3)
				{
					for (integer j = -300; j < 300; j += 11)
					{
						if (j != 0)
						{
							REQUIRE(F(i * 213) % F(j) == F((i * 213) % j));
						}
					}
				}
			}
		}

		void testNumberOfOneBits()
		{
			using F = Unsigned_Integer<20, uint8>;
			PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

			auto test = [&](F that, integer correct)
			{
				return numberOfOneBits(that) == correct;
			};

			REQUIRE(test(0xFFF34567ul, 10));
			REQUIRE(test(0x00034567ul, 10));
		}
	};

	TEST_CASE("MultiInteger", "[MultiInteger]")
	{
	}

}
