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
	void someFunction(const Tuple<Type, N>& jorma)
	{
	}

	template <int N>
	void testNullPointerStatic()
	{
		typedef Rational<BigInteger> Real;

		Vector<Real, N> au2(0);
		Vector<Real, N> au3(0.0);
		Vector<Real, N> au4(withCopying(au2.data()));

		Vector<float, N> af2(0);
		Vector<float, N> af3(0.0);
		Vector<float, N> af4(withCopying(af2.data()));

		Vector<int, N> ai1(0);
		Vector<int, N> ai2(0.0);
		Vector<int, N> ai3(withCopying(ai1.data()));
	}

	template <int N>
	void testNullPointer()
	{
		const integer n = (N == Dynamic) ? 3 : N;

		Tuple<float, N> t(ofDimension(5), 0);

		Vector<float, N> c;

		Vector<float, N> af1(c + c);
		Vector<float, N> af5(ofDimension(n), 0);
		Vector<float, N> af6(ofDimension(n), 0.0);
		Vector<float, N> af7(ofDimension(n), withCopying(af1.data()));

		Vector<int, N> ai5(ofDimension(n), 0);
		Vector<int, N> ai6(ofDimension(n), 0.0);
		Vector<int, N> ai7(ofDimension(n), withCopying(ai5.data()));
	}

	/*
	void testVectorBasic()
	{
		REPORT1(sizeof(Vector<Real, 3>) != 3 * sizeof(Real), sizeof(Vector<Real, 3>));
		REPORT1(sizeof(Vector<integer, 3>) != 3 * sizeof(integer), sizeof(Vector<integer, 3>));

		Vector<Real, 3> a = Vector<Real, 3>(1, 2, 3);
		REPORT(a[0] != 1 || a[1] != 2 || a[2] != 3);

		Vector<Real, 3> b = Vector<Real, 3>(4, 5, 6);
		REPORT(b[0] != 4 || b[1] != 5 || b[2] != 6);

		Vector<Real, 3> c(b);
		REPORT(c[0] != 4 || c[1] != 5 || c[2] != 6);

		a = b;
		REPORT(a[0] != 4 || a[1] != 5 || a[2] != 6);

		a = 3;
		REPORT(a[0] != 3 || a[1] != 3 || a[2] != 3);

		someFunction(asTuple(a));

		Vector<float> cd;

		a |= 1, 2, 3;
	}

	void testVectorLowDimensional()
	{
		// Tests the low-dimension specific
		// member functions.

		{
			Vector<Real, 1> a = Vector<Real, 1>(1);
			REPORT(a[0] != 1);

			a.set(4);
			REPORT(a[0] != 4);

			a = 7;
			REPORT(a[0] != 7);

			Vector<Real, 1> b(8);
			REPORT(b[0] != 8);

			b.set(9);
			REPORT(b[0] != 9);
		}
		{
			Vector<Real, 2> a = Vector<Real, 2>(1, 2);
			REPORT(a[0] != 1 || a[1] != 2);

			a.set(4, 5);
			REPORT(a[0] != 4 || a[1] != 5);

			a = 7;
			REPORT(a[0] != 7 || a[1] != 7);

			Vector<Real, 2> b(8);
			REPORT(b[0] != 8 || b[1] != 8);

			b.set(9);
			REPORT(b[0] != 9 || b[1] != 9);
		}
		{
			Vector<Real, 3> a = Vector<Real, 3>(1, 2, 3);
			REPORT(a[0] != 1 || a[1] != 2 || a[2] != 3);

			a.set(4, 5, 6);
			REPORT(a[0] != 4 || a[1] != 5 || a[2] != 6);

			a = 7;
			REPORT(a[0] != 7 || a[1] != 7 || a[2] != 7);

			Vector<Real, 3> b(8);
			REPORT(b[0] != 8 || b[1] != 8 || b[2] != 8);

			b.set(9);
			REPORT(b[0] != 9 || b[1] != 9 || b[2] != 9);
		}
		{
			Vector<Real, 4> a = Vector<Real, 4>(1, 2, 3, 4);
			REPORT(a[0] != 1 || a[1] != 2 ||
				 a[2] != 3 || a[3] != 4);

			a.set(4, 5, 6, 7);
			REPORT(a[0] != 4 || a[1] != 5 ||
				a[2] != 6 || a[3] != 7);

			a = 7;
			REPORT(a[0] != 7 || a[1] != 7 ||
				a[2] != 7 || a[3] != 7);

			Vector<Real, 4> b(8);
			REPORT(b[0] != 8 || b[1] != 8 ||
				b[2] != 8 || b[3] != 8);

			b.set(9);
			REPORT(b[0] != 9 || b[1] != 9 ||
				b[2] != 9 || b[3] != 9);
		}
	}

	void testVectorSimpleArithmetic()
	{
		Vector<Real, 3> a = Vector<Real, 3>(1, 2, 3);
		Vector<Real, 3> b = Vector<Real, 3>(4, 5, 6);

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

		Vector<Real, 3> a = Vector<Real, 3>(1, 2, 3);
		Vector<Real, 3> b = Vector<Real, 3>(4, 5, 6);

		Vector<Real, 3> c(a - b);
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

		Vector<Real, 3> a = Vector<Real, 3>(-1, -2, 3);

		Vector<Real, 3> b;

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

		Vector<Real, N> a(ofDimension(size));
		
		for (integer i = 0;i < size;++i)
		{
			a[i] = i;
		}

		Vector<Real, N> b = a.asTemporary();

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
			Vector<Real, N> c = b + b;
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
			Vector<Real, N> c = b;
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
			Vector<Real, N> c = b - b;
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
			Vector<Real, N> c = b;
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
			Vector<Real, N> c = b * b;
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
			Vector<Real, N> c = b;
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
			Vector<Real, N> c = b / b;
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
			Vector<Real, N> c = b;
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
			Vector<Real, N> c = b + 5;
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
			Vector<Real, N> c = b;
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
			Vector<Real, N> c = b - 5;
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
			Vector<Real, N> c = b;
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
			Vector<Real, N> c = b * 5;
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
			Vector<Real, N> c = b;
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
			Vector<Real, N> c = b / 5;
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
			Vector<Real, N> c = b;
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
			Vector<Real, N> c = 5 + b;
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
			Vector<Real, N> c = 5 - b;
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
			Vector<Real, N> c = 5 * b;
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
			Vector<Real, N> c = 5 / b;
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

		Point<Real, N> a(ofDimension(size));

		Vector<Real, N> b = a.asVector();

		Point<Real, N> e = a.asTemporary();
		if (N == Dynamic)
		{
			REPORT1(a.size() != 0, a.size());
			REPORT2(e.size() != size, e.size(), size);
		}

		Tuple<Real, N> c = b.asTuple().asTemporary();
		if (N == Dynamic)
		{
			REPORT1(b.size() != 0, b.size());
			REPORT2(c.size() != size, c.size(), size);
		}

		Tuple<Real, N> d = c;
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

		TemporaryTuple<Real, N> f = d.asTemporary();
		if (N == Dynamic)
		{
			REPORT2(f.size() != size, f.size(), size);
			REPORT1(d.size() != 0, d.size());
		}

		Tuple<Real, N> g(ofDimension(size));
		Vector<Real, N> j(g);
		Point<Real, N> h(g);
		Point<Real, N> k(j);

		j.asTuple();
		j.asTemporary().asTuple();

		h.asTuple();
		h.asTemporary().asTuple();

		h.asVector();
		h.asTemporary().asVector();
	}
	*/

	void testBegin()
	{
		testNullPointerStatic<1>();
		testNullPointerStatic<2>();
		testNullPointerStatic<3>();
		testNullPointerStatic<4>();

		testNullPointer<1>();
		testNullPointer<2>();
		testNullPointer<3>();
		testNullPointer<4>();
		testNullPointer<Dynamic>();

		/*
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
		*/
	}

	void testAdd()
	{
		mathTestList().add("Vector", testBegin);
	}

	CallFunction run(testAdd);

}
