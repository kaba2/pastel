// Description: Testing for native integers
// DocumentationOf: native_integer.h

#include "test/test_init.h"

#include <pastel/sys/integer/finite_integer_concept.h>
#include <pastel/sys/math/number_tests.h>
#include <pastel/sys/integer/integer_mean.h>

namespace
{

	template <typename Type>
	void testComparison()
	{
		REQUIRE((negative((Type)-1) || std::is_unsigned<Type>::value));
		REQUIRE(!negative((Type)0));
		REQUIRE(positive((Type)1));
		REQUIRE(!positive((Type)0));
	}

	template <typename Type>
	void testIntegerMean()
	{
		Type min = std::is_unsigned<Type>::value ? 
			-16 : 0;

		for (Type i = min; i < 16; ++i)
		{
			for (Type j = min; j < 16;++j)
			{
				REQUIRE(integerMean(i, j) == (i + j) / 2);
			}				
		}

		REQUIRE(integerMean(infinity<Type>(), infinity<Type>()) == infinity<Type>());
		if (std::is_signed<Type>::value)
		{
			REQUIRE(integerMean(-infinity<Type>(), -infinity<Type>()) == -infinity<Type>());
		}
	}

	template <typename Type>
	void testEvenOdd()
	{
		for (integer i = 1;i < 100;i += 2)
		{
			REQUIRE(odd((Type)i));
		}

		for (integer i = 0;i < 100;i += 2)
		{
			REQUIRE(even((Type)i));
		}
	}

	template <typename Type>
	void testIsPowerOfTwo()
	{
		auto f = [](integer t)
		{
			return isPowerOfTwo((Type)t);
		};

		REQUIRE(f(1));
		REQUIRE(f(2));
		REQUIRE(!f(3));
		REQUIRE(f(4));
		REQUIRE(!f(5));
		REQUIRE(!f(6));
		REQUIRE(!f(7));
		REQUIRE(f(8));
		REQUIRE(!f(9));
		REQUIRE(!f(10));
		REQUIRE(!f(11));
		REQUIRE(!f(12));
		REQUIRE(!f(13));
		REQUIRE(!f(14));
		REQUIRE(!f(15));
		REQUIRE(f(16));
		REQUIRE(!f(17));
		REQUIRE(!f(18));
		REQUIRE(!f(19));
		REQUIRE(!f(20));
		REQUIRE(!f(21));
		REQUIRE(!f(22));
		REQUIRE(!f(23));
		REQUIRE(!f(24));
		REQUIRE(!f(25));
		REQUIRE(!f(26));
		REQUIRE(!f(27));
		REQUIRE(!f(28));
		REQUIRE(!f(29));
		REQUIRE(!f(30));
		REQUIRE(!f(31));
		REQUIRE(f(32));
		REQUIRE(f(64));
		REQUIRE(!f(127));
	}

	template <typename Type>
	void testMany()
	{
		PASTEL_CONCEPT_CHECK(Type, Finite_Integer_Concept);

		testComparison<Type>();
		testEvenOdd<Type>();
		testIsPowerOfTwo<Type>();
	}

}

TEST_CASE("various (native_integer)")
{
	//testMany<bool>();
	testMany<int8>();
	testMany<int16>();
	testMany<int32>();
	testMany<int64>();

	testMany<uint8>();
	testMany<uint16>();
	testMany<uint32>();
	testMany<uint64>();

	testMany<schar>();
	testMany<char>();
	testMany<short>();
	testMany<int>();
	testMany<long>();
	testMany<longlong>();

	testMany<uchar>();
	testMany<ushort>();
	testMany<uint>();
	testMany<ulong>();
	testMany<ulonglong>();

	testMany<integer>();
	testMany<uinteger>();
}

TEST_CASE("Infinity (native_integer)")
{
	REQUIRE(infinity<uint8>() == 0xFFu);
	REQUIRE(infinity<uint16>() == 0xFFFFu);
	REQUIRE(infinity<uint32>() == 0xFFFFFFFFul);
	REQUIRE(infinity<uint64>() == 0xFFFFFFFFFFFFFFFFull);

	REQUIRE(infinity<int8>() == 0x7Fu);
	REQUIRE(infinity<int16>() == 0x7FFFu);
	REQUIRE(infinity<int32>() == 0x7FFFFFFFul);
	REQUIRE(infinity<int64>() == 0x7FFFFFFFFFFFFFFFull);
}

