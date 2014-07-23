// Description: Testing for Vector
// DocumentationOf: vector.h

#include "test_pastelsys.h"

#include "pastel/sys/rational.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/vector_tools.h"
#include "pastel/sys/biginteger.h"

using namespace Pastel;

namespace
{

	using Real = Rational<integer>;
	//using Real = real;

	template <typename Type, int N>
	void someFunction(const Tuple<Type, N>& jorma)
	{
	}

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
			testNullPointerStatic<1>();
			testNullPointerStatic<2>();
			testNullPointerStatic<3>();
			testNullPointerStatic<4>();

			testNullPointer<1>();
			testNullPointer<2>();
			testNullPointer<3>();
			testNullPointer<4>();
			testNullPointer<Dynamic>();

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
		
		template <int N>
		void testNullPointerStatic()
		{
			using Real = Rational<BigInteger>;

			Vector<Real, N> au2(0);
			Vector<Real, N> au3(0.0);

			Vector<float, N> af2(0);
			Vector<float, N> af3(0.0);

			Vector<int, N> ai1(0);
			Vector<int, N> ai2(0.0);
		}

		template <int N>
		void testNullPointer()
		{
			integer n = (N == Dynamic) ? 3 : N;

			Tuple<float, N> t(ofDimension(n), 0);

			Vector<float, N> c;

			Vector<float, N> af1(c + c);
			Vector<float, N> af5(ofDimension(n), 0);
			Vector<float, N> af6(ofDimension(n), 0.0);

			Vector<int, N> ai5(ofDimension(n), 0);
			Vector<int, N> ai6(ofDimension(n), 0.0);
		}

		void testVectorBasic()
		{

			PASTEL_STATIC_ASSERT(sizeof(Vector<Real, 3>) == 3 * sizeof(Real));
			PASTEL_STATIC_ASSERT(sizeof(Vector<integer, 3>) == 3 * sizeof(integer));

			Vector<Real, 3> a(1, 2, 3);
			TEST_ENSURE(a[0] == 1 && a[1] == 2 && a[2] == 3);

			Vector<Real, 3> b = Vector<Real, 3>(4, 5, 6);
			TEST_ENSURE(b[0] == 4 && b[1] == 5 && b[2] == 6);

			Vector<Real, 3> c(b);
			TEST_ENSURE(c[0] == 4 && c[1] == 5 && c[2] == 6);

			a = b;
			TEST_ENSURE(a[0] == 4 && a[1] == 5 && a[2] == 6);

			a = 3;
			TEST_ENSURE(a[0] == 3 && a[1] == 3 && a[2] == 3);

			someFunction(asTuple(a));

			Vector<float> cd;

			a.assign({ 1, 2, 3});
			TEST_ENSURE(a[0] == 1 && a[1] == 2 && a[2] == 3);

			// Visual Studio 2013 has a bug which ambiguates
			// this, although it shouldn't. A very weird bug:
			// there is no ambiguity when the number of items
			// is other than 3.
			//a = { 1, 2, 3 };
		}

		void testVectorLowDimensional()
		{
			// Tests the low-dimension specific
			// member functions.

			{
				Vector<Real, 1> a = Vector<Real, 1>(1);
				TEST_ENSURE(a[0] == 1);

				a.set(4);
				TEST_ENSURE(a[0] == 4);

				a = 7;
				TEST_ENSURE(a[0] == 7);

				Vector<Real, 1> b(8);
				TEST_ENSURE(b[0] == 8);

				b.set(9);
				TEST_ENSURE(b[0] == 9);
			}
			{
				Vector<Real, 2> a = Vector<Real, 2>(1, 2);
				TEST_ENSURE(a[0] == 1 && a[1] == 2);

				a.set(4, 5);
				TEST_ENSURE(a[0] == 4 && a[1] == 5);

				a = 7;
				TEST_ENSURE(a[0] == 7 && a[1] == 7);

				Vector<Real, 2> b(8);
				TEST_ENSURE(b[0] == 8 && b[1] == 8);

				b.set(9);
				TEST_ENSURE(b[0] == 9 && b[1] == 9);
			}
			{
				Vector<Real, 3> a = Vector<Real, 3>(1, 2, 3);
				TEST_ENSURE(a[0] == 1 && a[1] == 2 && a[2] == 3);

				a.set(4, 5, 6);
				TEST_ENSURE(a[0] == 4 && a[1] == 5 && a[2] == 6);

				a = 7;
				TEST_ENSURE(a[0] == 7 && a[1] == 7 && a[2] == 7);

				Vector<Real, 3> b(8);
				TEST_ENSURE(b[0] == 8 && b[1] == 8 && b[2] == 8);

				b.set(9);
				TEST_ENSURE(b[0] == 9 && b[1] == 9 && b[2] == 9);
			}
			{
				Vector<Real, 4> a = Vector<Real, 4>(1, 2, 3, 4);
				TEST_ENSURE(a[0] == 1 && a[1] == 2 &&
					 a[2] == 3 && a[3] == 4);

				a.set(4, 5, 6, 7);
				TEST_ENSURE(a[0] == 4 && a[1] == 5 &&
					a[2] == 6 && a[3] == 7);

				a = 7;
				TEST_ENSURE(a[0] == 7 && a[1] == 7 &&
					a[2] == 7 && a[3] == 7);

				Vector<Real, 4> b(8);
				TEST_ENSURE(b[0] == 8 && b[1] == 8 &&
					b[2] == 8 && b[3] == 8);

				b.set(9);
				TEST_ENSURE(b[0] == 9 && b[1] == 9 &&
					b[2] == 9 && b[3] == 9);
			}
		}

		void testVectorSimpleArithmetic()
		{
			Vector<Real, 3> a = Vector<Real, 3>(1, 2, 3);
			Vector<Real, 3> b = Vector<Real, 3>(4, 5, 6);

			// Vector op Vector

			a += b;
			TEST_ENSURE(a[0] == (Real)1 + 4 && a[1] == (Real)2 + 5 &&
				a[2] == (Real)3 + 6);

			a.set(1, 2, 3);
			a -= b;
			TEST_ENSURE(a[0] == (Real)1 - 4 && a[1] == (Real)2 - 5 &&
				a[2] == (Real)3 - 6);

			a.set(1, 2, 3);
			a *= b;
			TEST_ENSURE(a[0] == (Real)1 * 4 && a[1] == (Real)2 * 5 &&
				a[2] == (Real)3 * 6);

			a.set(1, 2, 3);
			a /= b;
			TEST_ENSURE(a[0] == (Real)1 / 4 && a[1] == (Real)2 / 5 &&
				a[2] == (Real)3 / 6);

			// Vector op scalar

			a.set(1, 2, 3);
			a += 4;
			TEST_ENSURE(a[0] == (Real)1 + 4 && a[1] == (Real)2 + 4 &&
				a[2] == (Real)3 + 4);

			a.set(1, 2, 3);
			a -= 4;
			TEST_ENSURE(a[0] == (Real)1 - 4 && a[1] == (Real)2 - 4 &&
				a[2] == (Real)3 - 4);

			a.set(1, 2, 3);
			a *= 4;
			TEST_ENSURE(a[0] == (Real)1 * 4 && a[1] == (Real)2 * 4 &&
				a[2] == (Real)3 * 4);

			a.set(1, 2, 3);
			a /= 4;
			TEST_ENSURE(a[0] == (Real)1 / 4 && a[1] == (Real)2 / 4 &&
				a[2] == (Real)3 / 4);

			a.set(1, 2, 3);
			a = -a;
			TEST_ENSURE(a[0] == (Real)-1 && a[1] == (Real)-2 &&
				a[2] == (Real)-3);
		};

		void testVectorArithmetic()
		{
			// Test the expression templates

			Vector<Real, 3> a = Vector<Real, 3>(1, 2, 3);
			Vector<Real, 3> b = Vector<Real, 3>(4, 5, 6);

			Vector<Real, 3> c(a - b);
			TEST_ENSURE(c[0] == -3 && c[1] == -3 && c[2] == -3);

			c = a * b;
			TEST_ENSURE(c[0] == 4 && c[1] == 10 && c[2] == 18);

			c = a / b;
			TEST_ENSURE(c[0] == (Real)1/4 &&
				c[1] == (Real)2/5 && c[2] == (Real)3/6);

			c = a + b;
			TEST_ENSURE(c[0] == 5 && c[1] == 7 && c[2] == 9);

			c = (a + b) + 5;
			TEST_ENSURE(c[0] == 10 && c[1] == 12 && c[2] == 14);

			c = 5 + (a + b);
			TEST_ENSURE(c[0] == 10 && c[1] == 12 && c[2] == 14);

			c = (a + b) * 5;
			TEST_ENSURE(c[0] == 25 && c[1] == 35 && c[2] == 45);

			c = 5 * (a + b);
			TEST_ENSURE(c[0] == 25 && c[1] == 35 && c[2] == 45);

			c = -a;
			TEST_ENSURE(c[0] == -1 && c[1] == -2 && c[2] == -3);
		}

		void testVectorArray()
		{
			using Real = float;

			Vector<Real, 3> a = Vector<Real, 3>(-1, -2, 3);

			Vector<Real, 3> b;

			b = mabs(a);

			TEST_ENSURE(
				b[0] == mabs(a[0]) &&
				b[1] == mabs(a[1]) &&
				b[2] == mabs(a[2]));

			a.set(0.25, 0.5, 0.75);

			b = exp(a);

			TEST_ENSURE(
				b[0] == std::exp(a[0]) &&
				b[1] == std::exp(a[1]) &&
				b[2] == std::exp(a[2]));

			b = log(a);

			TEST_ENSURE(
				b[0] == std::log(a[0]) &&
				b[1] == std::log(a[1]) &&
				b[2] == std::log(a[2]));

			b = pow(a, a);

			TEST_ENSURE(
				b[0] == std::pow(a[0], a[0]) &&
				b[1] == std::pow(a[1], a[1]) &&
				b[2] == std::pow(a[2], a[2]));

			b = pow(a, 2);

			TEST_ENSURE(
				b[0] == std::pow(a[0], (Real)2) &&
				b[1] == std::pow(a[1], (Real)2) &&
				b[2] == std::pow(a[2], (Real)2));

			b = sqrt(a);

			TEST_ENSURE(
				b[0] == std::sqrt(a[0]) &&
				b[1] == std::sqrt(a[1]) &&
				b[2] == std::sqrt(a[2]));

			b = floor(a);

			TEST_ENSURE(
				b[0] == std::floor(a[0]) &&
				b[1] == std::floor(a[1]) &&
				b[2] == std::floor(a[2]));

			b = sin(a);

			TEST_ENSURE(
				b[0] == std::sin(a[0]) &&
				b[1] == std::sin(a[1]) &&
				b[2] == std::sin(a[2]));

			b = cos(a);

			TEST_ENSURE(
				b[0] == std::cos(a[0]) &&
				b[1] == std::cos(a[1]) &&
				b[2] == std::cos(a[2]));

			b = tan(a);

			TEST_ENSURE(
				b[0] == std::tan(a[0]) &&
				b[1] == std::tan(a[1]) &&
				b[2] == std::tan(a[2]));

			b = asin(a);

			TEST_ENSURE(
				b[0] == std::asin(a[0]) &&
				b[1] == std::asin(a[1]) &&
				b[2] == std::asin(a[2]));

			b = acos(a);

			TEST_ENSURE(
				b[0] == std::acos(a[0]) &&
				b[1] == std::acos(a[1]) &&
				b[2] == std::acos(a[2]));

			b = Pastel::atan2(a, a);

			TEST_ENSURE(
				b[0] == std::atan2(a[0], a[0]) &&
				b[1] == std::atan2(a[1], a[1]) &&
				b[2] == std::atan2(a[2], a[2]));

			b = inverse(a);

			TEST_ENSURE(
				b[0] == inverse(a[0]) &&
				b[1] == inverse(a[1]) &&
				b[2] == inverse(a[2]));
		}

		template <int N>
		void testVectorArithmetic2()
		{
			integer size = (N == Dynamic) ? 100 : N;

			Vector<Real, N> a(ofDimension(size));
			
			for (integer i = 0;i < size;++i)
			{
				a[i] = i;
			}

			Vector<Real, N> b = a;

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
				TEST_ENSURE_OP(contentsDiffer, ==, 0);
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
				TEST_ENSURE_OP(contentsDiffer, ==, 0);
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
				TEST_ENSURE_OP(contentsDiffer, ==, 0);
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
				TEST_ENSURE_OP(contentsDiffer, ==, 0);
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
				TEST_ENSURE_OP(contentsDiffer, ==, 0);
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
				TEST_ENSURE_OP(contentsDiffer, ==, 0);
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
				TEST_ENSURE_OP(contentsDiffer, ==, 0);
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
				TEST_ENSURE_OP(contentsDiffer, ==, 0);
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
				TEST_ENSURE_OP(contentsDiffer, ==, 0);
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
				TEST_ENSURE_OP(contentsDiffer, ==, 0);
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
				TEST_ENSURE_OP(contentsDiffer, ==, 0);
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
				TEST_ENSURE_OP(contentsDiffer, ==, 0);
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
				TEST_ENSURE_OP(contentsDiffer, ==, 0);
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
				TEST_ENSURE_OP(contentsDiffer, ==, 0);
			}

			// Vector / Scalar.
			{
				Vector<Real, N> c = b / 5;
				integer contentsDiffer = 0;
				for (integer i = 0;i < size;++i)
				{
					Real five = 5;
					const Real invFactor = 1 / five;
					const Real left = i;

					if (c[i] != left * invFactor)
					{
						//log() << c[i] << " != " << i * invFactor << logNewLine;
						//log() << i << " / " << size << logNewLine;

						++contentsDiffer;
					}
				}
				TEST_ENSURE_OP(contentsDiffer, ==, 0);
			}

			// Vector /= Scalar.
			{
				Vector<Real, N> c = b;
				c /= 5;
				integer contentsDiffer = 0;
				for (integer i = 0;i < size;++i)
				{
					Real invFactor = (Real)1 / 5;
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
				TEST_ENSURE_OP(contentsDiffer, ==, 0);
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
				TEST_ENSURE_OP(contentsDiffer, ==, 0);
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
				TEST_ENSURE_OP(contentsDiffer, ==, 0);
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
				TEST_ENSURE_OP(contentsDiffer, ==, 0);
			}

			// Scalar / Vector.
			{
				Vector<Real, N> c = 5 / b;
				integer contentsDiffer = 0;
				for (integer i = 1;i < size;++i)
				{
					Real left = 5;
					if (c[i] != left / i)
					{
						//log() << c[i] << " != " << left / i << logNewLine;
						++contentsDiffer;
					}
				}
				TEST_ENSURE_OP(contentsDiffer, ==, 0);
			}
		}

		template <int N>
		void testCollaboration()
		{
			const integer size = (N == Dynamic) ? 10 : N;

			Vector<Real, N> a(ofDimension(size));

			// Static-sized vectors must be
			// constructible from dynamic-sized vectors
			// and vice versa. Same thing for
			// assignment.
			{
				Vector<Real> a(ofDimension(size));
				Vector<Real, N> b(a);
				Vector<Real> c(b);

				a = b;
				b = a;

				Tuple<Real, N> d(ofDimension(size));
				Tuple<Real> e(d);
				Tuple<Real, N> f(e);
				e = d;
				d = e;
			}

			Vector<Real, N> b = a;

			Vector<Real, N> e = a;
			TEST_ENSURE_OP(e.size(), ==, size);

			Tuple<Real, N> c = b.asTuple();
			TEST_ENSURE_OP(c.size(), ==, size);

			Tuple<Real, N> d = c;
			TEST_ENSURE_OP(c.size(), ==, size);

			d = c;
			TEST_ENSURE_OP(d.size(), ==, size);

			Tuple<Real, N> f = d;
			TEST_ENSURE_OP(f.size(), ==, size);

			{
				// Vectors must be constructible
				// from Tuples.
				Tuple<Real, N> a(ofDimension(size));
				Vector<Real, N> b(a);

				// The N does not need to match.
				Tuple<Real> c(ofDimension(size));
				Vector<Real, N> d(c);
				Tuple<Real> e(asTuple(d));
			}

			// Vector contains a Tuple.
			{
				Vector<Real, N> a(ofDimension(size));
				Tuple<Real, N> b(a.asTuple());
				TEST_ENSURE(b == a.asTuple());
				Tuple<Real, N> c(asTuple(a));
				TEST_ENSURE(c == a.asTuple());
			}
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("Vector", test);
	}

	CallFunction run(addTest);

}
