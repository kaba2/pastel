// Description: Testing for Rational
// DocumentationOf: rational.h

#include "test_pastelsys.h"

#include "pastel/sys/rational.h"
#include "pastel/sys/biginteger.h"
#include "pastel/sys/rational_tools.h"
#include "pastel/sys/biginteger_tools.h"

#include <iostream>

namespace
{

	using namespace Pastel;
	using namespace std;

	typedef BigInteger Integer;
	typedef Rational<Integer> Rat;

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
			testSyntax();
			testSimple();
			testSpecial();
			testAddition();
			testMultiply();
			testFloat();
			testDouble();
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
				TEST_ENSURE(a.numerator() == 5);
				TEST_ENSURE(a.denominator() == 7);
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

			Integer i;

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
