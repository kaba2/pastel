#include "pastelsystest.h"

#include "pastel/sys/rational.h"
#include "pastel/sys/biginteger.h"
#include "pastel/sys/nativeinteger.h"
#include "pastel/sys/rational_tools.h"
#include "pastel/sys/biginteger_tools.h"
#include "pastel/sys/nativeinteger_tools.h"

#include <iostream>

namespace
{

	using namespace Pastel;
	using namespace std;

	typedef BigInteger Integer;
	//typedef NativeInteger<integer> Integer;
	typedef Rational<Integer> Rat;

	void testSimple()
	{
		{
			Rat a;
			REPORT(a != 0);
		}
		{
			REPORT(Rat(1) != 1);
			REPORT(Rat(2) != 2);
			REPORT(Rat(1, 1) != 1);
			REPORT(Rat(2, 2) != 1);
			REPORT(Rat(4, 2) != 2);
		}
		{
			Rat a(0, 0);
			a.set(1, 5);
			REPORT(a != Rat(1, 5));
		}
		{
			Rat a(5 * 1234235, 7 * 1234235);
			REPORT(a != Rat(5, 7));
			REPORT(a.numerator() != 5);
			REPORT(a.denominator() != 7);
		}
		{
			Rat a(2, 5);
			Rat b(3, 5);
			Rat c(a - b);
			REPORT(c != Rat(-1, 5));
		}
		{
			Rat a(2, 5);
			Rat b(3, 5);
			Rat c(a / b);
			REPORT(c != Rat(2, 3));
		}
	}

	void testSpecial()
	{
		{
			Rat a(1, 0);
			REPORT(a != infinity<Rat>());

			Rat b(2, 0);
			REPORT(b != infinity<Rat>());

			Rat c(-1, 0);
			REPORT(c != -infinity<Rat>());

			Rat d(-2, 0);
			REPORT(d != -infinity<Rat>());
		}
		{
			REPORT(infinity<Rat>() < 23435);
			REPORT(-2312 < -infinity<Rat>());
			REPORT(Integer(-2312) < -infinity<Rat>());
			REPORT(infinity<Rat>() < -infinity<Rat>());
		}
	}

	void testAddition()
	{
		{
			REPORT(Rat(2, 5) + Rat(3, 5) != Rat(1, 1));
			REPORT(Rat(2, 5) + Rat(3, -5) != Rat(-1, 5));
			REPORT(Rat(2, 5) + Rat(-3, 5) != Rat(-1, 5));
			REPORT(Rat(2, 5) + Rat(-3, -5) != Rat(5, 5));

			REPORT(Rat(2, -5) + Rat(3, 5) != Rat(1, 5));
			REPORT(Rat(2, -5) + Rat(3, -5) != Rat(-1));
			REPORT(Rat(2, -5) + Rat(-3, 5) != Rat(-1, 1));
			REPORT(Rat(2, -5) + Rat(-3, -5) != Rat(1, 5));

			REPORT(Rat(-2, 5) + Rat(3, 5) != Rat(1, 5));
			REPORT(Rat(-2, 5) + Rat(3, -5) != Rat(-1));
			REPORT(Rat(-2, 5) + Rat(-3, 5) != Rat(-1, 1));
			REPORT(Rat(-2, 5) + Rat(-3, -5) != Rat(1, 5));

			REPORT(Rat(-2, -5) + Rat(3, 5) != Rat(1, 1));
			REPORT(Rat(-2, -5) + Rat(3, -5) != Rat(-1, 5));
			REPORT(Rat(-2, -5) + Rat(-3, 5) != Rat(-1, 5));
			REPORT(Rat(-2, -5) + Rat(-3, -5) != Rat(5, 5));
		}
		{
			// Adding with a NaN should give a NaN.

			REPORT(Rat(0, 0) + Rat(1, 4) != Rat(0, 0));
			REPORT(Rat(1, 5) + Rat(0, 0) != Rat(0, 0));

			REPORT(Rat(0, 3) + Rat(0, 0) != Rat(0, 0));
			REPORT(Rat(0, 0) + Rat(0, 3) != Rat(0, 0));

			REPORT(Rat(15, 0) + Rat(0, 0) != Rat(0, 0));
			REPORT(Rat(-15, 0) + Rat(0, 0) != Rat(0, 0));
		}
		{
			// Adding infinity and -infinity should give a NaN.

			REPORT(Rat(-1, 0) + Rat(1, 0) != Rat(0, 0));
			REPORT(Rat(1, 0) + Rat(-1, 0) != Rat(0, 0));
		}
	}

	void testMultiply()
	{
		{
			REPORT(Rat(2, 5) * Rat(3, 5) != Rat(6, 25));
			REPORT(Rat(2, 5) * Rat(3, -5) != Rat(-6, 25));
			REPORT(Rat(2, 5) * Rat(-3, 5) != Rat(-6, 25));
			REPORT(Rat(2, 5) * Rat(-3, -5) != Rat(6, 25));

			REPORT(Rat(2, -5) * Rat(3, 5) != Rat(-6, 25));
			REPORT(Rat(2, -5) * Rat(3, -5) != Rat(6, 25));
			REPORT(Rat(2, -5) * Rat(-3, 5) != Rat(6, 25));
			REPORT(Rat(2, -5) * Rat(-3, -5) != Rat(-6, 25));

			REPORT(Rat(-2, 5) * Rat(3, 5) != Rat(-6, 25));
			REPORT(Rat(-2, 5) * Rat(3, -5) != Rat(6, 25));
			REPORT(Rat(-2, 5) * Rat(-3, 5) != Rat(6, 25));
			REPORT(Rat(-2, 5) * Rat(-3, -5) != Rat(-6, 25));

			REPORT(Rat(-2, -5) * Rat(3, 5) != Rat(6, 25));
			REPORT(Rat(-2, -5) * Rat(3, -5) != Rat(-6, 25));
			REPORT(Rat(-2, -5) * Rat(-3, 5) != Rat(-6, 25));
			REPORT(Rat(-2, -5) * Rat(-3, -5) != Rat(6, 25));
		}
		{
			// Multiplication by zero should give a zero...

			REPORT(Rat(0, 4) * Rat(0, 7) != 0);
			REPORT(Rat(0, 4) * Rat(0, -7) != 0);
			REPORT(Rat(0, -4) * Rat(0, 7) != 0);
			REPORT(Rat(0, -4) * Rat(0, -7) != 0);

			// ..Except with NaNs a NaN:

			REPORT(Rat(0, 4) * Rat(0, 0) != Rat(0, 0));
			REPORT(Rat(0, 0) * Rat(0, 4) != Rat(0, 0));
			REPORT(Rat(0, -4) * Rat(0, 0) != Rat(0, 0));
			REPORT(Rat(0, 0) * Rat(0, -4) != Rat(0, 0));

			// Infinities with zeros should give zero:

			REPORT(infinity<Rat>() * 0 != 0);
			REPORT(-infinity<Rat>() * 0 != 0);

			// Infinities together should maintain
			// infinity.

			REPORT(infinity<Rat>() * infinity<Rat>() !=
				infinity<Rat>());
			REPORT(infinity<Rat>() * -infinity<Rat>() !=
				-infinity<Rat>());
			REPORT(-infinity<Rat>() * infinity<Rat>() !=
				-infinity<Rat>());
			REPORT(-infinity<Rat>() * -infinity<Rat>() !=
				infinity<Rat>());
		}
	}

	void testFloat()
	{
		float number = 0;
		uint32& bits = *((uint32*)&number);
		Rat rat;

		bits = (0x0 << 31) + (0x00 << 23) + (0x000000);
		rat = Rat(number);
		cout << rat << endl;
		REPORT(rat != 0);

		bits = (0x1 << 31) + (0x00 << 23) + (0x000000);
		rat = Rat(number);
		cout << rat << endl;
		REPORT(rat != 0);

		bits = (0x0 << 31) + (0xFF << 23) + (0x000000);
		rat = Rat(number);
		cout << rat << endl;
		REPORT(rat != infinity<Rat>());

		bits = (0x1 << 31) + (0xFF << 23) + (0x000000);
		rat = Rat(number);
		cout << rat << endl;
		REPORT(rat != -infinity<Rat>());

		bits = (0x0 << 31) + (0x7F << 23) + (0x123456);
		rat = Rat(number);
		cout << rat << endl;
		REPORT(rat != Rat(0x123456 + 0x800000, 1 << 23));

		cout << Rat(1.2134f) << endl;
		cout << Rat(61234.934f) << endl;
		cout << Rat(1435.642f) << endl;
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

	void testBegin()
	{
		testSyntax();
		testSimple();
		testSpecial();
		testAddition();
		testMultiply();
		testFloat();
	}

	void testAdd()
	{
		sysTestList().add("Rational", testBegin);
	}

	CallFunction run(testAdd);

}
