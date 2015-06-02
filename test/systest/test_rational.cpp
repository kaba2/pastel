// Description: Testing for Rational
// DocumentationOf: rational.h

#include "test_pastelsys.h"

#include "pastel/sys/rational.h"
#include "pastel/sys/integer/multi_integer.h"
#include "pastel/sys/math/mod.h"

#include <iostream>

namespace
{

	using namespace Pastel;

	//using Integer = Signed_Integer<32, uint8>;
	using Integer = integer;
	PASTEL_CONCEPT_CHECK(Integer, Integer_Concept);

	using Rat = Rational<Integer>;
	PASTEL_CONCEPT_CHECK(Rat, Real_Concept);

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
			testAsReal<float>();
			testAsReal<double>();
			testAsString();
			testClassify();
			testFloor();
			testCeil();
			testSyntax();
			testSimple();
			testSpecial();
			testAddition();
			testMultiply();
			testPower();
			testSimplestApproximation<float>();
			testSimplestApproximation<double>();
			testBestApproximation<float>();
			testBestApproximation<double>();
		}

		template <typename Real>
		void testAsReal()
		{
			TEST_ENSURE(Rat(1, 0).asReal<Real>() == infinity<Real>());
			TEST_ENSURE(Rat(-1, 0).asReal<Real>() == -infinity<Real>());
			TEST_ENSURE(isNan(Rat(0, 0).asReal<Real>()));
			TEST_ENSURE(Rat(1, 8).asReal<Real>() == (Real)1 / 8);
			TEST_ENSURE(Rat(22, 7).asReal<Real>() == (Real)22 / 7);
			TEST_ENSURE(Rat(355, 133).asReal<Real>() == (Real)355 / 133);
		}

		void testAsString()
		{
			TEST_ENSURE(Rat(1, 3).asStringRatio() == "1/3");
			TEST_ENSURE(Rat(-1, 3).asStringRatio() == "-1/3");
			TEST_ENSURE(Rat(1, -3).asStringRatio() == "-1/3");
			TEST_ENSURE(Rat(0, -3).asStringRatio() == "0");
			TEST_ENSURE(Rat(-3, 3).asStringRatio() == "-1");
			TEST_ENSURE(Rat(3, 3).asStringRatio() == "1");

			TEST_ENSURE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 0) == "1");
			TEST_ENSURE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 1) == "1.0");
			TEST_ENSURE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 2) == "1.00");
			TEST_ENSURE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 3) == "1.000");
			TEST_ENSURE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 4) == "0.9999");
			TEST_ENSURE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 5) == "0.9999");
			TEST_ENSURE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 6) == "0.9999");

			TEST_ENSURE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 0, PASTEL_TAG(rounding), Rounding::Truncate) == "0");
			TEST_ENSURE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 1, PASTEL_TAG(rounding), Rounding::Truncate) == "0.9");
			TEST_ENSURE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 2, PASTEL_TAG(rounding), Rounding::Truncate) == "0.99");
			TEST_ENSURE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 3, PASTEL_TAG(rounding), Rounding::Truncate) == "0.999");
			TEST_ENSURE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 4, PASTEL_TAG(rounding), Rounding::Truncate) == "0.9999");
			TEST_ENSURE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 5, PASTEL_TAG(rounding), Rounding::Truncate) == "0.9999");
			TEST_ENSURE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 6, PASTEL_TAG(rounding), Rounding::Truncate) == "0.9999");
																							
			TEST_ENSURE(Rat(-9999, 10000).asString(PASTEL_TAG(digits), 0) == "-1");
			TEST_ENSURE(Rat(-9999, 10000).asString(PASTEL_TAG(digits), 1) == "-1.0");
			TEST_ENSURE(Rat(-9999, 10000).asString(PASTEL_TAG(digits), 2) == "-1.00");
			TEST_ENSURE(Rat(-9999, 10000).asString(PASTEL_TAG(digits), 3) == "-1.000");
			TEST_ENSURE(Rat(-9999, 10000).asString(PASTEL_TAG(digits), 4) == "-0.9999");
			TEST_ENSURE(Rat(-9999, 10000).asString(PASTEL_TAG(digits), 5) == "-0.9999");
			TEST_ENSURE(Rat(-9999, 10000).asString(PASTEL_TAG(digits), 6) == "-0.9999");

			TEST_ENSURE(Rat(99999, 10000).asString(PASTEL_TAG(digits), 0) == "10");
			TEST_ENSURE(Rat(99999, 10000).asString(PASTEL_TAG(digits), 1) == "10.0");
			TEST_ENSURE(Rat(99999, 10000).asString(PASTEL_TAG(digits), 2) == "10.00");
			TEST_ENSURE(Rat(99999, 10000).asString(PASTEL_TAG(digits), 3) == "10.000");
			TEST_ENSURE(Rat(99999, 10000).asString(PASTEL_TAG(digits), 4) == "9.9999");
			TEST_ENSURE(Rat(99999, 10000).asString(PASTEL_TAG(digits), 5) == "9.9999");
			TEST_ENSURE(Rat(99999, 10000).asString(PASTEL_TAG(digits), 6) == "9.9999");

			TEST_ENSURE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 0, PASTEL_TAG(shortenExact), false) == "1");
			TEST_ENSURE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 1, PASTEL_TAG(shortenExact), false) == "1.0");
			TEST_ENSURE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 2, PASTEL_TAG(shortenExact), false) == "1.00");
			TEST_ENSURE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 3, PASTEL_TAG(shortenExact), false) == "1.000");
			TEST_ENSURE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 4, PASTEL_TAG(shortenExact), false) == "0.9999");
			TEST_ENSURE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 5, PASTEL_TAG(shortenExact), false) == "0.99990");
			TEST_ENSURE(Rat(9999, 10000).asString(PASTEL_TAG(digits), 6, PASTEL_TAG(shortenExact), false) == "0.999900");
			
			TEST_ENSURE(Rat(9995, 10000).asString(PASTEL_TAG(digits), 0) == "1");
			TEST_ENSURE(Rat(9995, 10000).asString(PASTEL_TAG(digits), 1) == "1.0");
			TEST_ENSURE(Rat(9995, 10000).asString(PASTEL_TAG(digits), 2) == "1.00");
			TEST_ENSURE(Rat(9995, 10000).asString(PASTEL_TAG(digits), 3) == "1.000");
			TEST_ENSURE(Rat(9995, 10000).asString(PASTEL_TAG(digits), 4) == "0.9995");
			TEST_ENSURE(Rat(9995, 10000).asString(PASTEL_TAG(digits), 5) == "0.9995");

			TEST_ENSURE(Rat(9994, 10000).asString(PASTEL_TAG(digits), 0) == "1");
			TEST_ENSURE(Rat(9994, 10000).asString(PASTEL_TAG(digits), 1) == "1.0");
			TEST_ENSURE(Rat(9994, 10000).asString(PASTEL_TAG(digits), 2) == "1.00");
			TEST_ENSURE(Rat(9994, 10000).asString(PASTEL_TAG(digits), 3) == "0.999");
			TEST_ENSURE(Rat(9994, 10000).asString(PASTEL_TAG(digits), 4) == "0.9994");
			TEST_ENSURE(Rat(9994, 10000).asString(PASTEL_TAG(digits), 5) == "0.9994");

			TEST_ENSURE(Rat(22, 7).asString(PASTEL_TAG(digits), 31) == "3.1428571428571428571428571428571");
			TEST_ENSURE(Rat(355, 113).asString(PASTEL_TAG(digits), 31) == "3.1415929203539823008849557522124");
			TEST_ENSURE(Rat(1, 3).asString() == "0.333");
			TEST_ENSURE(Rat(-1, 3).asString() == "-0.333");
			TEST_ENSURE(Rat(1, 3).asString(PASTEL_TAG(base), 3) == "0.1");
			TEST_ENSURE(Rat(2, 3).asString(PASTEL_TAG(base), 3) == "0.2");
			TEST_ENSURE(Rat(3, 3).asString(PASTEL_TAG(base), 3) == "1");
			TEST_ENSURE(Rat(-1, 3).asString(PASTEL_TAG(base), 3) == "-0.1");
			TEST_ENSURE(Rat(-2, 3).asString(PASTEL_TAG(base), 3) == "-0.2");
			TEST_ENSURE(Rat(-3, 3).asString(PASTEL_TAG(base), 3) == "-1");
			TEST_ENSURE(Rat(-1, 3).asString(PASTEL_TAG(base), 3, PASTEL_TAG(showBase), true) == "-0.1_3");
			TEST_ENSURE(Rat(-2, 3).asString(PASTEL_TAG(base), 3, PASTEL_TAG(showBase), true) == "-0.2_3");
			TEST_ENSURE(Rat(-3, 3).asString(PASTEL_TAG(base), 3, PASTEL_TAG(showBase), true) == "-1_3");
			TEST_ENSURE(Rat(-1, 3).asString(PASTEL_TAG(digits), 10, PASTEL_TAG(showBase), true) == "-0.3333333333_10");
		}

		void testClassify()
		{
			{
				TEST_ENSURE(!negative(-nan<Rat>()));
				TEST_ENSURE(negative(-infinity<Rat>()));
				TEST_ENSURE(negative(Rat(-2)));
				TEST_ENSURE(negative(Rat(-1)));
				TEST_ENSURE(!negative(Rat(0)));
				TEST_ENSURE(!negative(Rat(1)));
				TEST_ENSURE(!negative(Rat(2)));
				TEST_ENSURE(!negative(infinity<Rat>()));
				TEST_ENSURE(!negative(nan<Rat>()));
			}
			{
				TEST_ENSURE(!positive(-nan<Rat>()));
				TEST_ENSURE(!positive(-infinity<Rat>()));
				TEST_ENSURE(!positive(Rat(-2)));
				TEST_ENSURE(!positive(Rat(-1)));
				TEST_ENSURE(!positive(Rat(0)));
				TEST_ENSURE(positive(Rat(1)));
				TEST_ENSURE(positive(Rat(2)));
				TEST_ENSURE(positive(infinity<Rat>()));
				TEST_ENSURE(!positive(nan<Rat>()));
			}
			{
				TEST_ENSURE(!zero(-nan<Rat>()));
				TEST_ENSURE(!zero(-infinity<Rat>()));
				TEST_ENSURE(!zero(Rat(-2)));
				TEST_ENSURE(!zero(Rat(-1)));
				TEST_ENSURE(zero(Rat(0)));
				TEST_ENSURE(!zero(Rat(1)));
				TEST_ENSURE(!zero(Rat(2)));
				TEST_ENSURE(!zero(infinity<Rat>()));
				TEST_ENSURE(!zero(nan<Rat>()));
			}
			{
				TEST_ENSURE(!-nan<Rat>().isInfinity());
				TEST_ENSURE(!(-infinity<Rat>()).isInfinity());
				TEST_ENSURE(!Rat(-2).isInfinity());
				TEST_ENSURE(!Rat(-1).isInfinity());
				TEST_ENSURE(!Rat(0).isInfinity());
				TEST_ENSURE(!Rat(1).isInfinity());
				TEST_ENSURE(!Rat(2).isInfinity());
				TEST_ENSURE(infinity<Rat>().isInfinity());
				TEST_ENSURE(!nan<Rat>().isInfinity());
			}
			{
				TEST_ENSURE((-nan<Rat>()).isNan());
				TEST_ENSURE(!-infinity<Rat>().isNan());
				TEST_ENSURE(!Rat(-2).isNan());
				TEST_ENSURE(!Rat(-1).isNan());
				TEST_ENSURE(!Rat(0).isNan());
				TEST_ENSURE(!Rat(1).isNan());
				TEST_ENSURE(!Rat(2).isNan());
				TEST_ENSURE(nan<Rat>().isNan());
			}
			{
				TEST_ENSURE(!-nan<Rat>().isInteger());
				TEST_ENSURE(!-infinity<Rat>().isInteger());
				TEST_ENSURE(Rat(-2).isInteger());
				TEST_ENSURE(Rat(-1).isInteger());
				TEST_ENSURE(Rat(0).isInteger());
				TEST_ENSURE(Rat(1).isInteger());
				TEST_ENSURE(Rat(2).isInteger());
				TEST_ENSURE(!infinity<Rat>().isInteger());
				TEST_ENSURE(!nan<Rat>().isInteger());
				TEST_ENSURE(Rat(2, 2).isInteger());
			}
		}

		void testFloor()
		{
			TEST_ENSURE(floor(-infinity<Rat>()) == -infinity<Rat>());
			TEST_ENSURE(floor(Rat(-2)) == -2);
			TEST_ENSURE(floor(Rat(-1)) == -1);
			TEST_ENSURE(floor(Rat(0)) == 0);
			TEST_ENSURE(floor(Rat(1)) == 1);
			TEST_ENSURE(floor(Rat(2)) == 2);
			TEST_ENSURE(floor(infinity<Rat>()) == infinity<Rat>());

			TEST_ENSURE(floor(Rat(1, 2)) == 0);
			TEST_ENSURE(floor(Rat(1, 3)) == 0);
			TEST_ENSURE(floor(Rat(1, 4)) == 0);
			TEST_ENSURE(floor(Rat(1, 5)) == 0);

			TEST_ENSURE(floor(Rat(4, 1)) == 4);
			TEST_ENSURE(floor(Rat(4, 2)) == 2);
			TEST_ENSURE(floor(Rat(4, 3)) == 1);
			TEST_ENSURE(floor(Rat(4, 4)) == 1);

			TEST_ENSURE(floor(Rat(-4, 1)) == -4);
			TEST_ENSURE(floor(Rat(-4, 2)) == -2);
			TEST_ENSURE(floor(Rat(-4, 3)) == -2);
			TEST_ENSURE(floor(Rat(-4, 4)) == -1);
		}

		void testCeil()
		{
			TEST_ENSURE(ceil(-infinity<Rat>()) == -infinity<Rat>());
			TEST_ENSURE(ceil(Rat(-2)) == -2);
			TEST_ENSURE(ceil(Rat(-1)) == -1);
			TEST_ENSURE(ceil(Rat(0)) == 0);
			TEST_ENSURE(ceil(Rat(1)) == 1);
			TEST_ENSURE(ceil(Rat(2)) == 2);
			TEST_ENSURE(ceil(infinity<Rat>()) == infinity<Rat>());

			TEST_ENSURE(ceil(Rat(1, 2)) == 1);
			TEST_ENSURE(ceil(Rat(1, 3)) == 1);
			TEST_ENSURE(ceil(Rat(1, 4)) == 1);
			TEST_ENSURE(ceil(Rat(1, 5)) == 1);

			TEST_ENSURE(ceil(Rat(4, 1)) == 4);
			TEST_ENSURE(ceil(Rat(4, 2)) == 2);
			TEST_ENSURE(ceil(Rat(4, 3)) == 2);
			TEST_ENSURE(ceil(Rat(4, 4)) == 1);

			TEST_ENSURE(ceil(Rat(-4, 1)) == -4);
			TEST_ENSURE(ceil(Rat(-4, 2)) == -2);
			TEST_ENSURE(ceil(Rat(-4, 3)) == -1);
			TEST_ENSURE(ceil(Rat(-4, 4)) == -1);

			//TEST_ENSURE(mod(Rat(4, 3)) == Rat(1, 3));
		}

		void testSimple()
		{
			{
				Rat a;
				TEST_ENSURE(a == 0);
			}
			{
				TEST_ENSURE(Rat(1) == 1);
				TEST_ENSURE(Rat(2) == 2);
				TEST_ENSURE(Rat(1, 1) == 1);
				TEST_ENSURE(Rat(2, 2) == 1);
				TEST_ENSURE(Rat(4, 2) == 2);
			}
			{
				Rat a(0, 0);
				a.set(1, 5);
				TEST_ENSURE(a == Rat(1, 5));
			}
			{
				Rat a(5 * 1234235, 7 * 1234235);
				TEST_ENSURE(a == Rat(5, 7));
				TEST_ENSURE(a.m() == 5);
				TEST_ENSURE(a.n() == 7);
			}
			{
				Rat a(2, 5);
				Rat b(3, 5);
				Rat c(a - b);
				TEST_ENSURE(c == Rat(-1, 5));
			}
			{
				Rat a(2, 5);
				Rat b(3, 5);
				Rat c(a / b);
				TEST_ENSURE(c == Rat(2, 3));
			}
		}

		void testSpecial()
		{
			{
				Rat a(1, 0);
				TEST_ENSURE(a == infinity<Rat>());

				Rat b(2, 0);
				TEST_ENSURE(b == infinity<Rat>());

				Rat c(-1, 0);
				TEST_ENSURE(c == -infinity<Rat>());

				Rat d(-2, 0);
				TEST_ENSURE(d == -infinity<Rat>());
			}
			{
				TEST_ENSURE(infinity<Rat>() >= 23435);
				TEST_ENSURE(-2312 >= -infinity<Rat>());
				TEST_ENSURE(Integer(-2312) >= -infinity<Rat>());
				TEST_ENSURE(infinity<Rat>() >= -infinity<Rat>());
			}
		}

		void testAddition()
		{
			{
				TEST_ENSURE(Rat(2, 5) + Rat(3, 5) == Rat(1, 1));
				TEST_ENSURE(Rat(2, 5) + Rat(3, -5) == Rat(-1, 5));
				TEST_ENSURE(Rat(2, 5) + Rat(-3, 5) == Rat(-1, 5));
				TEST_ENSURE(Rat(2, 5) + Rat(-3, -5) == Rat(5, 5));

				TEST_ENSURE(Rat(2, -5) + Rat(3, 5) == Rat(1, 5));
				TEST_ENSURE(Rat(2, -5) + Rat(3, -5) == Rat(-1));
				TEST_ENSURE(Rat(2, -5) + Rat(-3, 5) == Rat(-1, 1));
				TEST_ENSURE(Rat(2, -5) + Rat(-3, -5) == Rat(1, 5));

				TEST_ENSURE(Rat(-2, 5) + Rat(3, 5) == Rat(1, 5));
				TEST_ENSURE(Rat(-2, 5) + Rat(3, -5) == Rat(-1));
				TEST_ENSURE(Rat(-2, 5) + Rat(-3, 5) == Rat(-1, 1));
				TEST_ENSURE(Rat(-2, 5) + Rat(-3, -5) == Rat(1, 5));

				TEST_ENSURE(Rat(-2, -5) + Rat(3, 5) == Rat(1, 1));
				TEST_ENSURE(Rat(-2, -5) + Rat(3, -5) == Rat(-1, 5));
				TEST_ENSURE(Rat(-2, -5) + Rat(-3, 5) == Rat(-1, 5));
				TEST_ENSURE(Rat(-2, -5) + Rat(-3, -5) == Rat(5, 5));
			}
			{
				// Adding with a NaN should give a NaN.

				TEST_ENSURE(Rat(0, 0) + Rat(1, 4) == Rat(0, 0));
				TEST_ENSURE(Rat(1, 5) + Rat(0, 0) == Rat(0, 0));

				TEST_ENSURE(Rat(0, 3) + Rat(0, 0) == Rat(0, 0));
				TEST_ENSURE(Rat(0, 0) + Rat(0, 3) == Rat(0, 0));

				TEST_ENSURE(Rat(15, 0) + Rat(0, 0) == Rat(0, 0));
				TEST_ENSURE(Rat(-15, 0) + Rat(0, 0) == Rat(0, 0));
			}
			{
				// Adding infinity and -infinity should give a NaN.

				TEST_ENSURE(Rat(-1, 0) + Rat(1, 0) == Rat(0, 0));
				TEST_ENSURE(Rat(1, 0) + Rat(-1, 0) == Rat(0, 0));
			}
		}

		void testMultiply()
		{
			{
				TEST_ENSURE(Rat(2, 5) * Rat(3, 5) == Rat(6, 25));
				TEST_ENSURE(Rat(2, 5) * Rat(3, -5) == Rat(-6, 25));
				TEST_ENSURE(Rat(2, 5) * Rat(-3, 5) == Rat(-6, 25));
				TEST_ENSURE(Rat(2, 5) * Rat(-3, -5) == Rat(6, 25));

				TEST_ENSURE(Rat(2, -5) * Rat(3, 5) == Rat(-6, 25));
				TEST_ENSURE(Rat(2, -5) * Rat(3, -5) == Rat(6, 25));
				TEST_ENSURE(Rat(2, -5) * Rat(-3, 5) == Rat(6, 25));
				TEST_ENSURE(Rat(2, -5) * Rat(-3, -5) == Rat(-6, 25));

				TEST_ENSURE(Rat(-2, 5) * Rat(3, 5) == Rat(-6, 25));
				TEST_ENSURE(Rat(-2, 5) * Rat(3, -5) == Rat(6, 25));
				TEST_ENSURE(Rat(-2, 5) * Rat(-3, 5) == Rat(6, 25));
				TEST_ENSURE(Rat(-2, 5) * Rat(-3, -5) == Rat(-6, 25));

				TEST_ENSURE(Rat(-2, -5) * Rat(3, 5) == Rat(6, 25));
				TEST_ENSURE(Rat(-2, -5) * Rat(3, -5) == Rat(-6, 25));
				TEST_ENSURE(Rat(-2, -5) * Rat(-3, 5) == Rat(-6, 25));
				TEST_ENSURE(Rat(-2, -5) * Rat(-3, -5) == Rat(6, 25));
			}
			{
				// Multiplication by zero should give a zero...

				TEST_ENSURE(Rat(0, 4) * Rat(0, 7) == 0);
				TEST_ENSURE(Rat(0, 4) * Rat(0, -7) == 0);
				TEST_ENSURE(Rat(0, -4) * Rat(0, 7) == 0);
				TEST_ENSURE(Rat(0, -4) * Rat(0, -7) == 0);

				// ..Except with NaNs a NaN:

				TEST_ENSURE(Rat(0, 4) * Rat(0, 0) == Rat(0, 0));
				TEST_ENSURE(Rat(0, 0) * Rat(0, 4) == Rat(0, 0));
				TEST_ENSURE(Rat(0, -4) * Rat(0, 0) == Rat(0, 0));
				TEST_ENSURE(Rat(0, 0) * Rat(0, -4) == Rat(0, 0));

				// Infinities with zeros should give zero:

				TEST_ENSURE(infinity<Rat>() * 0 == 0);
				TEST_ENSURE(-infinity<Rat>() * 0 == 0);

				// Infinities together should maintain
				// infinity.

				TEST_ENSURE(infinity<Rat>() * infinity<Rat>() ==
					infinity<Rat>());
				TEST_ENSURE(infinity<Rat>() * -infinity<Rat>() ==
					-infinity<Rat>());
				TEST_ENSURE(-infinity<Rat>() * infinity<Rat>() ==
					-infinity<Rat>());
				TEST_ENSURE(-infinity<Rat>() * -infinity<Rat>() ==
					infinity<Rat>());
			}
		}

		void testPower()
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
					TEST_ENSURE(pow(Rat(i), j) == naivePower(Rat(i), j));
				}
			}
		}

		template <typename Real>
		void testBestApproximation()
		{
			// Positive zero.
			TEST_ENSURE(Rat((Real)0) == 0);

			// Negative zero.
			TEST_ENSURE(Rat(-(Real)0) == 0);

			// Positive infinity.
			TEST_ENSURE(Rat(infinity<Real>()) == infinity<Rat>());

			// Negative infinity.
			TEST_ENSURE(Rat(-infinity<Real>()) == -infinity<Rat>());

			// Positive integers
			TEST_ENSURE(Rat((Real)1) == 1);
			TEST_ENSURE(Rat((Real)2) == 2);
			TEST_ENSURE(Rat((Real)3) == 3);
			TEST_ENSURE(Rat((Real)4) == 4);

			// Negative integers
			TEST_ENSURE(Rat((Real)-1) == -1);
			TEST_ENSURE(Rat((Real)-2) == -2);
			TEST_ENSURE(Rat((Real)-3) == -3);
			TEST_ENSURE(Rat((Real)-4) == -4);

			// Non-negative powers of two.
			for (integer i = 0;i < 31;++i)
			{
				TEST_ENSURE(Rat((Real)std::pow(2, i)) == Rat(1 << i));
			}

			// Negative powers of two.
			integer maxPower = std::is_same<Real, float>::value ? 9 : 18;
			for (integer i = 1;i < maxPower;++i)
			{
				TEST_ENSURE(Rat((Real)std::pow(2, -i)) == Rat(1, (1 << i)));
			}

			TEST_ENSURE(Rat((Real)std::pow(2, 20)) == Rat(1 << 20));

			TEST_ENSURE(Rat((Real)0.125, PASTEL_TAG(nMax), 10) == Rat(1, 8));
			TEST_ENSURE(Rat((Real)-0.125) == Rat(-1, 8));

			// The best rational approximations of pi.
			TEST_ENSURE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 1) == Rat(3, 1));
			TEST_ENSURE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 2) == Rat(6, 2));
			TEST_ENSURE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 3) == Rat(9, 3));
			TEST_ENSURE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 4) == Rat(13, 4));
			TEST_ENSURE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 5) == Rat(16, 5));
			TEST_ENSURE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 6) == Rat(19, 6));
			TEST_ENSURE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 7) == Rat(22, 7));
			TEST_ENSURE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 56) == Rat(22, 7));
			TEST_ENSURE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 57) == Rat(179, 57));
			TEST_ENSURE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 63) == Rat(179, 57));
			TEST_ENSURE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 64) == Rat(201, 64));
			TEST_ENSURE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 70) == Rat(201, 64));
			TEST_ENSURE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 71) == Rat(223, 71));
			TEST_ENSURE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 77) == Rat(223, 71));
			TEST_ENSURE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 78) == Rat(245, 78));
			TEST_ENSURE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 84) == Rat(245, 78));
			TEST_ENSURE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 85) == Rat(267, 85));
			TEST_ENSURE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 91) == Rat(267, 85));
			TEST_ENSURE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 92) == Rat(289, 92));
			TEST_ENSURE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 98) == Rat(289, 92));
			TEST_ENSURE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 99) == Rat(311, 99));
			TEST_ENSURE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 105) == Rat(311, 99));
			TEST_ENSURE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 106) == Rat(333, 106));
			TEST_ENSURE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 112) == Rat(333, 106));
			TEST_ENSURE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 113) == Rat(355, 113));
			TEST_ENSURE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 16603) == Rat(355, 113));

			if (std::is_same<Real, double>::value)
			{
				// The next rational approximation exceeds the precision of float.
				TEST_ENSURE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 16604) == Rat(52163, 16604));
				TEST_ENSURE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 16716) == Rat(52163, 16604));
				TEST_ENSURE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 16717) == Rat(52518, 16717));
				TEST_ENSURE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 16829) == Rat(52518, 16717));
				TEST_ENSURE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 16830) == Rat(52873, 16830));
				TEST_ENSURE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 16942) == Rat(52873, 16830));
				TEST_ENSURE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 16943) == Rat(53228, 16943));
				TEST_ENSURE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 17055) == Rat(53228, 16943));
				TEST_ENSURE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 17056) == Rat(53583, 17056));
				TEST_ENSURE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 17168) == Rat(53583, 17056));
				TEST_ENSURE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 17169) == Rat(53938, 17169));
				
				// Skipping approximations...

				TEST_ENSURE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 364913) == Rat(1146408, 364913));
				TEST_ENSURE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 995206) == Rat(1146408, 364913));
				// This is 11 correct digits.
				TEST_ENSURE(Rat(constantPi<Real>(), PASTEL_TAG(nMax), 995207) == Rat(3126535, 995207));
				TEST_ENSURE(Rat(-constantPi<Real>(), PASTEL_TAG(nMax), 995207) == Rat(-3126535, 995207));
			}
		}

		template <typename Real>
		void testSimplestApproximation()
		{
			// Positive zero.
			TEST_ENSURE(Rat((Real)0, Simplest()) == 0);

			// Negative zero.
			TEST_ENSURE(Rat(-(Real)0, Simplest()) == 0);

			// Positive infinity.
			TEST_ENSURE(Rat(infinity<Real>(), Simplest()) == infinity<Rat>());

			// Negative infinity.
			TEST_ENSURE(Rat(-infinity<Real>(), Simplest()) == -infinity<Rat>());

			// Positive integers
			TEST_ENSURE(Rat((Real)1, Simplest()) == 1);
			TEST_ENSURE(Rat((Real)2, Simplest()) == 2);
			TEST_ENSURE(Rat((Real)3, Simplest()) == 3);
			TEST_ENSURE(Rat((Real)4, Simplest()) == 4);

			// Negative integers
			TEST_ENSURE(Rat((Real)-1, Simplest()) == -1);
			TEST_ENSURE(Rat((Real)-2, Simplest()) == -2);
			TEST_ENSURE(Rat((Real)-3, Simplest()) == -3);
			TEST_ENSURE(Rat((Real)-4, Simplest()) == -4);

			TEST_ENSURE(Rat((Real)0.1, Simplest(), PASTEL_TAG(maxError), 0.05) == Rat(1, 7));
			
			// There result here depends on the form of the search-interval:
			// Closed interval: 2 / 17
			// Open interval: 1 / 8
			TEST_ENSURE(Rat((Real)0.12, Simplest(), PASTEL_TAG(maxError), 0.005) == Rat(1, 8));

			TEST_ENSURE(Rat((Real)0.15, Simplest(), PASTEL_TAG(maxError), 0.005) == Rat(2, 13));
			
			if (std::is_same<Real, double>::value)
			{
				// The accuracy of single-precision float falls short here.
				TEST_ENSURE(Rat((Real)0.111112, Simplest(), PASTEL_TAG(maxError), 0.0000005) == Rat(8889, 80000));
				TEST_ENSURE(Rat((Real)0.111125, Simplest(), PASTEL_TAG(maxError), 0.0000005) == Rat(859, 7730));
			}
		}

		void testSyntax()
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
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("Rational", test);
	}

	CallFunction run(addTest);

}
