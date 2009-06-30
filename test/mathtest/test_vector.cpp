#include "pastelmathtest.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/vector_tools.h"

#include "pastel/math/rational_tools.h"
#include "pastel/math/biginteger.h"
#include "pastel/sys/nativeinteger_tools.h"

using namespace Pastel;

namespace
{

	typedef Rational<NativeInteger<integer> > Real;
	//typedef real Real;

	template <int N, typename Type>
	void someFunction(const Tuple<N, Type>& jorma)
	{
	}

	void testVectorBasic()
	{
		REPORT1(sizeof(Vector<3, Real>) != 3 * sizeof(Real), sizeof(Vector<3, Real>));
		REPORT1(sizeof(Vector<3, integer>) != 3 * sizeof(integer), sizeof(Vector<3, integer>));

		Vector<3, Real> a = Vector<3, Real>(1, 2, 3);
		REPORT(a[0] != 1 || a[1] != 2 || a[2] != 3);

		Vector<3, Real> b = Vector<3, Real>(4, 5, 6);
		REPORT(b[0] != 4 || b[1] != 5 || b[2] != 6);

		Vector<3, Real> c(b);
		REPORT(c[0] != 4 || c[1] != 5 || c[2] != 6);

		a = b;
		REPORT(a[0] != 4 || a[1] != 5 || a[2] != 6);

		a = 3;
		REPORT(a[0] != 3 || a[1] != 3 || a[2] != 3);

		someFunction(asTuple(a));

		a |= 1, 2, 3;
	}

	void testVectorLowDimensional()
	{
		// Tests the low-dimension specific
		// member functions.

		{
			Vector<1, Real> a = Vector<1, Real>(1);
			REPORT(a[0] != 1);

			a.set(4);
			REPORT(a[0] != 4);

			a = 7;
			REPORT(a[0] != 7);

			Vector<1, Real> b(8);
			REPORT(b[0] != 8);

			b.set(9);
			REPORT(b[0] != 9);
		}
		{
			Vector<2, Real> a = Vector<2, Real>(1, 2);
			REPORT(a[0] != 1 || a[1] != 2);

			a.set(4, 5);
			REPORT(a[0] != 4 || a[1] != 5);

			a = 7;
			REPORT(a[0] != 7 || a[1] != 7);

			Vector<2, Real> b(8);
			REPORT(b[0] != 8 || b[1] != 8);

			b.set(9);
			REPORT(b[0] != 9 || b[1] != 9);
		}
		{
			Vector<3, Real> a = Vector<3, Real>(1, 2, 3);
			REPORT(a[0] != 1 || a[1] != 2 || a[2] != 3);

			a.set(4, 5, 6);
			REPORT(a[0] != 4 || a[1] != 5 || a[2] != 6);

			a = 7;
			REPORT(a[0] != 7 || a[1] != 7 || a[2] != 7);

			Vector<3, Real> b(8);
			REPORT(b[0] != 8 || b[1] != 8 || b[2] != 8);

			b.set(9);
			REPORT(b[0] != 9 || b[1] != 9 || b[2] != 9);
		}
		{
			Vector<4, Real> a = Vector<4, Real>(1, 2, 3, 4);
			REPORT(a[0] != 1 || a[1] != 2 ||
				 a[2] != 3 || a[3] != 4);

			a.set(4, 5, 6, 7);
			REPORT(a[0] != 4 || a[1] != 5 ||
				a[2] != 6 || a[3] != 7);

			a = 7;
			REPORT(a[0] != 7 || a[1] != 7 ||
				a[2] != 7 || a[3] != 7);

			Vector<4, Real> b(8);
			REPORT(b[0] != 8 || b[1] != 8 ||
				b[2] != 8 || b[3] != 8);

			b.set(9);
			REPORT(b[0] != 9 || b[1] != 9 ||
				b[2] != 9 || b[3] != 9);
		}
	}

	void testVectorSimpleArithmetic()
	{
		Vector<3, Real> a = Vector<3, Real>(1, 2, 3);
		Vector<3, Real> b = Vector<3, Real>(4, 5, 6);

		// Vector op Vector

		a += b;
		REPORT(a[0] != (Real)1 + 4 || a[1] != (Real)2 + 5 ||
			a[2] != (Real)3 + 6);

		a.set(1, 2, 3);
		a -= b;
		REPORT(a[0] != (Real)1 - 4 || a[1] != (Real)2 - 5 ||
			a[2] != (Real)3 - 6);

		a.set(1, 2, 3);
		a *= b;
		REPORT(a[0] != (Real)1 * 4 || a[1] != (Real)2 * 5 ||
			a[2] != (Real)3 * 6);

		a.set(1, 2, 3);
		a /= b;
		REPORT(a[0] != (Real)1 / 4 || a[1] != (Real)2 / 5 ||
			a[2] != (Real)3 / 6);

		// Vector op scalar

		a.set(1, 2, 3);
		a += 4;
		REPORT(a[0] != (Real)1 + 4 || a[1] != (Real)2 + 4 ||
			a[2] != (Real)3 + 4);

		a.set(1, 2, 3);
		a -= 4;
		REPORT(a[0] != (Real)1 - 4 || a[1] != (Real)2 - 4 ||
			a[2] != (Real)3 - 4);

		a.set(1, 2, 3);
		a *= 4;
		REPORT(a[0] != (Real)1 * 4 || a[1] != (Real)2 * 4 ||
			a[2] != (Real)3 * 4);

		a.set(1, 2, 3);
		a /= 4;
		REPORT(a[0] != (Real)1 / 4 || a[1] != (Real)2 / 4 ||
			a[2] != (Real)3 / 4);

		a.set(1, 2, 3);
		a = -a;
		REPORT(a[0] != (Real)-1 || a[1] != (Real)-2 ||
			a[2] != (Real)-3);
	};

	void testVectorArithmetic()
	{
		// Test the expression templates

		Vector<3, Real> a = Vector<3, Real>(1, 2, 3);
		Vector<3, Real> b = Vector<3, Real>(4, 5, 6);

		Vector<3, Real> c(a - b);
		REPORT(c[0] != -3 || c[1] != -3 || c[2] != -3);

		c = a * b;
		REPORT(c[0] != 4 || c[1] != 10 || c[2] != 18);

		c = a / b;
		REPORT(c[0] != (Real)1/4 ||
			c[1] != (Real)2/5 || c[2] != (Real)3/6);

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
		typedef float Real;

		Vector<3, Real> a = Vector<3, Real>(-1, -2, 3);

		Vector<3, Real> b;

		b = mabs(a);

		REPORT(
			b[0] != mabs(a[0]) ||
			b[1] != mabs(a[1]) ||
			b[2] != mabs(a[2]));

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
			b[0] != std::pow(a[0], (Real)2) ||
			b[1] != std::pow(a[1], (Real)2) ||
			b[2] != std::pow(a[2], (Real)2));

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

	template <int N>
	void testVectorArithmetic2()
	{
		const integer size = (N == Dynamic) ? 100 : N;

		Vector<N, Real> a(ofDimension(size));
		
		for (integer i = 0;i < size;++i)
		{
			a[i] = i;
		}

		Vector<N, Real> b = a.asTemporary();

		// Move construction.
		{
			REPORT1(N == Dynamic && a.size() != 0, a.size());
			REPORT1(N == Dynamic && b.size() != size, b.size());
			
			integer contentsDiffer = 0;
			for (integer i = 0;i < size;++i)
			{
				if (b[i] != i)
				{
					++contentsDiffer;
					break;
				}
			}
			REPORT1(contentsDiffer != 0, contentsDiffer);
		}

		// Vector + Vector.
		{
			Vector<N, Real> c = b + b;
			integer contentsDiffer = 0;
			for (integer i = 0;i < size;++i)
			{
				if (c[i] != i + i)
				{
					++contentsDiffer;
				}
			}
			REPORT1(contentsDiffer != 0, contentsDiffer);
		}

		// Vector += Vector.
		{
			Vector<N, Real> c = b;
			c += b;
			integer contentsDiffer = 0;
			for (integer i = 0;i < size;++i)
			{
				if (c[i] != i + i)
				{
					++contentsDiffer;
				}
			}
			REPORT1(contentsDiffer != 0, contentsDiffer);
		}

		// Vector - Vector.
		{
			Vector<N, Real> c = b - b;
			integer contentsDiffer = 0;
			for (integer i = 0;i < size;++i)
			{
				if (c[i] != i - i)
				{
					++contentsDiffer;
				}
			}
			REPORT1(contentsDiffer != 0, contentsDiffer);
		}

		// Vector -= Vector.
		{
			Vector<N, Real> c = b;
			c -= b;
			integer contentsDiffer = 0;
			for (integer i = 0;i < size;++i)
			{
				if (c[i] != i - i)
				{
					++contentsDiffer;
				}
			}
			REPORT1(contentsDiffer != 0, contentsDiffer);
		}

		// Vector * Vector.
		{
			Vector<N, Real> c = b * b;
			integer contentsDiffer = 0;
			for (integer i = 0;i < size;++i)
			{
				if (c[i] != i * i)
				{
					++contentsDiffer;
				}
			}
			REPORT1(contentsDiffer != 0, contentsDiffer);
		}

		// Vector *= Vector.
		{
			Vector<N, Real> c = b;
			c *= b;
			integer contentsDiffer = 0;
			for (integer i = 0;i < size;++i)
			{
				if (c[i] != i * i)
				{
					++contentsDiffer;
				}
			}
			REPORT1(contentsDiffer != 0, contentsDiffer);
		}

		// Vector / Vector.
		{
			Vector<N, Real> c = b / b;
			integer contentsDiffer = 0;
			for (integer i = 1;i < size;++i)
			{
				if (c[i] != (Real)i / i)
				{
					++contentsDiffer;
				}
			}
			REPORT1(contentsDiffer != 0, contentsDiffer);
		}

		// Vector /= Vector.
		{
			Vector<N, Real> c = b;
			c /= b;
			integer contentsDiffer = 0;
			for (integer i = 1;i < size;++i)
			{
				if (c[i] != (Real)i / i)
				{
					++contentsDiffer;
				}
			}
			REPORT1(contentsDiffer != 0, contentsDiffer);
		}

		// Vector + Scalar.
		{
			Vector<N, Real> c = b + 5;
			integer contentsDiffer = 0;
			for (integer i = 0;i < size;++i)
			{
				if (c[i] != i + 5)
				{
					++contentsDiffer;
				}
			}
			REPORT1(contentsDiffer != 0, contentsDiffer);
		}

		// Vector + Scalar.
		{
			Vector<N, Real> c = b;
			c += 5;
			integer contentsDiffer = 0;
			for (integer i = 0;i < size;++i)
			{
				if (c[i] != i + 5)
				{
					++contentsDiffer;
				}
			}
			REPORT1(contentsDiffer != 0, contentsDiffer);
		}

		// Vector - Scalar.
		{
			Vector<N, Real> c = b - 5;
			integer contentsDiffer = 0;
			for (integer i = 0;i < size;++i)
			{
				if (c[i] != i - 5)
				{
					++contentsDiffer;
				}
			}
			REPORT1(contentsDiffer != 0, contentsDiffer);
		}

		// Vector -= Scalar.
		{
			Vector<N, Real> c = b;
			c -= 5;
			integer contentsDiffer = 0;
			for (integer i = 0;i < size;++i)
			{
				if (c[i] != i - 5)
				{
					++contentsDiffer;
				}
			}
			REPORT1(contentsDiffer != 0, contentsDiffer);
		}

		// Vector * Scalar.
		{
			Vector<N, Real> c = b * 5;
			integer contentsDiffer = 0;
			for (integer i = 0;i < size;++i)
			{
				if (c[i] != i * 5)
				{
					++contentsDiffer;
				}
			}
			REPORT1(contentsDiffer != 0, contentsDiffer);
		}

		// Vector *= Scalar.
		{
			Vector<N, Real> c = b;
			c *= 5;
			integer contentsDiffer = 0;
			for (integer i = 0;i < size;++i)
			{
				if (c[i] != i * 5)
				{
					++contentsDiffer;
				}
			}
			REPORT1(contentsDiffer != 0, contentsDiffer);
		}

		// Vector / Scalar.
		{
			Vector<N, Real> c = b / 5;
			integer contentsDiffer = 0;
			for (integer i = 0;i < size;++i)
			{
				const Real five = 5;
				const Real invFactor = 1 / five;
				const Real left = i;
				if (c[i] != left * invFactor)
				{
					//log() << c[i] << " != " << i * invFactor << logNewLine;
					//log() << i << " / " << size << logNewLine;

					++contentsDiffer;
				}
			}
			REPORT1(contentsDiffer != 0, contentsDiffer);
		}

		// Vector /= Scalar.
		{
			Vector<N, Real> c = b;
			c /= 5;
			integer contentsDiffer = 0;
			for (integer i = 0;i < size;++i)
			{
				const Real invFactor = (Real)1 / 5;
				Real result = i;
				result *= invFactor;
				if (c[i] != result)
				{
					std::cout << c[i] << " != " << result << std::endl;
					//log() << c[i] << " != " << i * invFactor << logNewLine;
					log() << i << " / " << size << logNewLine;

					++contentsDiffer;
				}
			}
			REPORT1(contentsDiffer != 0, contentsDiffer);
		}

		// Scalar + Vector.
		{
			Vector<N, Real> c = 5 + b;
			integer contentsDiffer = 0;
			for (integer i = 0;i < size;++i)
			{
				if (c[i] != 5 + i)
				{
					++contentsDiffer;
				}
			}
			REPORT1(contentsDiffer != 0, contentsDiffer);
		}

		// Scalar - Vector.
		{
			Vector<N, Real> c = 5 - b;
			integer contentsDiffer = 0;
			for (integer i = 0;i < size;++i)
			{
				if (c[i] != 5 - i)
				{
					++contentsDiffer;
				}
			}
			REPORT1(contentsDiffer != 0, contentsDiffer);
		}

		// Scalar * Vector.
		{
			Vector<N, Real> c = 5 * b;
			integer contentsDiffer = 0;
			for (integer i = 0;i < size;++i)
			{
				if (c[i] != 5 * i)
				{
					++contentsDiffer;
				}
			}
			REPORT1(contentsDiffer != 0, contentsDiffer);
		}

		// Scalar / Vector.
		{
			Vector<N, Real> c = 5 / b;
			integer contentsDiffer = 0;
			for (integer i = 1;i < size;++i)
			{
				const Real left = 5;
				if (c[i] != left / i)
				{
					//log() << c[i] << " != " << left / i << logNewLine;
					++contentsDiffer;
				}
			}
			REPORT1(contentsDiffer != 0, contentsDiffer);
		}
	}

	template <int N>
	void testCollaboration()
	{
		const integer size = (N == Dynamic) ? 100 : N;

		Point<N, Real> a(ofDimension(size));

		Vector<N, Real> b = a.asVector();

		Point<N, Real> e = a.asTemporary();
		if (N == Dynamic)
		{
			REPORT1(a.size() != 0, a.size());
			REPORT2(e.size() != size, e.size(), size);
		}

		Tuple<N, Real> c = b.asTuple().asTemporary();
		if (N == Dynamic)
		{
			REPORT1(b.size() != 0, b.size());
			REPORT2(c.size() != size, c.size(), size);
		}

		Tuple<N, Real> d = c;
		if (N == Dynamic)
		{
			REPORT2(d.size() != size, d.size(), size);
			REPORT2(c.size() != size, c.size(), size);
		}

		d = c;
		if (N == Dynamic)
		{
			REPORT2(d.size() != size, d.size(), size);
			REPORT2(c.size() != size, c.size(), size);
		}

		TemporaryTuple<N, Real> f = d.asTemporary();
		if (N == Dynamic)
		{
			REPORT2(f.size() != size, f.size(), size);
			REPORT1(d.size() != 0, d.size());
		}

		Tuple<N, Real> g(ofDimension(size));
		Vector<N, Real> j(g);
		Point<N, Real> h(g);
		Point<N, Real> k(j);

		j.asTuple();
		j.asTemporary().asTuple();

		h.asTuple();
		h.asTemporary().asTuple();

		h.asVector();
		h.asTemporary().asVector();
	}

	void testBegin()
	{
		testVectorBasic();
		testVectorLowDimensional();
		testVectorSimpleArithmetic();
		testVectorArithmetic();
		testVectorArray();
		
		testVectorArithmetic2<1>();
		testVectorArithmetic2<2>();
		testVectorArithmetic2<3>();
		testVectorArithmetic2<4>();
		testVectorArithmetic2<Dynamic>();

		testCollaboration<1>();
		testCollaboration<2>();
		testCollaboration<3>();
		testCollaboration<4>();
		testCollaboration<Dynamic>();
	}

	void testAdd()
	{
		mathTestList().add("Vector", testBegin);
	}

	CallFunction run(testAdd);

}
