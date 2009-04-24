#include "pastelmathtest.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/vector_tools.h"

using namespace Pastel;

namespace
{

	template <int N, typename Type>
	void someFunction(const Tuple<N, Type>& jorma)
	{
	}

	void testVectorBasic()
	{
		REPORT1(sizeof(Vector<3, real>) != 3 * sizeof(real), sizeof(Vector<3, real>));
		REPORT1(sizeof(Vector<3, integer>) != 3 * sizeof(integer), sizeof(Vector<3, integer>));

		Vector<3, real> a(1, 2, 3);
		REPORT(a[0] != 1 || a[1] != 2 || a[2] != 3);

		Vector<3, real> b(4, 5, 6);
		REPORT(b[0] != 4 || b[1] != 5 || b[2] != 6);

		Vector<3, real> c(b);
		REPORT(c[0] != 4 || c[1] != 5 || c[2] != 6);

		a = b;
		REPORT(a[0] != 4 || a[1] != 5 || a[2] != 6);

		a = 3;
		REPORT(a[0] != 3 || a[1] != 3 || a[2] != 3);

		someFunction(asTuple(a));
	}

	void testVectorLowDimensional()
	{
		// Tests the low-dimension specific
		// member functions.

		{
			Vector<1, real> a(1);
			REPORT(a[0] != 1);

			a.set(4);
			REPORT(a[0] != 4);

			a = 7;
			REPORT(a[0] != 7);

			Vector<1, real> b(8);
			REPORT(b[0] != 8);

			b.set(9);
			REPORT(b[0] != 9);
		}
		{
			Vector<2, real> a(1, 2);
			REPORT(a[0] != 1 || a[1] != 2);

			a.set(4, 5);
			REPORT(a[0] != 4 || a[1] != 5);

			a = 7;
			REPORT(a[0] != 7 || a[1] != 7);

			Vector<2, real> b(8);
			REPORT(b[0] != 8 || b[1] != 8);

			b.set(9);
			REPORT(b[0] != 9 || b[1] != 9);
		}
		{
			Vector<3, real> a(1, 2, 3);
			REPORT(a[0] != 1 || a[1] != 2 || a[2] != 3);

			a.set(4, 5, 6);
			REPORT(a[0] != 4 || a[1] != 5 || a[2] != 6);

			a = 7;
			REPORT(a[0] != 7 || a[1] != 7 || a[2] != 7);

			Vector<3, real> b(8);
			REPORT(b[0] != 8 || b[1] != 8 || b[2] != 8);

			b.set(9);
			REPORT(b[0] != 9 || b[1] != 9 || b[2] != 9);
		}
		{
			Vector<4, real> a(1, 2, 3, 4);
			REPORT(a[0] != 1 || a[1] != 2 ||
				 a[2] != 3 || a[3] != 4);

			a.set(4, 5, 6, 7);
			REPORT(a[0] != 4 || a[1] != 5 ||
				a[2] != 6 || a[3] != 7);

			a = 7;
			REPORT(a[0] != 7 || a[1] != 7 ||
				a[2] != 7 || a[3] != 7);

			Vector<4, real> b(8);
			REPORT(b[0] != 8 || b[1] != 8 ||
				b[2] != 8 || b[3] != 8);

			b.set(9);
			REPORT(b[0] != 9 || b[1] != 9 ||
				b[2] != 9 || b[3] != 9);
		}
	}

	void testVectorSimpleArithmetic()
	{
		Vector<3, real> a(1, 2, 3);
		Vector<3, real> b(4, 5, 6);

		// Vector op Vector

		a += b;
		REPORT(a[0] != (real)1 + 4 || a[1] != (real)2 + 5 ||
			a[2] != (real)3 + 6);

		a.set(1, 2, 3);
		a -= b;
		REPORT(a[0] != (real)1 - 4 || a[1] != (real)2 - 5 ||
			a[2] != (real)3 - 6);

		a.set(1, 2, 3);
		a *= b;
		REPORT(a[0] != (real)1 * 4 || a[1] != (real)2 * 5 ||
			a[2] != (real)3 * 6);

		a.set(1, 2, 3);
		a /= b;
		REPORT(a[0] != (real)1 / 4 || a[1] != (real)2 / 5 ||
			a[2] != (real)3 / 6);

		// Vector op scalar

		a.set(1, 2, 3);
		a += 4;
		REPORT(a[0] != (real)1 + 4 || a[1] != (real)2 + 4 ||
			a[2] != (real)3 + 4);

		a.set(1, 2, 3);
		a -= 4;
		REPORT(a[0] != (real)1 - 4 || a[1] != (real)2 - 4 ||
			a[2] != (real)3 - 4);

		a.set(1, 2, 3);
		a *= 4;
		REPORT(a[0] != (real)1 * 4 || a[1] != (real)2 * 4 ||
			a[2] != (real)3 * 4);

		a.set(1, 2, 3);
		a /= 4;
		REPORT(a[0] != (real)1 / 4 || a[1] != (real)2 / 4 ||
			a[2] != (real)3 / 4);

		a.set(1, 2, 3);
		a = -a;
		REPORT(a[0] != (real)-1 || a[1] != (real)-2 ||
			a[2] != (real)-3);
	};

	void testVectorArithmetic()
	{
		// Test the expression templates

		Vector<3, real> a(1, 2, 3);
		Vector<3, real> b(4, 5, 6);

		Vector<3, real> c(a - b);
		REPORT(c[0] != -3 || c[1] != -3 || c[2] != -3);

		c = a * b;
		REPORT(c[0] != 4 || c[1] != 10 || c[2] != 18);

		c = a / b;
		REPORT(c[0] != (real)1/4 ||
			c[1] != (real)2/5 || c[2] != (real)3/6);

		c = a + b;
		REPORT(c[0] != 5 || c[1] != 7 || c[2] != 9);

		c = (a + b) + 5;
		REPORT(c[0] != 10 || c[1] != 12 || c[2] != 14);

		c = 5 + (a + b);
		REPORT(c[0] != 10 || c[1] != 12 || c[2] != 14);

		c = (a + b) * 5;
		REPORT(c[0] != 25 || c[1] != 35 || c[2] != 45);

		c = 5 * (a + b);
		REPORT(c[0] != 25 || c[1] != 35 || c[2] != 45);

		c = -a;
		REPORT(c[0] != -1 || c[1] != -2 || c[2] != -3);
	}

	void testVectorArray()
	{
		Vector<3, real> a(-1, -2, 3);

		Vector<3, real> b;

		b = abs(a);

		REPORT(
			b[0] != std::abs(a[0]) ||
			b[1] != std::abs(a[1]) ||
			b[2] != std::abs(a[2]));

		a.set(0.25, 0.5, 0.75);

		b = exp(a);

		REPORT(
			b[0] != std::exp(a[0]) ||
			b[1] != std::exp(a[1]) ||
			b[2] != std::exp(a[2]));

		b = log(a);

		REPORT(
			b[0] != std::log(a[0]) ||
			b[1] != std::log(a[1]) ||
			b[2] != std::log(a[2]));

		b = pow(a, a);

		REPORT(
			b[0] != std::pow(a[0], a[0]) ||
			b[1] != std::pow(a[1], a[1]) ||
			b[2] != std::pow(a[2], a[2]));

		b = pow(a, 2);

		REPORT(
			b[0] != std::pow(a[0], (real)2) ||
			b[1] != std::pow(a[1], (real)2) ||
			b[2] != std::pow(a[2], (real)2));

		b = sqrt(a);

		REPORT(
			b[0] != std::sqrt(a[0]) ||
			b[1] != std::sqrt(a[1]) ||
			b[2] != std::sqrt(a[2]));

		b = floor(a);

		REPORT(
			b[0] != std::floor(a[0]) ||
			b[1] != std::floor(a[1]) ||
			b[2] != std::floor(a[2]));

		b = sin(a);

		REPORT(
			b[0] != std::sin(a[0]) ||
			b[1] != std::sin(a[1]) ||
			b[2] != std::sin(a[2]));

		b = cos(a);

		REPORT(
			b[0] != std::cos(a[0]) ||
			b[1] != std::cos(a[1]) ||
			b[2] != std::cos(a[2]));

		b = tan(a);

		REPORT(
			b[0] != std::tan(a[0]) ||
			b[1] != std::tan(a[1]) ||
			b[2] != std::tan(a[2]));

		b = asin(a);

		REPORT(
			b[0] != std::asin(a[0]) ||
			b[1] != std::asin(a[1]) ||
			b[2] != std::asin(a[2]));

		b = acos(a);

		REPORT(
			b[0] != std::acos(a[0]) ||
			b[1] != std::acos(a[1]) ||
			b[2] != std::acos(a[2]));

		b = atan2(a, a);

		REPORT(
			b[0] != std::atan2(a[0], a[0]) ||
			b[1] != std::atan2(a[1], a[1]) ||
			b[2] != std::atan2(a[2], a[2]));

		b = inverse(a);

		REPORT(
			b[0] != inverse(a[0]) ||
			b[1] != inverse(a[1]) ||
			b[2] != inverse(a[2]));
	}

	void testVectorUnbounded()
	{
		const integer Size = 100;

		Vector<Unbounded, real> a;
		a.setSize(Size);
		
		for (integer i = 0;i < Size;++i)
		{
			a[i] = i;
		}

		Vector<Unbounded, real> b = a.asTemporary();

		// Test move construction.
		{
			REPORT1(a.size() != 0, a.size());
			REPORT1(b.size() != Size, b.size());
			
			bool contentsMatch = true;
			for (integer i = 0;i < Size;++i)
			{
				if (b[i] != i)
				{
					contentsMatch = false;
					break;
				}
			}
			REPORT(!contentsMatch);
		}

		// Test addition.
		{
			Vector<Unbounded, real> c = b + b;
			bool contentsMatch = true;
			for (integer i = 0;i < Size;++i)
			{
				if (c[i] != i + i)
				{
					contentsMatch = false;
				}
			}
			REPORT(!contentsMatch);
		}

		// Test subtraction.
		{
			Vector<Unbounded, real> c = b - b;
			bool contentsMatch = true;
			for (integer i = 0;i < Size;++i)
			{
				if (c[i] != i - i)
				{
					contentsMatch = false;
				}
			}
			REPORT(!contentsMatch);
		}

		// Test multiplication.
		{
			Vector<Unbounded, real> c = b * b;
			bool contentsMatch = true;
			for (integer i = 0;i < Size;++i)
			{
				if (c[i] != i * i)
				{
					contentsMatch = false;
				}
			}
			REPORT(!contentsMatch);
		}

		// Test division.
		{
			Vector<Unbounded, real> c = b / b;
			bool contentsMatch = true;
			for (integer i = 1;i < Size;++i)
			{
				if (c[i] != (real)i / i)
				{
					contentsMatch = false;
				}
			}
			REPORT(!contentsMatch);
		}

		Tuple<Unbounded, real> c = b.asTuple();
		Tuple<Unbounded, real> d = c;
		d = c;
	}

	void testBegin()
	{
		testVectorBasic();
		testVectorLowDimensional();
		testVectorSimpleArithmetic();
		testVectorArithmetic();
		testVectorArray();
	}

	void testAdd()
	{
		mathTestList().add("Vector.Unbounded", testVectorUnbounded);
		mathTestList().add("Vector", testBegin);
	}

	CallFunction run(testAdd);

}
