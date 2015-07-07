// Description: Testing for Vector
// DocumentationOf: vector.h

#include "test/test_init.h"

#include "pastel/sys/rational.h"
#include "pastel/sys/vector.h"

namespace
{

	using Real = Rational<integer>;
	//using Real = real;

	template <typename Type, integer N>
	void someFunction(const Tuple<Type, N>& jorma)
	{
	}

	class Test
	{
	public:
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

		template <integer N>
		void testNullPointerStatic()
		{
			Vector<Real, N> au2(0);
			Vector<Real, N> au3(0.0);

			Vector<float, N> af2(0);
			Vector<float, N> af3(0.0);

			Vector<int, N> ai1(0);
			Vector<int, N> ai2(0.0);
		}

		template <integer N>
		void testNullPointer()
		{
			integer n = (N == Dynamic) ? 3 : N;

			Tuple<float, N> t(ofDimension(n), 0);

			Vector<float, N> c(ofDimension(n));

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
			REQUIRE(a[0] == 1);
			REQUIRE(a[1] == 2);
			REQUIRE(a[2] == 3);

			Vector<Real, 3> b = Vector<Real, 3>(4, 5, 6);
			REQUIRE(b[0] == 4);
			REQUIRE(b[1] == 5);
			REQUIRE(b[2] == 6);

			Vector<Real, 3> c(b);
			REQUIRE(c[0] == 4);
			REQUIRE(c[1] == 5);
			REQUIRE(c[2] == 6);

			a = b;
			REQUIRE(a[0] == 4);
			REQUIRE(a[1] == 5);
			REQUIRE(a[2] == 6);

			a = 3;
			REQUIRE(a[0] == 3);
			REQUIRE(a[1] == 3);
			REQUIRE(a[2] == 3);

			someFunction(asTuple(a));

			a.assign({ 1, 2, 3});
			REQUIRE(a[0] == 1);
			REQUIRE(a[1] == 2);
			REQUIRE(a[2] == 3);

			a = { 1, 2, 3 };

			Vector<float> cd(ofDimension(2));
		}

		void testVectorLowDimensional()
		{
			// Tests the low-dimension specific
			// member functions.

			{
				Vector<Real, 1> a = Vector<Real, 1>(1);
				REQUIRE(a[0] == 1);

				a.set(4);
				REQUIRE(a[0] == 4);

				a = 7;
				REQUIRE(a[0] == 7);

				Vector<Real, 1> b(8);
				REQUIRE(b[0] == 8);

				b.set(9);
				REQUIRE(b[0] == 9);
			}
			{
				Vector<Real, 2> a = Vector<Real, 2>(1, 2);
				REQUIRE(a[0] == 1);
				REQUIRE(a[1] == 2);

				a.set(4, 5);
				REQUIRE(a[0] == 4);
				REQUIRE(a[1] == 5);

				a = 7;
				REQUIRE(a[0] == 7);
				REQUIRE(a[1] == 7);

				Vector<Real, 2> b(8);
				REQUIRE(b[0] == 8);
				REQUIRE(b[1] == 8);

				b.set(9);
				REQUIRE(b[0] == 9);
				REQUIRE(b[1] == 9);
			}
			{
				Vector<Real, 3> a = Vector<Real, 3>(1, 2, 3);
				REQUIRE(a[0] == 1);
				REQUIRE(a[1] == 2);
				REQUIRE(a[2] == 3);

				a.set(4, 5, 6);
				REQUIRE(a[0] == 4);
				REQUIRE(a[1] == 5);
				REQUIRE(a[2] == 6);

				a = 7;
				REQUIRE(a[0] == 7);
				REQUIRE(a[1] == 7);
				REQUIRE(a[2] == 7);

				Vector<Real, 3> b(8);
				REQUIRE(b[0] == 8);
				REQUIRE(b[1] == 8);
				REQUIRE(b[2] == 8);

				b.set(9);
				REQUIRE(b[0] == 9);
				REQUIRE(b[1] == 9);
				REQUIRE(b[2] == 9);
			}
			{
				Vector<Real, 4> a = Vector<Real, 4>(1, 2, 3, 4);
				REQUIRE(a[0] == 1);
				REQUIRE(a[1] == 2);
				REQUIRE(a[2] == 3);
				REQUIRE(a[3] == 4);

				a.set(4, 5, 6, 7);
				REQUIRE(a[0] == 4);
				REQUIRE(a[1] == 5);
				REQUIRE(a[2] == 6);
				REQUIRE(a[3] == 7);

				a = 7;
				REQUIRE(a[0] == 7);
				REQUIRE(a[1] == 7);
				REQUIRE(a[2] == 7);
				REQUIRE(a[3] == 7);

				Vector<Real, 4> b(8);
				REQUIRE(b[0] == 8);
				REQUIRE(b[1] == 8);
				REQUIRE(b[2] == 8);
				REQUIRE(b[3] == 8);

				b.set(9);
				REQUIRE(b[0] == 9);
				REQUIRE(b[1] == 9);
				REQUIRE(b[2] == 9);
				REQUIRE(b[3] == 9);
			}
		}

		void testVectorSimpleArithmetic()
		{
			Vector<Real, 3> a = Vector<Real, 3>(1, 2, 3);
			Vector<Real, 3> b = Vector<Real, 3>(4, 5, 6);

			// Vector op Vector

			a += b;
			REQUIRE(a[0] == (Real)1 + 4);
			REQUIRE(a[1] == (Real)2 + 5);
			REQUIRE(a[2] == (Real)3 + 6);

			a.set(1, 2, 3);
			a -= b;
			REQUIRE(a[0] == (Real)1 - 4);
			REQUIRE(a[1] == (Real)2 - 5);
			REQUIRE(a[2] == (Real)3 - 6);

			a.set(1, 2, 3);
			a *= b;
			REQUIRE(a[0] == (Real)1 * 4);
			REQUIRE(a[1] == (Real)2 * 5);
			REQUIRE(a[2] == (Real)3 * 6);

			a.set(1, 2, 3);
			a /= b;
			REQUIRE(a[0] == (Real)1 / 4);
			REQUIRE(a[1] == (Real)2 / 5);
			REQUIRE(a[2] == (Real)3 / 6);

			// Vector op scalar

			a.set(1, 2, 3);
			a += 4;
			REQUIRE(a[0] == (Real)1 + 4);
			REQUIRE(a[1] == (Real)2 + 4);
			REQUIRE(a[2] == (Real)3 + 4);

			a.set(1, 2, 3);
			a -= 4;
			REQUIRE(a[0] == (Real)1 - 4);
			REQUIRE(a[1] == (Real)2 - 4);
			REQUIRE(a[2] == (Real)3 - 4);

			a.set(1, 2, 3);
			a *= 4;
			REQUIRE(a[0] == (Real)1 * 4);
			REQUIRE(a[1] == (Real)2 * 4);
			REQUIRE(a[2] == (Real)3 * 4);

			a.set(1, 2, 3);
			a /= 4;
			REQUIRE(a[0] == (Real)1 / 4);
			REQUIRE(a[1] == (Real)2 / 4);
			REQUIRE(a[2] == (Real)3 / 4);

			a.set(1, 2, 3);
			a = -a;
			REQUIRE(a[0] == (Real)-1);
			REQUIRE(a[1] == (Real)-2);
			REQUIRE(a[2] == (Real)-3);
		};

		void testVectorArithmetic()
		{
			// Test the expression templates

			Vector<Real, 3> a = Vector<Real, 3>(1, 2, 3);
			Vector<Real, 3> b = Vector<Real, 3>(4, 5, 6);

			Vector<Real, 3> c(a - b);
			REQUIRE(c[0] == -3);
			REQUIRE(c[1] == -3);
			REQUIRE(c[2] == -3);

			c = a * b;
			REQUIRE(c[0] == 4);
			REQUIRE(c[1] == 10);
			REQUIRE(c[2] == 18);

			c = a / b;
			REQUIRE(c[0] == (Real)1/4);
			REQUIRE(c[1] == (Real)2/5);
			REQUIRE(c[2] == (Real)3/6);

			c = a + b;
			REQUIRE(c[0] == 5);
			REQUIRE(c[1] == 7);
			REQUIRE(c[2] == 9);

			c = (a + b) + 5;
			REQUIRE(c[0] == 10);
			REQUIRE(c[1] == 12);
			REQUIRE(c[2] == 14);

			c = 5 + (a + b);
			REQUIRE(c[0] == 10);
			REQUIRE(c[1] == 12);
			REQUIRE(c[2] == 14);

			c = (a + b) * 5;
			REQUIRE(c[0] == 25);
			REQUIRE(c[1] == 35);
			REQUIRE(c[2] == 45);

			c = 5 * (a + b);
			REQUIRE(c[0] == 25);
			REQUIRE(c[1] == 35);
			REQUIRE(c[2] == 45);

			c = -a;
			REQUIRE(c[0] == -1);
			REQUIRE(c[1] == -2);
			REQUIRE(c[2] == -3);
		}

		void testVectorArray()
		{
			using Real = float;

			Vector<Real, 3> a = Vector<Real, 3>(-1, -2, 3);

			Vector<Real, 3> b;

			b = abs(a);

			REQUIRE(b[0] == abs(a[0]));
			REQUIRE(b[1] == abs(a[1]));
			REQUIRE(b[2] == abs(a[2]));

			a.set(0.25, 0.5, 0.75);

			b = exp(a);

			REQUIRE(b[0] == std::exp(a[0]));
			REQUIRE(b[1] == std::exp(a[1]));
			REQUIRE(b[2] == std::exp(a[2]));

			b = log(a);

			REQUIRE(b[0] == std::log(a[0]));
			REQUIRE(b[1] == std::log(a[1]));
			REQUIRE(b[2] == std::log(a[2]));

			b = pow(a, a);

			REQUIRE(b[0] == std::pow(a[0], a[0]));
			REQUIRE(b[1] == std::pow(a[1], a[1]));
			REQUIRE(b[2] == std::pow(a[2], a[2]));

			b = pow(a, 2);

			REQUIRE(b[0] == std::pow(a[0], (Real)2));
			REQUIRE(b[1] == std::pow(a[1], (Real)2));
			REQUIRE(b[2] == std::pow(a[2], (Real)2));

			b = sqrt(a);

			REQUIRE(b[0] == std::sqrt(a[0]));
			REQUIRE(b[1] == std::sqrt(a[1]));
			REQUIRE(b[2] == std::sqrt(a[2]));

			b = floor(a);

			REQUIRE(b[0] == std::floor(a[0]));
			REQUIRE(b[1] == std::floor(a[1]));
			REQUIRE(b[2] == std::floor(a[2]));

			b = sin(a);

			REQUIRE(b[0] == std::sin(a[0]));
			REQUIRE(b[1] == std::sin(a[1]));
			REQUIRE(b[2] == std::sin(a[2]));

			b = cos(a);

			REQUIRE(b[0] == std::cos(a[0]));
			REQUIRE(b[1] == std::cos(a[1]));
			REQUIRE(b[2] == std::cos(a[2]));

			b = tan(a);

			REQUIRE(b[0] == std::tan(a[0]));
			REQUIRE(b[1] == std::tan(a[1]));
			REQUIRE(b[2] == std::tan(a[2]));

			b = asin(a);

			REQUIRE(b[0] == std::asin(a[0]));
			REQUIRE(b[1] == std::asin(a[1]));
			REQUIRE(b[2] == std::asin(a[2]));

			b = acos(a);

			REQUIRE(b[0] == std::acos(a[0]));
			REQUIRE(b[1] == std::acos(a[1]));
			REQUIRE(b[2] == std::acos(a[2]));

			b = Pastel::atan2(a, a);

			REQUIRE(b[0] == std::atan2(a[0], a[0]));
			REQUIRE(b[1] == std::atan2(a[1], a[1]));
			REQUIRE(b[2] == std::atan2(a[2], a[2]));

			b = inverse(a);

			REQUIRE(b[0] == inverse(a[0]));
			REQUIRE(b[1] == inverse(a[1]));
			REQUIRE(b[2] == inverse(a[2]));
		}

		template <integer N>
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
				REQUIRE(contentsDiffer == 0);
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
				REQUIRE(contentsDiffer == 0);
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
				REQUIRE(contentsDiffer == 0);
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
				REQUIRE(contentsDiffer == 0);
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
				REQUIRE(contentsDiffer == 0);
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
				REQUIRE(contentsDiffer == 0);
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
				REQUIRE(contentsDiffer == 0);
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
				REQUIRE(contentsDiffer == 0);
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
				REQUIRE(contentsDiffer == 0);
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
				REQUIRE(contentsDiffer == 0);
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
				REQUIRE(contentsDiffer == 0);
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
				REQUIRE(contentsDiffer == 0);
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
				REQUIRE(contentsDiffer == 0);
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
				REQUIRE(contentsDiffer == 0);
			}

			// Vector / Scalar.
			{
				Vector<Real, N> c = b / 5;
				integer contentsDiffer = 0;
				for (integer i = 0;i < size;++i)
				{
					Real five = 5;
					Real invFactor = 1 / five;
					Real left = i;

					if (c[i] != left * invFactor)
					{
						//log() << c[i] << " != " << i * invFactor << logNewLine;
						//log() << i << " / " << size << logNewLine;

						++contentsDiffer;
					}
				}
				REQUIRE(contentsDiffer == 0);
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
				REQUIRE(contentsDiffer == 0);
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
				REQUIRE(contentsDiffer == 0);
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
				REQUIRE(contentsDiffer == 0);
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
				REQUIRE(contentsDiffer == 0);
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
				REQUIRE(contentsDiffer == 0);
			}
		}

		template <integer N>
		void testCollaboration()
		{
			integer size = (N == Dynamic) ? 10 : N;

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
			REQUIRE(e.size() == size);

			Tuple<Real, N> c = b.asTuple();
			REQUIRE(c.size() == size);

			Tuple<Real, N> d = c;
			REQUIRE(c.size() == size);

			d = c;
			REQUIRE(d.size() == size);

			Tuple<Real, N> f = d;
			REQUIRE(f.size() == size);

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
				REQUIRE(b == a.asTuple());
				Tuple<Real, N> c(asTuple(a));
				REQUIRE(c == a.asTuple());
			}
		}
	};

	TEST_CASE("Vector", "[Vector]")
	{
	}

}
