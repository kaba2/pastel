// Description: Testing for infinity
// DocumentationOf: infinity.h

#include "test/test_init.h"

#include <pastel/sys/infinity.h>
#include <pastel/sys/integer/integer_concept.h>

#include <iostream>

template <typename Type>
void testNativeSigned()
{
	{
		Type a = Infinity();
		REQUIRE(positive(a));

		REQUIRE(EqualTo()(a, Infinity()));
		REQUIRE(EqualTo()(Infinity(), a));

		REQUIRE((Type)-Infinity() == -(Type)Infinity());
	}

	{
		Type a = -Infinity();
		REQUIRE(negative(a));

		REQUIRE(EqualTo()(a, -Infinity()));
		REQUIRE(EqualTo()(-Infinity(), a));

		REQUIRE((Type)-(-Infinity()) == (Type)Infinity());
	}
}

template <typename Type>
void testNativeSignedInteger()
{
	testNativeSigned<Type>();

	{
		Type a = Infinity();
		REQUIRE(positive(a));

		Type b = a;
		++b;
		REQUIRE(negative(b));
	}

	{
		Type a = -Infinity();
		REQUIRE(negative(a));

		Type b = a;
		--b;
		REQUIRE(positive(b));
	}
}

TEST_CASE("Native signed (infinity)")
{
	testNativeSignedInteger<char>();
	testNativeSignedInteger<int>();
	testNativeSignedInteger<long>();
	testNativeSignedInteger<longlong>();
}

template <typename Type>
void testNativeUnsignedInteger()
{
	Type a = Infinity();
	REQUIRE(positive(a));

	Type b = a;
	++b;
	REQUIRE(zero(b));
}

TEST_CASE("Native unsigned (infinity)")
{
	testNativeUnsignedInteger<uchar>();
	testNativeUnsignedInteger<uint>();
	testNativeUnsignedInteger<ulong>();
	testNativeUnsignedInteger<ulonglong>();
}

template <typename Type>
void testFloatingPoint()
{
	testNativeSigned<Type>();

	Type a = Infinity();

	REQUIRE(positive(a));

	REQUIRE(a == std::numeric_limits<Type>::infinity());

	--a;
	REQUIRE(a == std::numeric_limits<Type>::infinity());

	++a;
	REQUIRE(a == std::numeric_limits<Type>::infinity());

	a *= 2;
	REQUIRE(a == std::numeric_limits<Type>::infinity());

	a /= 2;
	REQUIRE(a == std::numeric_limits<Type>::infinity());
}

TEST_CASE("Floating point (infinity)")
{
	testFloatingPoint<float>();
	testFloatingPoint<double>();
}
