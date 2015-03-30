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
			testFloat();
			testDouble();
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

		void testFloat()
		{
			TEST_ENSURE(Rat(0.125f) == Rat(1, 8));
			TEST_ENSURE(Rat(-0.125f) == Rat(-1, 8));
			TEST_ENSURE(Rat(0.0f) == Rat(0));
			TEST_ENSURE(Rat(-0.0f) == Rat(0));

			real32_ieee number = 0;
			uint32& bits = *((uint32*)&number);

			// Positive zero.
			bits = (0x0 << 31) + (0x00 << 23) + (0x000000);
			TEST_ENSURE(Rat(number) == 0);

			// Negative zero.
			bits = (0x1 << 31) + (0x00 << 23) + (0x000000);
			TEST_ENSURE(Rat(number) == 0);

			// Positive infinity.
			bits = (0x0 << 31) + (0xFF << 23) + (0x000000);
			TEST_ENSURE(Rat(number) == infinity<Rat>());

			// Negative infinity.
			bits = (0x1 << 31) + (0xFF << 23) + (0x000000);
			TEST_ENSURE(Rat(number) == -infinity<Rat>());

			bits = (0x0 << 31) + (0x7F << 23) + (0x123456);
			TEST_ENSURE(Rat(number) == Rat(0x123456 + 0x800000, 1 << 23));
		}

		void testDouble()
		{
			Rat a(0.125);

			TEST_ENSURE(Rat(0.125) == Rat(1, 8));
			TEST_ENSURE(Rat(-0.125) == Rat(-1, 8));
			TEST_ENSURE(Rat(0.0) == Rat(0));
			TEST_ENSURE(Rat(-0.0) == Rat(0));

			real64_ieee number = 0;
			uint64& bits = *((uint64*)&number);

			// Positive zero.
			bits = ((uint64)0x0 << 63) + ((uint64)0x000 << 52) + ((uint64)0x0000000000000);
			TEST_ENSURE(Rat(number) == 0);

			// Negative zero.
			bits = ((uint64)0x1 << 63) + ((uint64)0x000 << 52) + ((uint64)0x0000000000000);
			TEST_ENSURE(Rat(number) == 0);

			// Positive infinity.
			bits = ((uint64)0x0 << 63) + ((uint64)0x7FF << 52) + ((uint64)0x0000000000000);
			TEST_ENSURE(Rat(number) == infinity<Rat>());

			// Negative infinity.
			bits = ((uint64)0x1 << 63) + ((uint64)0x7FF << 52) + ((uint64)0x0000000000000);
			TEST_ENSURE(Rat(number) == -infinity<Rat>());
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
