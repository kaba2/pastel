// Description: Testing for Rational
// DocumentationOf: rational.h

#include "test/test_init.h"

#include "pastel/sys/integer/multi_integer.h"
#include "pastel/sys/rational.h"
#include "pastel/sys/math/mod.h"

#include <iostream>

namespace
{

	using Integer = Signed_Integer<32, uint8>;
	//using Integer = integer;
	PASTEL_CONCEPT_CHECK(Integer, Integer_Concept);

	using Rat = Rational<Integer>;
	PASTEL_CONCEPT_CHECK(Rat, Real_Concept);

	template <typename Real>
	void testAsReal()
	{
		REQUIRE(Rat(1, 0).asReal<Real>() == (Real)Infinity());
		REQUIRE(Rat(-1, 0).asReal<Real>() == -(Real)Infinity());
		REQUIRE(isNan(Rat(0, 0).asReal<Real>()));
		REQUIRE(Rat(1, 8).asReal<Real>() == (Real)1 / 8);
		REQUIRE(Rat(22, 7).asReal<Real>() == (Real)22 / 7);
		REQUIRE(Rat(355, 133).asReal<Real>() == (Real)355 / 133);
	}

}

TEST_CASE("asReal (Rational)")
{
	testAsReal<float>();
	testAsReal<double>();
}

TEST_CASE("AsString (Rational)")
{
	REQUIRE(Rat(1, 3).asStringRatio() == "1/3");
	REQUIRE(Rat(-1, 3).asStringRatio() == "-1/3");
	REQUIRE(Rat(1, -3).asStringRatio() == "-1/3");
	REQUIRE(Rat(0, -3).asStringRatio() == "0");
	REQUIRE(Rat(-3, 3).asStringRatio() == "-1");
	REQUIRE(Rat(3, 3).asStringRatio() == "1");

	REQUIRE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 0) == "1");
	REQUIRE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 1) == "1.0");
	REQUIRE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 2) == "1.00");
	REQUIRE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 3) == "1.000");
	REQUIRE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 4) == "0.9999");
	REQUIRE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 5) == "0.9999");
	REQUIRE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 6) == "0.9999");

	REQUIRE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 0, PASTEL_TAG(rounding), Rounding::Truncate) == "0");
	REQUIRE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 1, PASTEL_TAG(rounding), Rounding::Truncate) == "0.9");
	REQUIRE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 2, PASTEL_TAG(rounding), Rounding::Truncate) == "0.99");
	REQUIRE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 3, PASTEL_TAG(rounding), Rounding::Truncate) == "0.999");
	REQUIRE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 4, PASTEL_TAG(rounding), Rounding::Truncate) == "0.9999");
	REQUIRE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 5, PASTEL_TAG(rounding), Rounding::Truncate) == "0.9999");
	REQUIRE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 6, PASTEL_TAG(rounding), Rounding::Truncate) == "0.9999");

	REQUIRE(Rat(-9999, 10000).asString(PASTEL_TAG(digits), 0) == "-1");
	REQUIRE(Rat(-9999, 10000).asString(PASTEL_TAG(digits), 1) == "-1.0");
	REQUIRE(Rat(-9999, 10000).asString(PASTEL_TAG(digits), 2) == "-1.00");
	REQUIRE(Rat(-9999, 10000).asString(PASTEL_TAG(digits), 3) == "-1.000");
	REQUIRE(Rat(-9999, 10000).asString(PASTEL_TAG(digits), 4) == "-0.9999");
	REQUIRE(Rat(-9999, 10000).asString(PASTEL_TAG(digits), 5) == "-0.9999");
	REQUIRE(Rat(-9999, 10000).asString(PASTEL_TAG(digits), 6) == "-0.9999");

	REQUIRE(Rat(99999, 10000).asString(PASTEL_TAG(digits), 0) == "10");
	REQUIRE(Rat(99999, 10000).asString(PASTEL_TAG(digits), 1) == "10.0");
	REQUIRE(Rat(99999, 10000).asString(PASTEL_TAG(digits), 2) == "10.00");
	REQUIRE(Rat(99999, 10000).asString(PASTEL_TAG(digits), 3) == "10.000");
	REQUIRE(Rat(99999, 10000).asString(PASTEL_TAG(digits), 4) == "9.9999");
	REQUIRE(Rat(99999, 10000).asString(PASTEL_TAG(digits), 5) == "9.9999");
	REQUIRE(Rat(99999, 10000).asString(PASTEL_TAG(digits), 6) == "9.9999");

	REQUIRE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 0, PASTEL_TAG(shortenExact), false) == "1");
	REQUIRE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 1, PASTEL_TAG(shortenExact), false) == "1.0");
	REQUIRE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 2, PASTEL_TAG(shortenExact), false) == "1.00");
	REQUIRE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 3, PASTEL_TAG(shortenExact), false) == "1.000");
	REQUIRE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 4, PASTEL_TAG(shortenExact), false) == "0.9999");
	REQUIRE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 5, PASTEL_TAG(shortenExact), false) == "0.99990");
	REQUIRE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 6, PASTEL_TAG(shortenExact), false) == "0.999900");

	REQUIRE(Rat(9995, 10000).asString(PASTEL_TAG(digits), 0) == "1");
	REQUIRE(Rat(9995, 10000).asString(PASTEL_TAG(digits), 1) == "1.0");
	REQUIRE(Rat(9995, 10000).asString(PASTEL_TAG(digits), 2) == "1.00");
	REQUIRE(Rat(9995, 10000).asString(PASTEL_TAG(digits), 3) == "1.000");
	REQUIRE(Rat(9995, 10000).asString(PASTEL_TAG(digits), 4) == "0.9995");
	REQUIRE(Rat(9995, 10000).asString(PASTEL_TAG(digits), 5) == "0.9995");

	REQUIRE(Rat(9994, 10000).asString(PASTEL_TAG(digits), 0) == "1");
	REQUIRE(Rat(9994, 10000).asString(PASTEL_TAG(digits), 1) == "1.0");
	REQUIRE(Rat(9994, 10000).asString(PASTEL_TAG(digits), 2) == "1.00");
	REQUIRE(Rat(9994, 10000).asString(PASTEL_TAG(digits), 3) == "0.999");
	REQUIRE(Rat(9994, 10000).asString(PASTEL_TAG(digits), 4) == "0.9994");
	REQUIRE(Rat(9994, 10000).asString(PASTEL_TAG(digits), 5) == "0.9994");

	REQUIRE(Rat(22, 7).asString(PASTEL_TAG(digits), 31) == "3.1428571428571428571428571428571");
	REQUIRE(Rat(355, 113).asString(PASTEL_TAG(digits), 31) == "3.1415929203539823008849557522124");
	REQUIRE(Rat(1, 3).asString() == "0.333");
	REQUIRE(Rat(-1, 3).asString() == "-0.333");
	REQUIRE(Rat(1, 3).asString(PASTEL_TAG(base), 3) == "0.1");
	REQUIRE(Rat(2, 3).asString(PASTEL_TAG(base), 3) == "0.2");
	REQUIRE(Rat(3, 3).asString(PASTEL_TAG(base), 3) == "1");
	REQUIRE(Rat(-1, 3).asString(PASTEL_TAG(base), 3) == "-0.1");
	REQUIRE(Rat(-2, 3).asString(PASTEL_TAG(base), 3) == "-0.2");
	REQUIRE(Rat(-3, 3).asString(PASTEL_TAG(base), 3) == "-1");
	REQUIRE(Rat(-1, 3).asString(PASTEL_TAG(base), 3, PASTEL_TAG(showBase), true) == "-0.1_3");
	REQUIRE(Rat(-2, 3).asString(PASTEL_TAG(base), 3, PASTEL_TAG(showBase), true) == "-0.2_3");
	REQUIRE(Rat(-3, 3).asString(PASTEL_TAG(base), 3, PASTEL_TAG(showBase), true) == "-1_3");
	REQUIRE(Rat(-1, 3).asString(PASTEL_TAG(digits), 10, PASTEL_TAG(showBase), true) == "-0.3333333333_10");
}

TEST_CASE("Classify (Rational)")
{
	{
		REQUIRE(!negative(-nan<Rat>()));
		REQUIRE(negative(-(Rat)Infinity()));
		REQUIRE(negative(Rat(-2)));
		REQUIRE(negative(Rat(-1)));
		REQUIRE(!negative(Rat(0)));
		REQUIRE(!negative(Rat(1)));
		REQUIRE(!negative(Rat(2)));
		REQUIRE(!negative((Rat)Infinity()));
		REQUIRE(!negative(nan<Rat>()));
	}
	{
		REQUIRE(!positive(-nan<Rat>()));
		REQUIRE(!positive(-(Rat)Infinity()));
		REQUIRE(!positive(Rat(-2)));
		REQUIRE(!positive(Rat(-1)));
		REQUIRE(!positive(Rat(0)));
		REQUIRE(positive(Rat(1)));
		REQUIRE(positive(Rat(2)));
		REQUIRE(positive((Rat)Infinity()));
		REQUIRE(!positive(nan<Rat>()));
	}
	{
		REQUIRE(!zero(-nan<Rat>()));
		REQUIRE(!zero(-(Rat)Infinity()));
		REQUIRE(!zero(Rat(-2)));
		REQUIRE(!zero(Rat(-1)));
		REQUIRE(zero(Rat(0)));
		REQUIRE(!zero(Rat(1)));
		REQUIRE(!zero(Rat(2)));
		REQUIRE(!zero((Rat)Infinity()));
		REQUIRE(!zero(nan<Rat>()));
	}
	{
		REQUIRE(!-nan<Rat>().isInfinity());
		REQUIRE(!(-(Rat)Infinity()).isInfinity());
		REQUIRE(!Rat(-2).isInfinity());
		REQUIRE(!Rat(-1).isInfinity());
		REQUIRE(!Rat(0).isInfinity());
		REQUIRE(!Rat(1).isInfinity());
		REQUIRE(!Rat(2).isInfinity());
		REQUIRE(((Rat)Infinity()).isInfinity());
		REQUIRE(!nan<Rat>().isInfinity());
	}
	{
		REQUIRE((-nan<Rat>()).isNan());
		REQUIRE(!(-(Rat)Infinity()).isNan());
		REQUIRE(!Rat(-2).isNan());
		REQUIRE(!Rat(-1).isNan());
		REQUIRE(!Rat(0).isNan());
		REQUIRE(!Rat(1).isNan());
		REQUIRE(!Rat(2).isNan());
		REQUIRE(nan<Rat>().isNan());
	}
	{
		REQUIRE(!-nan<Rat>().isInteger());
		REQUIRE(!(-(Rat)Infinity()).isInteger());
		REQUIRE(Rat(-2).isInteger());
		REQUIRE(Rat(-1).isInteger());
		REQUIRE(Rat(0).isInteger());
		REQUIRE(Rat(1).isInteger());
		REQUIRE(Rat(2).isInteger());
		REQUIRE(!((Rat)Infinity()).isInteger());
		REQUIRE(!nan<Rat>().isInteger());
		REQUIRE(Rat(2, 2).isInteger());
	}
}

TEST_CASE("Floor (Rational)")
{
	REQUIRE(floor(-(Rat)Infinity()) == -(Rat)Infinity());
	REQUIRE(floor(Rat(-2)) == -2);
	REQUIRE(floor(Rat(-1)) == -1);
	REQUIRE(floor(Rat(0)) == 0);
	REQUIRE(floor(Rat(1)) == 1);
	REQUIRE(floor(Rat(2)) == 2);
	REQUIRE(floor((Rat)Infinity()) == (Rat)Infinity());

	REQUIRE(floor(Rat(1, 2)) == 0);
	REQUIRE(floor(Rat(1, 3)) == 0);
	REQUIRE(floor(Rat(1, 4)) == 0);
	REQUIRE(floor(Rat(1, 5)) == 0);

	REQUIRE(floor(Rat(4, 1)) == 4);
	REQUIRE(floor(Rat(4, 2)) == 2);
	REQUIRE(floor(Rat(4, 3)) == 1);
	REQUIRE(floor(Rat(4, 4)) == 1);

	REQUIRE(floor(Rat(-4, 1)) == -4);
	REQUIRE(floor(Rat(-4, 2)) == -2);
	REQUIRE(floor(Rat(-4, 3)) == -2);
	REQUIRE(floor(Rat(-4, 4)) == -1);
}

TEST_CASE("Ceil (Rational)")
{
	REQUIRE(ceil(-(Rat)Infinity()) == -(Rat)Infinity());
	REQUIRE(ceil(Rat(-2)) == -2);
	REQUIRE(ceil(Rat(-1)) == -1);
	REQUIRE(ceil(Rat(0)) == 0);
	REQUIRE(ceil(Rat(1)) == 1);
	REQUIRE(ceil(Rat(2)) == 2);
	REQUIRE(ceil((Rat)Infinity()) == (Rat)Infinity());

	REQUIRE(ceil(Rat(1, 2)) == 1);
	REQUIRE(ceil(Rat(1, 3)) == 1);
	REQUIRE(ceil(Rat(1, 4)) == 1);
	REQUIRE(ceil(Rat(1, 5)) == 1);

	REQUIRE(ceil(Rat(4, 1)) == 4);
	REQUIRE(ceil(Rat(4, 2)) == 2);
	REQUIRE(ceil(Rat(4, 3)) == 2);
	REQUIRE(ceil(Rat(4, 4)) == 1);

	REQUIRE(ceil(Rat(-4, 1)) == -4);
	REQUIRE(ceil(Rat(-4, 2)) == -2);
	REQUIRE(ceil(Rat(-4, 3)) == -1);
	REQUIRE(ceil(Rat(-4, 4)) == -1);

	//REQUIRE(mod(Rat(4, 3)) == Rat(1, 3));
}

TEST_CASE("Simple (Rational)")
{
	{
		Rat a;
		REQUIRE(a == 0);
	}
	{
		REQUIRE(Rat(1) == 1);
		REQUIRE(Rat(2) == 2);
		REQUIRE(Rat(1, 1) == 1);
		REQUIRE(Rat(2, 2) == 1);
		REQUIRE(Rat(4, 2) == 2);
	}
	{
		Rat a(0, 0);
		a.set(1, 5);
		REQUIRE(a == Rat(1, 5));
	}
	{
		Rat a(5 * 1234235, 7 * 1234235);
		REQUIRE(a == Rat(5, 7));
		REQUIRE(a.m() == 5);
		REQUIRE(a.n() == 7);
	}
	{
		Rat a(2, 5);
		Rat b(3, 5);
		Rat c(a - b);
		REQUIRE(c == Rat(-1, 5));
	}
	{
		Rat a(2, 5);
		Rat b(3, 5);
		Rat c(a / b);
		REQUIRE(c == Rat(2, 3));
	}
}

TEST_CASE("Special (Rational)")
{
	{
		Rat a(1, 0);
		REQUIRE(a == (Rat)Infinity());

		Rat b(2, 0);
		REQUIRE(b == (Rat)Infinity());

		Rat c(-1, 0);
		REQUIRE(c == -(Rat)Infinity());

		Rat d(-2, 0);
		REQUIRE(d == -(Rat)Infinity());
	}
	{
		REQUIRE((Rat)Infinity() >= 23435);
		REQUIRE(-2312 >= -(Rat)Infinity());
		REQUIRE(Integer(-2312) >= -(Rat)Infinity());
		REQUIRE((Rat)Infinity() >= -(Rat)Infinity());
	}
}

TEST_CASE("Addition (Rational)")
{
	{
		REQUIRE(Rat(2, 5) + Rat(3, 5) == Rat(1, 1));
		REQUIRE(Rat(2, 5) + Rat(3, -5) == Rat(-1, 5));
		REQUIRE(Rat(2, 5) + Rat(-3, 5) == Rat(-1, 5));
		REQUIRE(Rat(2, 5) + Rat(-3, -5) == Rat(5, 5));

		REQUIRE(Rat(2, -5) + Rat(3, 5) == Rat(1, 5));
		REQUIRE(Rat(2, -5) + Rat(3, -5) == Rat(-1));
		REQUIRE(Rat(2, -5) + Rat(-3, 5) == Rat(-1, 1));
		REQUIRE(Rat(2, -5) + Rat(-3, -5) == Rat(1, 5));

		REQUIRE(Rat(-2, 5) + Rat(3, 5) == Rat(1, 5));
		REQUIRE(Rat(-2, 5) + Rat(3, -5) == Rat(-1));
		REQUIRE(Rat(-2, 5) + Rat(-3, 5) == Rat(-1, 1));
		REQUIRE(Rat(-2, 5) + Rat(-3, -5) == Rat(1, 5));

		REQUIRE(Rat(-2, -5) + Rat(3, 5) == Rat(1, 1));
		REQUIRE(Rat(-2, -5) + Rat(3, -5) == Rat(-1, 5));
		REQUIRE(Rat(-2, -5) + Rat(-3, 5) == Rat(-1, 5));
		REQUIRE(Rat(-2, -5) + Rat(-3, -5) == Rat(5, 5));
	}
	{
		// Adding with a NaN should give a NaN.

		REQUIRE(Rat(0, 0) + Rat(1, 4) == Rat(0, 0));
		REQUIRE(Rat(1, 5) + Rat(0, 0) == Rat(0, 0));

		REQUIRE(Rat(0, 3) + Rat(0, 0) == Rat(0, 0));
		REQUIRE(Rat(0, 0) + Rat(0, 3) == Rat(0, 0));

		REQUIRE(Rat(15, 0) + Rat(0, 0) == Rat(0, 0));
		REQUIRE(Rat(-15, 0) + Rat(0, 0) == Rat(0, 0));
	}
	{
		// Adding infinity and -infinity should give a NaN.

		REQUIRE(Rat(-1, 0) + Rat(1, 0) == Rat(0, 0));
		REQUIRE(Rat(1, 0) + Rat(-1, 0) == Rat(0, 0));
	}
}

TEST_CASE("Multiply (Rational)")
{
	{
		REQUIRE(Rat(2, 5) * Rat(3, 5) == Rat(6, 25));
		REQUIRE(Rat(2, 5) * Rat(3, -5) == Rat(-6, 25));
		REQUIRE(Rat(2, 5) * Rat(-3, 5) == Rat(-6, 25));
		REQUIRE(Rat(2, 5) * Rat(-3, -5) == Rat(6, 25));

		REQUIRE(Rat(2, -5) * Rat(3, 5) == Rat(-6, 25));
		REQUIRE(Rat(2, -5) * Rat(3, -5) == Rat(6, 25));
		REQUIRE(Rat(2, -5) * Rat(-3, 5) == Rat(6, 25));
		REQUIRE(Rat(2, -5) * Rat(-3, -5) == Rat(-6, 25));

		REQUIRE(Rat(-2, 5) * Rat(3, 5) == Rat(-6, 25));
		REQUIRE(Rat(-2, 5) * Rat(3, -5) == Rat(6, 25));
		REQUIRE(Rat(-2, 5) * Rat(-3, 5) == Rat(6, 25));
		REQUIRE(Rat(-2, 5) * Rat(-3, -5) == Rat(-6, 25));

		REQUIRE(Rat(-2, -5) * Rat(3, 5) == Rat(6, 25));
		REQUIRE(Rat(-2, -5) * Rat(3, -5) == Rat(-6, 25));
		REQUIRE(Rat(-2, -5) * Rat(-3, 5) == Rat(-6, 25));
		REQUIRE(Rat(-2, -5) * Rat(-3, -5) == Rat(6, 25));
	}
	{
		// Multiplication by zero should give a zero...

		REQUIRE(Rat(0, 4) * Rat(0, 7) == 0);
		REQUIRE(Rat(0, 4) * Rat(0, -7) == 0);
		REQUIRE(Rat(0, -4) * Rat(0, 7) == 0);
		REQUIRE(Rat(0, -4) * Rat(0, -7) == 0);

		// ..Except with NaNs a NaN:

		REQUIRE(Rat(0, 4) * Rat(0, 0) == Rat(0, 0));
		REQUIRE(Rat(0, 0) * Rat(0, 4) == Rat(0, 0));
		REQUIRE(Rat(0, -4) * Rat(0, 0) == Rat(0, 0));
		REQUIRE(Rat(0, 0) * Rat(0, -4) == Rat(0, 0));

		// Infinities with zeros should give zero:

		REQUIRE((Rat)Infinity() * 0 == 0);
		REQUIRE(-(Rat)Infinity() * 0 == 0);

		// Infinities together should maintain
		// infinity.

		REQUIRE((Rat)Infinity() * (Rat)Infinity() ==
			(Rat)Infinity());
		REQUIRE((Rat)Infinity() * -(Rat)Infinity() ==
			-(Rat)Infinity());
		REQUIRE(-(Rat)Infinity() * (Rat)Infinity() ==
			-(Rat)Infinity());
		REQUIRE(-(Rat)Infinity() * -(Rat)Infinity() ==
			(Rat)Infinity());
	}
}

TEST_CASE("Power (Rational)")
{
	auto naivePower = [](Rat x, integer p)
	{
		Rat result = 1;
		for (integer i = 0;i < abs(p);++i)
		{
			result *= x;
		}

		return (p < 0) ? inverse(result) : result;
	};

	for (integer i = -16;i < 16;++i)
	{
		for (integer j = -7; j < 8;++j)
		{
			REQUIRE(pow(Rat(i), j) == naivePower(Rat(i), j));
		}
	}
}

namespace
{
		
	template <typename Real>
	void testBestApproximation()
	{
		// Positive zero.
		REQUIRE(Rat((Real)0) == 0);

		// Negative zero.
		REQUIRE(Rat(-(Real)0) == 0);

		// Positive infinity.
		REQUIRE(Rat((Real)Infinity()) == (Rat)Infinity());

		// Negative infinity.
		REQUIRE(Rat(-(Real)Infinity()) == -(Rat)Infinity());

		// Positive integers
		REQUIRE(Rat((Real)1) == 1);
		REQUIRE(Rat((Real)2) == 2);
		REQUIRE(Rat((Real)3) == 3);
		REQUIRE(Rat((Real)4) == 4);

		// Negative integers
		REQUIRE(Rat((Real)-1) == -1);
		REQUIRE(Rat((Real)-2) == -2);
		REQUIRE(Rat((Real)-3) == -3);
		REQUIRE(Rat((Real)-4) == -4);

		// Non-negative powers of two.
		for (integer i = 0;i < 31;++i)
		{
			REQUIRE(Rat((Real)std::pow(2, i)) == Rat(1 << i));
		}

		// Negative powers of two.
		integer maxPower = std::is_same<Real, float>::value ? 9 : 18;
		for (integer i = 1;i < maxPower;++i)
		{
			REQUIRE(Rat((Real)std::pow(2, -i)) == Rat(1, (1 << i)));
		}

		REQUIRE(Rat((Real)std::pow(2, 20)) == Rat(1 << 20));

		REQUIRE(Rat((Real)0.125, PASTEL_TAG(nMax), 10) == Rat(1, 8));
		REQUIRE(Rat((Real)-0.125) == Rat(-1, 8));

		// The best rational approximations of pi.
		REQUIRE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 1) == Rat(3, 1));
		REQUIRE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 2) == Rat(6, 2));
		REQUIRE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 3) == Rat(9, 3));
		REQUIRE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 4) == Rat(13, 4));
		REQUIRE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 5) == Rat(16, 5));
		REQUIRE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 6) == Rat(19, 6));
		REQUIRE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 7) == Rat(22, 7));
		REQUIRE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 56) == Rat(22, 7));
		REQUIRE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 57) == Rat(179, 57));
		REQUIRE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 63) == Rat(179, 57));
		REQUIRE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 64) == Rat(201, 64));
		REQUIRE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 70) == Rat(201, 64));
		REQUIRE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 71) == Rat(223, 71));
		REQUIRE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 77) == Rat(223, 71));
		REQUIRE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 78) == Rat(245, 78));
		REQUIRE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 84) == Rat(245, 78));
		REQUIRE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 85) == Rat(267, 85));
		REQUIRE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 91) == Rat(267, 85));
		REQUIRE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 92) == Rat(289, 92));
		REQUIRE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 98) == Rat(289, 92));
		REQUIRE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 99) == Rat(311, 99));
		REQUIRE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 105) == Rat(311, 99));
		REQUIRE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 106) == Rat(333, 106));
		REQUIRE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 112) == Rat(333, 106));
		REQUIRE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 113) == Rat(355, 113));
		REQUIRE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 16603) == Rat(355, 113));

		if (std::is_same<Real, double>::value)
		{
			// The next rational approximation exceeds the precision of float.
			REQUIRE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 16604) == Rat(52163, 16604));
			REQUIRE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 16716) == Rat(52163, 16604));
			REQUIRE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 16717) == Rat(52518, 16717));
			REQUIRE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 16829) == Rat(52518, 16717));
			REQUIRE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 16830) == Rat(52873, 16830));
			REQUIRE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 16942) == Rat(52873, 16830));
			REQUIRE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 16943) == Rat(53228, 16943));
			REQUIRE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 17055) == Rat(53228, 16943));
			REQUIRE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 17056) == Rat(53583, 17056));
			REQUIRE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 17168) == Rat(53583, 17056));
			REQUIRE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 17169) == Rat(53938, 17169));

			// Skipping approximations...

			REQUIRE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 364913) == Rat(1146408, 364913));
			REQUIRE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 995206) == Rat(1146408, 364913));
			// This is 11 correct digits.
			REQUIRE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 995207) == Rat(3126535, 995207));
			REQUIRE(Rat(-constantPi<Real>(), PASTEL_TAG(nMax), 995207) == Rat(-3126535, 995207));
		}
	}

	template <typename Real>
	void testSimplestApproximation()
	{
		// Positive zero.
		REQUIRE(Rat((Real)0) == 0);

		// Negative zero.
		REQUIRE(Rat(-(Real)0) == 0);

		// Positive infinity.
		REQUIRE(Rat((Real)Infinity()) == (Rat)Infinity());

		// Negative infinity.
		REQUIRE(Rat(-(Real)Infinity()) == -(Rat)Infinity());

		// Positive integers
		REQUIRE(Rat((Real)1) == 1);
		REQUIRE(Rat((Real)2) == 2);
		REQUIRE(Rat((Real)3) == 3);
		REQUIRE(Rat((Real)4) == 4);

		// Negative integers
		REQUIRE(Rat((Real)-1) == -1);
		REQUIRE(Rat((Real)-2) == -2);
		REQUIRE(Rat((Real)-3) == -3);
		REQUIRE(Rat((Real)-4) == -4);

		REQUIRE(Rat((Real)0.1, PASTEL_TAG(maxError), 0.05) == Rat(1, 7));

		// The result here depends on the form of the search-interval:
		// Closed interval: 2 / 17
		// Open interval: 1 / 8
		REQUIRE(Rat((Real)0.12, PASTEL_TAG(maxError), 0.005) == Rat(1, 8));

		REQUIRE(Rat((Real)0.15, PASTEL_TAG(maxError), 0.005) == Rat(2, 13));

		if (std::is_same<Real, double>::value)
		{
			// The accuracy of single-precision float falls short here.
			REQUIRE(Rat((Real)0.111112, PASTEL_TAG(maxError), 0.0000005) == Rat(8889, 80000));
			REQUIRE(Rat((Real)0.111125, PASTEL_TAG(maxError), 0.0000005) == Rat(859, 7730));
		}
	}

}

TEST_CASE("SimplestApproximation (Rational)")
{
	testSimplestApproximation<float>();
	testSimplestApproximation<double>();
}

TEST_CASE("Syntax (Rational)")
{
	Rat a;
	Rat b(1, 1);

	a * b;
	a * Integer(1);

	a < b;
	a > b;
	a <= b;
	a >= b;
	a == b;
	a != b;

	Integer i = 0;

	a += i;
	a -= i;
	a *= i;
	a /= i;
	a + i;
	a - i;
	a * i;
	a / i;
	i + a;
	i - a;
	i * a;
	i / a;

	a += 1;
	a -= 1;
	a *= 1;
	a /= 1;
	a + 1;
	a - 1;
	a * 1;
	a / 1;
	1 + a;
	1 - a;
	1 * a;
	1 / a;

	a == 1;
	a != 1;
	a == 0.15f;

	a == i;
	a != i;

	a < 1;
	a > 1;
	a <= i;
	a >= i;

	a < i;
	a > i;
	a <= i;
	a >= i;

	1 == a;
	0.15f == a;
	1 != a;
	1 < a;
	1 > a;
	1 <= a;
	1 >= a;

	i == a;
	i != a;
	i < a;
	i > a;
	i <= a;
	i >= a;
}

TEST_CASE("BestApproximation (Rational)")
{
	testBestApproximation<float>();
	testBestApproximation<double>();
}
