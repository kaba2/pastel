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

	using Integer = Signed_Integer<64, uint8>;
	PASTEL_CONCEPT_CHECK(Integer, Integer_Concept);

	using Rat = Rational<Integer>;
	PASTEL_CONCEPT_CHECK_BASE(Rat, Real_Concept);

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
			testReal<float>();
			testReal<double>();
		}

		template <typename Real>
		void testAsReal()
		{
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

			TEST_ENSURE(Rat(22, 7).asString(10, 31) == "3.1428571428571428571428571428571");
			// FIX: The correctly rounded value ends at 4.
			TEST_ENSURE(Rat(355, 113).asString(10, 31) == "3.1415929203539823008849557522123");
			TEST_ENSURE(Rat(1, 3).asString() == "0.333");
			TEST_ENSURE(Rat(-1, 3).asString() == "-0.333");
			TEST_ENSURE(Rat(1, 3).asString(3) == "0.1");
			TEST_ENSURE(Rat(2, 3).asString(3) == "0.2");
			TEST_ENSURE(Rat(3, 3).asString(3) == "1");
			TEST_ENSURE(Rat(-1, 3).asString(3) == "-0.1");
			TEST_ENSURE(Rat(-2, 3).asString(3) == "-0.2");
			TEST_ENSURE(Rat(-3, 3).asString(3) == "-1");
			TEST_ENSURE(Rat(-1, 3).asString(3, 10, true) == "-0.1_3");
			TEST_ENSURE(Rat(-2, 3).asString(3, 10, true) == "-0.2_3");
			TEST_ENSURE(Rat(-3, 3).asString(3, 10, true) == "-1_3");
			TEST_ENSURE(Rat(-1, 3).asString(10, 10, true) == "-0.3333333333_10");
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

		template <typename Real>
		void testReal()
		{
			// Positive zero.
			TEST_ENSURE(Rat((Real)0) == 0);

			// Negative zero.
			TEST_ENSURE(Rat(-(Real)0) == 0);

			// Positive infinity.
			TEST_ENSURE(Rat(infinity<Real>()) == infinity<Rat>());

			// Negative infinity.
			TEST_ENSURE(Rat(-infinity<Real>()) == -infinity<Rat>());

			TEST_ENSURE(Rat((Real)0.125) == Rat(1, 8));
			TEST_ENSURE(Rat((Real)-0.125) == Rat(-1, 8));

			// The best rational approximations of pi.
			TEST_ENSURE(Rat(constantPi<Real>(), 1) == Rat(3, 1));
			TEST_ENSURE(Rat(constantPi<Real>(), 2) == Rat(6, 2));
			TEST_ENSURE(Rat(constantPi<Real>(), 3) == Rat(9, 3));
			TEST_ENSURE(Rat(constantPi<Real>(), 4) == Rat(13, 4));
			TEST_ENSURE(Rat(constantPi<Real>(), 5) == Rat(16, 5));
			TEST_ENSURE(Rat(constantPi<Real>(), 6) == Rat(19, 6));
			TEST_ENSURE(Rat(constantPi<Real>(), 7) == Rat(22, 7));
			TEST_ENSURE(Rat(constantPi<Real>(), 56) == Rat(22, 7));
			TEST_ENSURE(Rat(constantPi<Real>(), 57) == Rat(179, 57));
			TEST_ENSURE(Rat(constantPi<Real>(), 63) == Rat(179, 57));
			TEST_ENSURE(Rat(constantPi<Real>(), 64) == Rat(201, 64));
			TEST_ENSURE(Rat(constantPi<Real>(), 70) == Rat(201, 64));
			TEST_ENSURE(Rat(constantPi<Real>(), 71) == Rat(223, 71));
			TEST_ENSURE(Rat(constantPi<Real>(), 77) == Rat(223, 71));
			TEST_ENSURE(Rat(constantPi<Real>(), 78) == Rat(245, 78));
			TEST_ENSURE(Rat(constantPi<Real>(), 84) == Rat(245, 78));
			TEST_ENSURE(Rat(constantPi<Real>(), 85) == Rat(267, 85));
			TEST_ENSURE(Rat(constantPi<Real>(), 91) == Rat(267, 85));
			TEST_ENSURE(Rat(constantPi<Real>(), 92) == Rat(289, 92));
			TEST_ENSURE(Rat(constantPi<Real>(), 98) == Rat(289, 92));
			TEST_ENSURE(Rat(constantPi<Real>(), 99) == Rat(311, 99));
			TEST_ENSURE(Rat(constantPi<Real>(), 105) == Rat(311, 99));
			TEST_ENSURE(Rat(constantPi<Real>(), 106) == Rat(333, 106));
			TEST_ENSURE(Rat(constantPi<Real>(), 112) == Rat(333, 106));
			TEST_ENSURE(Rat(constantPi<Real>(), 113) == Rat(355, 113));
			TEST_ENSURE(Rat(constantPi<Real>(), 16603) == Rat(355, 113));

			if (std::is_same<Real, double>::value)
			{
				// The next rational approximation exceeds the precision of float.
				TEST_ENSURE(Rat(constantPi<Real>(), 16604) == Rat(52163, 16604));
				TEST_ENSURE(Rat(constantPi<Real>(), 16716) == Rat(52163, 16604));
				TEST_ENSURE(Rat(constantPi<Real>(), 16717) == Rat(52518, 16717));
				TEST_ENSURE(Rat(constantPi<Real>(), 16829) == Rat(52518, 16717));
				TEST_ENSURE(Rat(constantPi<Real>(), 16830) == Rat(52873, 16830));
				TEST_ENSURE(Rat(constantPi<Real>(), 16942) == Rat(52873, 16830));
				TEST_ENSURE(Rat(constantPi<Real>(), 16943) == Rat(53228, 16943));
				TEST_ENSURE(Rat(constantPi<Real>(), 17055) == Rat(53228, 16943));
				TEST_ENSURE(Rat(constantPi<Real>(), 17056) == Rat(53583, 17056));
				TEST_ENSURE(Rat(constantPi<Real>(), 17168) == Rat(53583, 17056));
				TEST_ENSURE(Rat(constantPi<Real>(), 17169) == Rat(53938, 17169));
				
				// Skipping approximations...

				TEST_ENSURE(Rat(constantPi<Real>(), 364913) == Rat(1146408, 364913));
				TEST_ENSURE(Rat(constantPi<Real>(), 995206) == Rat(1146408, 364913));
				// This is 11 correct digits.
				TEST_ENSURE(Rat(constantPi<Real>(), 995207) == Rat(3126535, 995207));
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
