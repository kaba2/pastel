// Description: Testing for MultiInteger
// DocumentationOf: multi_integer.h

#include "test/test_init.h"

#include "pastel/sys/integer/multi_integer.h"

#include <iomanip>

TEST_CASE("Infinity (MultiInteger)")
{
	{
		using F = Unsigned_Integer<32, uint8>;
		CHECK((F)Infinity() == 0xFFFFFFFF);
		CHECK(positive((F)Infinity()));
		CHECK(zero(++(F)Infinity()));
	}
	{
		using F = Signed_Integer<32, uint8>;
		CHECK((F)Infinity() == 0x7FFFFFFF);
		CHECK(positive((F)Infinity()));
		CHECK(negative(++(F)Infinity()));
	}
	{
		using F = Unsigned_Integer<8, uint8>;
		CHECK((F)Infinity() == 255);
		CHECK(positive((F)Infinity()));
		CHECK(zero(++(F)Infinity()));
	}
	{
		using F = Signed_Integer<8, uint8>;
		CHECK((F)Infinity() == 127);
		CHECK(positive((F)Infinity()));
		CHECK(negative(++(F)Infinity()));
	}
	{
		using F = Unsigned_Integer<7, uint8>;
		CHECK((F)Infinity() == 127);
		CHECK(positive((F)Infinity()));
		CHECK(zero(++(F)Infinity()));
	}
	{
		using F = Signed_Integer<7, uint8>;
		CHECK((F)Infinity() == 63);
		CHECK(positive((F)Infinity()));
		CHECK(negative(++(F)Infinity()));
	}
}

TEST_CASE("AsString (MultiInteger)")
{
	{
		using F = Unsigned_Integer<1, uint8>;
		CHECK(F(0).asString(2) == "0");
		CHECK(F(1).asString(2) == "1");
		CHECK(F(2).asString(2) == "0");
		CHECK(F(3).asString(2) == "1");
	}
	{
		using F = Signed_Integer<1, uint8>;

		CHECK(F(-2).asString(2) == "0");
		CHECK(F(-1).asString(2) == "-1");
		CHECK(F(0).asString(2) == "0");
		CHECK(F(1).asString(2) == "-1");
		CHECK(F(2).asString(2) == "0");
		CHECK(F(3).asString(2) == "-1");
	}
	{
		using F = Signed_Integer<20, uint8>;
		CHECK(F(-0x80000).asString(2) == "-10000000000000000000");
		CHECK(F(0x80000).asString(2) == "-10000000000000000000");
		CHECK(F(-0x80000).asString() == "-524288");
		CHECK(F(0x80000).asString() == "-524288");
	}
	{
		using F = Unsigned_Integer<32, uint8>;

		CHECK(F(0x12345678).asString(16) == "12345678");
		CHECK(F(132).asString(10) == "132");
		CHECK(F(432874).asString(10) == "432874");

		CHECK(F(0x0).asString(16) == "0");
		CHECK(F(0xFFFFFFFF).asString(16) == "ffffffff");

		CHECK(F(0x9ABCDEF0).asString(2) ==
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

		CHECK(F(-132).asString(10) == "-132");
		CHECK(F(-0x12345678).asString(16) == "-12345678");
		CHECK(F(-1).asString(2) == "-1");
		CHECK(F(-2).asString(2) == "-10");
		CHECK(F(-3).asString(2) == "-11");
		CHECK(F(-4).asString(2) == "-100");
	}
	{
		using F = Unsigned_Integer<32, uint8>;
		CHECK(F(0x12345678).asString(16) == "12345678");

		CHECK(F(0x9ABCDEF0).asString(2) ==
			"10011010101111001101111011110000");
	}
}

TEST_CASE("AsNative (MultiInteger)")
{
	{
		using F = Unsigned_Integer<32, uint8>;
		CHECK(static_cast<uint64>(F(true)) == 1);
	}
	{
		using F = Unsigned_Integer<32, uint8>;
		CHECK(static_cast<uint64>(F(false)) == 0);
	}
	{
		using F = Unsigned_Integer<32, uint8>;
		CHECK(static_cast<uint64>(F(0x12345678)) == 0x12345678);
	}
	{
		using F = Unsigned_Integer<31, uint8>;
		CHECK((uint64)F(0x12345678) == 0x12345678);
	}
	{
		using F = Unsigned_Integer<30, uint8>;
		CHECK((uint64)F(0x12345678) == 0x12345678);
	}
	{
		using F = Unsigned_Integer<29, uint8>;
		CHECK((uint64)F(0x12345678) == 0x12345678);
	}
	{
		using F = Unsigned_Integer<28, uint8>;
		CHECK((uint64)F(0x12345678) == 0x02345678);
	}
	{
		using F = Unsigned_Integer<20, uint8>;
		CHECK((uint64)F(0x12345678) == 0x00045678);
	}
	{
		using F = Unsigned_Integer<19, uint8>;
		CHECK((uint64)F(0x12345678) == 0x00045678);
	}
	{
		using F = Unsigned_Integer<18, uint8>;
		CHECK((uint64)F(0x12345678) == 0x00005678);
	}
	{
		using F = Unsigned_Integer<17, uint8>;
		CHECK((uint64)F(0x12345678) == 0x00005678);
	}
	{
		using F = Unsigned_Integer<16, uint8>;
		CHECK((uint64)F(0x12345678) == 0x00005678);
	}
	{
		using F = Unsigned_Integer<15, uint8>;
		CHECK((uint64)F(0x12345678) == 0x00005678);
	}
	{
		using F = Unsigned_Integer<14, uint8>;
		CHECK((uint64)F(0x12345678) == 0x00001678);
	}
	{
		using F = Unsigned_Integer<13, uint8>;
		CHECK((uint64)F(0x12345678) == 0x00001678);
	}
	{
		using F = Unsigned_Integer<12, uint8>;
		CHECK((uint64)F(0x12345678) == 0x00000678);
	}
	{
		using F = Unsigned_Integer<11, uint8>;
		CHECK((uint64)F(0x12345678) == 0x00000678);
	}
	{
		using F = Unsigned_Integer<10, uint8>;
		CHECK((uint64)F(0x12345678) == 0x00000278);
	}
	{
		using F = Unsigned_Integer<9, uint8>;
		CHECK((uint64)F(0x12345678) == 0x00000078);
	}
	{
		using F = Unsigned_Integer<8, uint8>;
		CHECK((uint64)F(0x12345678) == 0x00000078);
	}
	{
		using F = Unsigned_Integer<7, uint8>;
		CHECK((uint64)F(0x12345678) == 0x00000078);
	}
	{
		using F = Unsigned_Integer<6, uint8>;
		CHECK((uint64)F(0x12345678) == 0x00000038);
	}
	{
		using F = Unsigned_Integer<5, uint8>;
		CHECK((uint64)F(0x12345678) == 0x00000018);
	}
	{
		using F = Unsigned_Integer<4, uint8>;
		CHECK((uint64)F(0x12345678) == 0x00000008);
	}
	{
		using F = Unsigned_Integer<3, uint8>;
		CHECK((uint64)F(0x12345678) == 0x00000000);
	}
	{
		using F = Unsigned_Integer<2, uint8>;
		CHECK((uint64)F(0x12345678) == 0x00000000);
	}
	{
		using F = Unsigned_Integer<1, uint8>;
		CHECK((uint64)F(0x12345678) == 0x00000000);
	}
	{
		using F = Unsigned_Integer<32, uint8>;
		CHECK((uint64)F(0x87654321) == 0x87654321);
	}
	{
		using F = Signed_Integer<32, uint8>;
		CHECK(static_cast<int64>(F(-0x12345678)) == -0x12345678);
	}
	{
		using F = Signed_Integer<16, uint8>;
		for (integer i = -40000; i < 40000; i += 13)
		{
			CHECK(static_cast<int64>(F(i)) == mod(i + 32768, (integer)65536) - 32768);
		}
	}
	{
		using F = Signed_Integer<15, uint8>;
		for (integer i = -40000; i < 40000; i += 13)
		{
			CHECK(static_cast<int64>(F(i)) == mod(i + 16384, (integer)32768) - 16384);
		}
	}
	{
		using F = Signed_Integer<6, uint8>;
		for (integer i = -100; i < 100; ++i)
		{
			CHECK(static_cast<int64>(F(i)) == mod(i + 32, (integer)64) - 32);
		}
	}
	{
		using F = Signed_Integer<6, uint16>;
		for (integer i = -100; i < 100; ++i)
		{
			CHECK(static_cast<int64>(F(i)) == mod(i + 32, (integer)64) - 32);
		}
	}
}

TEST_CASE("Construction (MultiInteger)")
{
	using F = Unsigned_Integer<20, uint8>;
	PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

	CHECK(F(0x12345678) == F({ 0x12, 0x34, 0x56, 0x78 }));
	CHECK(F(0x12345678) == F({ 0x34, 0x56, 0x78 }));
	CHECK(F(0x12345678) == F({ 0x04, 0x56, 0x78 }));

	{
		CHECK(F((int8)-1) == F({0xFF, 0xFF, 0xFF}));
		CHECK(F((int8)-2) == F({0xFF, 0xFF, 0xFE}));
		CHECK(F((int8)-16) == F({0xFF, 0xFF, 0xF0}));
	}
	{
		F a({ 0x04, 0x56, 0x78 });
		CHECK(a.word(0) == 0x78);
		CHECK(a.word(1) == 0x56);
		CHECK(a.word(2) == 0x04);
	}

	{
		F a(0x12345678);
		CHECK(a.word(0) == 0x78);
		CHECK(a.word(1) == 0x56);
		CHECK(a.word(2) == 0x04);
	}

	CHECK(F(0) == F());	

	{
		F a(-1);
		CHECK(a.word(0) == 0xFF);
		CHECK(a.word(1) == 0xFF);
		CHECK(a.word(2) == 0x0F);
	}

	{
		F a(-16);
		CHECK(a.word(0) == 0xF0);
		CHECK(a.word(1) == 0xFF);
		CHECK(a.word(2) == 0x0F);
	}

	{
		using F = Unsigned_Integer<20, uint8>;
		PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

		F a(0x12345678);

		CHECK(F(a, 0, 0) == F(0x00000000));
		CHECK(F(a, 0, 1) == F(0x00000000));
		CHECK(F(a, 0, 2) == F(0x00000000));
		CHECK(F(a, 0, 3) == F(0x00000000));
		CHECK(F(a, 0, 4) == F(0x00000008));
		CHECK(F(a, 0, 5) == F(0x00000018));
		CHECK(F(a, 0, 6) == F(0x00000038));
		CHECK(F(a, 0, 7) == F(0x00000078));

		CHECK(F(a, 0, 8) == F(0x00000078));
		CHECK(F(a, 0, 9) == F(0x00000078));
		CHECK(F(a, 0, 10) == F(0x00000278));
		CHECK(F(a, 0, 11) == F(0x00000678));
		CHECK(F(a, 0, 12) == F(0x00000678));
		CHECK(F(a, 0, 13) == F(0x00001678));
		CHECK(F(a, 0, 14) == F(0x00001678));
		CHECK(F(a, 0, 15) == F(0x00005678));

		CHECK(F(a, 0, 16) == F(0x00005678));
		CHECK(F(a, 1, 16) == F(0x00005678));
		CHECK(F(a, 2, 16) == F(0x00005678));
		CHECK(F(a, 3, 16) == F(0x00005678));
		CHECK(F(a, 4, 16) == F(0x00005670));
		CHECK(F(a, 5, 16) == F(0x00005660));
		CHECK(F(a, 6, 16) == F(0x00005640));
		CHECK(F(a, 7, 16) == F(0x00005600));

		CHECK(F(a, 8, 16) == F(0x00005600));
		CHECK(F(a, 9, 16) == F(0x00005600));
		CHECK(F(a, 10, 16) == F(0x00005400));
		CHECK(F(a, 11, 16) == F(0x00005000));
		CHECK(F(a, 12, 16) == F(0x00005000));
		CHECK(F(a, 13, 16) == F(0x00004000));
		CHECK(F(a, 14, 16) == F(0x00004000));
		CHECK(F(a, 15, 16) == F(0x00000000));

		CHECK(F(a, 16, 16) == F(0x00000000));
	}
}

TEST_CASE("Negation (MultiInteger)")
{
	{
		using F = Signed_Integer<16, uint8>;
		PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

		for (integer i = -512; i <= 512; ++i)
		{
			CHECK(-F(i) == F(-i));
			CHECK(F(i) + (-F(i)) == 0);
		} 
	}

	{
		using F = Unsigned_Integer<16, uint8>;
		PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

		for (integer i = -512; i < 512; ++i)
		{
			CHECK(-F(i) == F(-i));
			CHECK(F(i) + (-F(i)) == 0);
		}
	}
}

TEST_CASE("IncrementDecrement (MultiInteger)")
{
	using F = Signed_Integer<20, uint8>;
	PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

	for (integer i = -512; i <= 512; ++i)
	{
		CHECK(--F(i) == i - 1);
		CHECK(++F(i) == i + 1);
	}

	CHECK(++F(0x7FFFF) == -0x80000);
	CHECK(--F(0x80000) == +0x7FFFF);
	CHECK(--F(0x10000) == +0x0FFFF);
	CHECK(++F(0x0FFFF) == +0x10000);
}

TEST_CASE("Comparison (MultiInteger)")
{
	using F = Signed_Integer<20, uint8>;
	PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

	CHECK(negative(F(-0x00001)));
	CHECK(negative(F(-0x23456)));
	CHECK(negative(F(+0x83456u)));
	CHECK(negative(F(+0x80000u)));
	CHECK(positive(F(+0x00001)));
	CHECK(positive(F(+0x23456)));
	CHECK(positive(F(+0x7FFFF)));
	CHECK(positive(F(+0x7FFFFu)));
	CHECK(zero(F(0x000000)));
	CHECK(zero(F(0x000000u)));

	CHECK(F(-0x7FFFF) < F(0x12354));
	CHECK(F(-0x20000) < F(0x10000));
	CHECK(F(-0x10000) < F(0x00001));
	CHECK(F(0x00000) < F(0x00001));
	CHECK(F(0x00000) < F(0x04323));
	CHECK(F(0x04323) < F(0x10000));
	CHECK(F(0x100000) < F(0x20000));
	CHECK(F(0x23456) < F(0x34567));
}

TEST_CASE("ClearBits (MultiInteger)")
{
		using F = Unsigned_Integer<16, uint8>;
		PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

		auto f = [&](integer begin, integer end) -> F
		{
			return F(0xFFFF).clearBits(begin, end);
		};

		// Clear the front for multiple words.

		CHECK(f(0, 0) == F(0xFFFF));
		CHECK(f(0, 1) == F(0xFFFE));
		CHECK(f(0, 2) == F(0xFFFC));
		CHECK(f(0, 3) == F(0xFFF8));
		CHECK(f(0, 4) == F(0xFFF0));
		CHECK(f(0, 5) == F(0xFFE0));
		CHECK(f(0, 6) == F(0xFFC0));
		CHECK(f(0, 7) == F(0xFF80));
		CHECK(f(0, 8) == F(0xFF00));
		CHECK(f(0, 9) == F(0xFE00));
		CHECK(f(0, 10) == F(0xFC00));
		CHECK(f(0, 11) == F(0xF800));
		CHECK(f(0, 12) == F(0xF000));
		CHECK(f(0, 13) == F(0xE000));
		CHECK(f(0, 14) == F(0xC000));
		CHECK(f(0, 15) == F(0x8000));
		CHECK(f(0, 16) == F(0x0000));

		// Clear the tail for multiple words.

		CHECK(f(16, 16) == F(0xFFFF));
		CHECK(f(15, 16) == F(0x7FFF));
		CHECK(f(14, 16) == F(0x3FFF));
		CHECK(f(13, 16) == F(0x1FFF));
		CHECK(f(12, 16) == F(0x0FFF));
		CHECK(f(11, 16) == F(0x07FF));
		CHECK(f(10, 16) == F(0x03FF));
		CHECK(f(9, 16) == F(0x01FF));
		CHECK(f(8, 16) == F(0x00FF));
		CHECK(f(7, 16) == F(0x007F));
		CHECK(f(6, 16) == F(0x003F));
		CHECK(f(5, 16) == F(0x001F));
		CHECK(f(4, 16) == F(0x000F));
		CHECK(f(3, 16) == F(0x0007));
		CHECK(f(2, 16) == F(0x0003));
		CHECK(f(1, 16) == F(0x0001));
		CHECK(f(0, 16) == F(0x0000));

		// Clear the front inside a single word.

		CHECK(f(0, 0) == F(0xFFFF));
		CHECK(f(0, 1) == F(0xFFFE));
		CHECK(f(0, 2) == F(0xFFFC));
		CHECK(f(0, 3) == F(0xFFF8));
		CHECK(f(0, 4) == F(0xFFF0));
		CHECK(f(0, 5) == F(0xFFE0));
		CHECK(f(0, 6) == F(0xFFC0));
		CHECK(f(0, 7) == F(0xFF80));
		CHECK(f(0, 8) == F(0xFF00));

		// Clear the tail inside a single word.

		CHECK(f(0, 8) == F(0xFF00));
		CHECK(f(1, 8) == F(0xFF01));
		CHECK(f(2, 8) == F(0xFF03));
		CHECK(f(3, 8) == F(0xFF07));
		CHECK(f(4, 8) == F(0xFF0F));
		CHECK(f(5, 8) == F(0xFF1F));
		CHECK(f(6, 8) == F(0xFF3F));
		CHECK(f(7, 8) == F(0xFF7F));
		CHECK(f(8, 8) == F(0xFFFF));

		// Clear zero bits.

		CHECK(f(0, 0) == F(0xFFFF));
		CHECK(f(1, 1) == F(0xFFFF));
		CHECK(f(2, 2) == F(0xFFFF));
		CHECK(f(3, 3) == F(0xFFFF));
		CHECK(f(4, 4) == F(0xFFFF));
		CHECK(f(5, 5) == F(0xFFFF));
		CHECK(f(6, 6) == F(0xFFFF));
		CHECK(f(7, 7) == F(0xFFFF));
		CHECK(f(8, 8) == F(0xFFFF));
		CHECK(f(9, 9) == F(0xFFFF));
		CHECK(f(10, 10) == F(0xFFFF));
		CHECK(f(11, 11) == F(0xFFFF));
		CHECK(f(12, 12) == F(0xFFFF));
		CHECK(f(13, 13) == F(0xFFFF));
		CHECK(f(14, 14) == F(0xFFFF));
		CHECK(f(15, 15) == F(0xFFFF));
		CHECK(f(16, 16) == F(0xFFFF));

		// Clear one bit.

		CHECK(f(0, 1) == F(0xFFFE));
		CHECK(f(1, 2) == F(0xFFFD));
		CHECK(f(2, 3) == F(0xFFFB));
		CHECK(f(3, 4) == F(0xFFF7));
		CHECK(f(4, 5) == F(0xFFEF));
		CHECK(f(5, 6) == F(0xFFDF));
		CHECK(f(6, 7) == F(0xFFBF));
		CHECK(f(7, 8) == F(0xFF7F));
		CHECK(f(8, 9) == F(0xFEFF));
		CHECK(f(9, 10) == F(0xFDFF));
		CHECK(f(10, 11) == F(0xFBFF));
		CHECK(f(11, 12) == F(0xF7FF));
		CHECK(f(12, 13) == F(0xEFFF));
		CHECK(f(13, 14) == F(0xDFFF));
		CHECK(f(14, 15) == F(0xBFFF));
		CHECK(f(15, 16) == F(0x7FFF));

		// Clear two bits.

		CHECK(f(0, 2) == F(0xFFFC));
		CHECK(f(1, 3) == F(0xFFF9));
		CHECK(f(2, 4) == F(0xFFF3));
		CHECK(f(3, 5) == F(0xFFE7));
		CHECK(f(4, 6) == F(0xFFCF));
		CHECK(f(5, 7) == F(0xFF9F));
		CHECK(f(6, 8) == F(0xFF3F));
		CHECK(f(7, 9) == F(0xFE7F));
		CHECK(f(8, 10) == F(0xFCFF));
		CHECK(f(9, 11) == F(0xF9FF));
		CHECK(f(10, 12) == F(0xF3FF));
		CHECK(f(11, 13) == F(0xE7FF));
		CHECK(f(12, 14) == F(0xCFFF));
		CHECK(f(13, 15) == F(0x9FFF));
		CHECK(f(14, 16) == F(0x3FFF));
}

TEST_CASE("Addition (MultiInteger)")
{
	{
		using F = Signed_Integer<64, uint8>;
		PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

		CHECK(F(-10) + F(-15) == F(-25));
	}
}

TEST_CASE("Arithmetic (MultiInteger)")
{
	{
		using F = Unsigned_Integer<1, uint8>;
		PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

		CHECK(F(0) + F(0) == F(0));
		CHECK(F(0) + F(1) == F(1));
		CHECK(F(1) + F(0) == F(1));
		CHECK(F(1) + F(1) == F(0));

		CHECK(F(0) - F(0) == F(0));
		CHECK(F(0) - F(1) == F(-1));
		CHECK(F(1) - F(0) == F(1));
		CHECK(F(1) - F(1) == F(0));
	}

	{
		using F = Unsigned_Integer<2, uint8>;
		PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

		CHECK(F(0) + F(0) == F(0));
		CHECK(F(0) + F(1) == F(1));
		CHECK(F(0) + F(2) == F(2));
		CHECK(F(0) + F(3) == F(3));
		CHECK(F(1) + F(0) == F(1));
		CHECK(F(1) + F(1) == F(2));
		CHECK(F(1) + F(2) == F(3));
		CHECK(F(1) + F(3) == F(0));
		CHECK(F(2) + F(0) == F(2));
		CHECK(F(2) + F(1) == F(3));
		CHECK(F(2) + F(2) == F(0));
		CHECK(F(2) + F(3) == F(1));
		CHECK(F(3) + F(0) == F(3));
		CHECK(F(3) + F(1) == F(0));
		CHECK(F(3) + F(2) == F(1));
		CHECK(F(3) + F(3) == F(2));

		CHECK(F(0) - F(0) == F(0));
		CHECK(F(0) - F(1) == F(-1));
		CHECK(F(0) - F(2) == F(-2));
		CHECK(F(0) - F(3) == F(-3));
		CHECK(F(1) - F(0) == F(1));
		CHECK(F(1) - F(1) == F(0));
		CHECK(F(1) - F(2) == F(-1));
		CHECK(F(1) - F(3) == F(-2));
		CHECK(F(2) - F(0) == F(2));
		CHECK(F(2) - F(1) == F(1));
		CHECK(F(2) - F(2) == F(0));
		CHECK(F(2) - F(3) == F(-1));
		CHECK(F(3) - F(0) == F(3));
		CHECK(F(3) - F(1) == F(2));
		CHECK(F(3) - F(2) == F(1));
		CHECK(F(3) - F(3) == F(0));
	}

	{
		using F = Unsigned_Integer<20, uint8>;
		PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

		auto f = [](integer a, integer b) -> bool
		{
			return (F(a) + F(b)) == (a + b);
		};

		CHECK(f(0x13413, 0x32432));
		CHECK(f(0x35486, 0x72234));
		CHECK(f(0x91847, 0xF4824));
	}

	{
		using F = Unsigned_Integer<20, uint8>;
		PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

		auto f = [](integer a, integer b) -> bool
		{
			return (F(a) - F(b)) == (a - b);
		};

		CHECK(f(0x13413, 0x32432));
		CHECK(f(0x35486, 0x72234));
		CHECK(f(0x91847, 0xF4824));
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

			CHECK(
				(F(a) << i) == F(a << i));

			CHECK(
				(F(a) >> i) == F(a >> i));
		}

		// We define the out-of-range shift anyway.
		CHECK((F(a) << 33) == F(0));
		CHECK((F(a) >> 33) == F(0));
	}
}

TEST_CASE("BitOperators (MultiInteger)")
{
	using F = Unsigned_Integer<20, uint8>;
	PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

	CHECK((F(0x12345678u) ^ F(0x45635603u)) == (0x12345678u ^ 0x45635603u));
	CHECK((F(0x12345678u) | F(0x45635603u)) == (0x12345678u | 0x45635603u));
	CHECK((F(0x12345678u) & F(0x45635603u)) == (0x12345678u & 0x45635603u));
	CHECK((~F(0x12345678u)) == (~0x12345678u));
}

TEST_CASE("Counting (MultiInteger)")
{
	{
		using F = Unsigned_Integer<1, uint8>;
		PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

		CHECK(F(0).oneBits() == 0);
		CHECK(F(1).oneBits() == 1);
	}

	{
		using F = Unsigned_Integer<32, uint8>;
		PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

		CHECK(F(0x12345678).oneBits() == 1 + 1 + 2 + 1 + 2 + 2 + 3 + 1);

		CHECK(F(0x9ABCDEF0).oneBits() == 2 + 2 + 3 + 2 + 3 + 3 + 4 + 0);
	}
}

TEST_CASE("SetBits (MultiInteger)")
{
	using F = Unsigned_Integer<20, uint8>;
	PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

	{
		CHECK(!F(0).bit(0));
		CHECK(F(1).bit(0));

		CHECK(F(0).setBit(0).bit(0));
		CHECK(F(0).setBit(0, true).bit(0));
		CHECK(!F(1).setBit(0, false).bit(0));
		CHECK(!F(1).clearBit(0).bit(0));
	}

	F a;

	CHECK(!a.bit(20));
	CHECK(!a.bit(21));
	CHECK(!a.bit(22));
	CHECK(!a.bit(32));
	CHECK(!a.bit(64));
	CHECK(!a.bit(128));

	a.setBit(0);
	CHECK(a == (1 << 0));

	a.setBit(1);
	CHECK(a == (1 << 0) + (1 << 1));

	a.setBit(2);
	CHECK(a == (1 << 0) + (1 << 1) + (1 << 2));

	a.setBit(7);
	CHECK(a == (1 << 0) + (1 << 1) + (1 << 2) + (1 << 7));

	a.setBit(12);
	CHECK(a == (1 << 0) + (1 << 1) + (1 << 2) + (1 << 7) + (1 << 12));

	a.clearBit(2);
	CHECK(a == (1 << 0) + (1 << 1) + (1 << 7) + (1 << 12));

	a.clearBit(7);
	CHECK(a == (1 << 0) + (1 << 1) + (1 << 12));

	a.clearBit(12);
	CHECK(a == (1 << 0) + (1 << 1));

	a.clearBit(1);
	CHECK(a == (1 << 0));

	a.clearBit(0);
	CHECK(a == 0);

	a.setBit(19);
	CHECK(a == (1 << 19));

	a.clearBit(19);
	CHECK(a == 0);

	a = 0x12345678;
	a.clearBits();
	CHECK(a == 0);

	a = 0x12345678;
	a.setBits();
	CHECK(a == 0x000FFFFF);

	{
		Signed_Integer<20, uint8> b(-1);
		CHECK(b.bit(20));
		CHECK(b.bit(21));
		CHECK(b.bit(22));
		CHECK(b.bit(32));
		CHECK(b.bit(64));
		CHECK(b.bit(128));
	}
	{
		Signed_Integer<20, uint8> b(0);
		CHECK(!b.bit(20));
		CHECK(!b.bit(21));
		CHECK(!b.bit(22));
		CHECK(!b.bit(32));
		CHECK(!b.bit(64));
		CHECK(!b.bit(128));
	}
}

TEST_CASE("Multiplication (MultiInteger)")
{
	{
		using F = Unsigned_Integer<20, uint8>;
		PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

		for (integer i = 0; i < 256; ++i)
		{
			for (integer j = 0; j < 256; ++j)
			{
				CHECK(F(i) * F(j) == F(i * j));
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
				CHECK(F(i) * F(j) == F(i * j));
			}
		}
	}
}

TEST_CASE("Division (MultiInteger)")
{
	{
		using F = Unsigned_Integer<23, uint8>;
		PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

		for (integer i = 0; i < 300; i += 7)
		{
			for (integer j = 1; j < 300; j += 5)
			{
				CHECK(F(i * 213) / F(j) == F((i * 213) / j));
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
					CHECK(F(i * 213) / F(j) == F((i * 213) / j));
				}
			}
		}
	}

	{
		using F = Signed_Integer<1, uint8>;
		PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

		CHECK(F(0) / -2 == F(0));
		CHECK(F(0) / -1 == F(0));
		CHECK(F(0) / 1 == F(0));
		CHECK(F(0) / 2 == F(0));

		CHECK(F(-1) / -2 == F(0));
		CHECK(F(-1) / -1 == F(1));
		CHECK(F(-1) / 1 == F(1));
		CHECK(F(-1) / 2 == F(0));
		CHECK(F(-1) / 3 == F(0));
	}

	{
		using F = Signed_Integer<2, uint8>;
		PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

		CHECK(F(0) / 1 == F(0));
		CHECK(F(1) / 1 == F(1));
		CHECK(F(-1) / 1 == F(-1));
		F(-2) / 2;
		CHECK(F(-2) / 1 == F(-2));

		CHECK(F(0) / 2 == F(0));
		CHECK(F(1) / 2 == F(0));
		CHECK(F(-1) / 2 == F(0));
		CHECK(F(-2) / 2 == F(-1));

		CHECK(F(0) / 3 == F(0));
		CHECK(F(1) / 3 == F(0));
		CHECK(F(-1) / 3 == F(0));
		CHECK(F(-2) / 3 == F(0));

		CHECK(F(0) / 4 == F(0));
		CHECK(F(1) / 4 == F(0));
		CHECK(F(-1) / 4 == F(0));
		CHECK(F(-2) / 4 == F(0));
	}
	{
		using F = Unsigned_Integer<2, uint8>;
		PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

		CHECK(F(0) / 1 == F(0));
		CHECK(F(1) / 1 == F(1));
		CHECK(F(2) / 1 == F(2));
		CHECK(F(3) / 1 == F(3));

		CHECK(F(0) / 2 == F(0));
		CHECK(F(1) / 2 == F(0));
		CHECK(F(2) / 2 == F(1));
		CHECK(F(3) / 2 == F(1));

		CHECK(F(0) / 3 == F(0));
		CHECK(F(1) / 3 == F(0));
		CHECK(F(2) / 3 == F(0));
		CHECK(F(3) / 3 == F(1));

		CHECK(F(0) / 4 == F(0));
		CHECK(F(1) / 4 == F(0));
		CHECK(F(2) / 4 == F(0));
		CHECK(F(3) / 4 == F(0));
	}
}

TEST_CASE("Modulo (MultiInteger)")
{
	{
		using F = Unsigned_Integer<23, uint8>;
		PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

		for (integer i = 0; i < 300; i += 7)
		{
			for (integer j = 1; j < 300; j += 5)
			{
				CHECK(F(i * 213) % F(j) == F((i * 213) % j));
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
					CHECK(F(i * 213) % F(j) == F((i * 213) % j));
				}
			}
		}
	}

	{
		using F = Signed_Integer<1, uint8>;
		PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

		CHECK(F(0) % -2 == F(0));
		CHECK(F(0) % -1 == F(0));
		CHECK(F(0) % 1 == F(0));
		CHECK(F(0) % 2 == F(0));

		CHECK(F(-1) % -2 == F(-1));
		CHECK(F(-1) % -1 == F(0));
		CHECK(F(-1) % 1 == F(0));
		CHECK(F(-1) % 2 == F(-1));
		CHECK(F(-1) % 3 == F(-1));
	}

	{
		using F = Signed_Integer<2, uint8>;
		PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

		CHECK(F(-2) % -4 == F(-2));
		CHECK(F(-1) % -4 == F(-1));
		CHECK(F(0) % -4 == F(0));
		CHECK(F(1) % -4 == F(1));

		CHECK(F(-2) % -3 == F(-2));
		CHECK(F(-1) % -3 == F(-1));
		CHECK(F(0) % -3 == F(0));
		CHECK(F(1) % -3 == F(1));

		CHECK(F(-2) % -2 == F(0));
		CHECK(F(-1) % -2 == F(-1));
		CHECK(F(0) % -2 == F(0));
		CHECK(F(1) % -2 == F(1));

		CHECK(F(-2) % -1 == F(0));
		CHECK(F(-1) % -1 == F(0));
		CHECK(F(0) % -1 == F(0));
		CHECK(F(1) % -1 == F(0));

		CHECK(F(-2) % 1 == F(0));
		CHECK(F(-1) % 1 == F(0));
		CHECK(F(0) % 1 == F(0));
		CHECK(F(1) % 1 == F(0));

		CHECK(F(-2) % 2 == F(0));
		CHECK(F(-1) % 2 == F(-1));
		CHECK(F(0) % 2 == F(0));
		CHECK(F(1) % 2 == F(1));

		CHECK(F(-2) % 3 == F(-2));
		CHECK(F(-1) % 3 == F(-1));
		CHECK(F(0) % 3 == F(0));
		CHECK(F(1) % 3 == F(1));

		CHECK(F(-2) % 4 == F(-2));
		CHECK(F(-1) % 4 == F(-1));
		CHECK(F(0) % 4 == F(0));
		CHECK(F(1) % 4 == F(1));
	}
	{
		using F = Unsigned_Integer<2, uint8>;
		PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

		CHECK(F(0) % 1 == F(0));
		CHECK(F(1) % 1 == F(0));
		CHECK(F(2) % 1 == F(0));
		CHECK(F(3) % 1 == F(0));

		CHECK(F(0) % 2 == F(0));
		CHECK(F(1) % 2 == F(1));
		CHECK(F(2) % 2 == F(0));
		CHECK(F(3) % 2 == F(1));

		CHECK(F(0) % 3 == F(0));
		CHECK(F(1) % 3 == F(1));
		CHECK(F(2) % 3 == F(2));
		CHECK(F(3) % 3 == F(0));

		CHECK(F(0) % 4 == F(0));
		CHECK(F(1) % 4 == F(1));
		CHECK(F(2) % 4 == F(2));
		CHECK(F(3) % 4 == F(3));
	}
}

TEST_CASE("NumberOfOneBits (MultiInteger)")
{
	using F = Unsigned_Integer<20, uint8>;
	PASTEL_CONCEPT_CHECK(F, Finite_Integer_Concept);

	auto test = [&](F that, integer correct)
	{
		return numberOfOneBits(that) == correct;
	};

	CHECK(test(0xFFF34567ul, 10));
	CHECK(test(0x00034567ul, 10));
}

