// Description: Testing for Polynomial
// DocumentationOf: polynomial.h

#include "test_pastelmath.h"

#include "pastel/math/polynomial.h"
#include "pastel/math/polynomial_tools.h"
#include "pastel/sys/rational.h"

using namespace Pastel;

namespace
{

	using Real = real;
	using Poly = Polynomial<Real>;

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
			testBasic();
			testOperators();
			testTools();
		}

		void testBasic()
		{
			Poly a;
			TEST_ENSURE_OP(a.size(), ==, 1);
			TEST_ENSURE_OP(a[0], ==, 0);
			TEST_ENSURE_OP(a.degree(), ==, 0);

			a[0] = 1;
			TEST_ENSURE_OP(a.degree(), ==, 0);

			a.set(1, 0);
			TEST_ENSURE_OP(a.size(), ==, 2);
			TEST_ENSURE_OP(a.degree(), ==, 0);

			a.set(5, 0);
			TEST_ENSURE_OP(a.size(), ==, 6);
			TEST_ENSURE_OP(a.degree(), ==, 0);
			a.set(5, 1);
			TEST_ENSURE_OP(a.size(), ==, 6);
			TEST_ENSURE_OP(a.degree(), ==, 5);

			a.setSize(10);
			TEST_ENSURE_OP(a.size(), ==, 10);
			TEST_ENSURE_OP(a.degree(), ==, 5);

			a.setSize(4);
			TEST_ENSURE_OP(a.size(), ==, 4);
			TEST_ENSURE_OP(a.degree(), ==, 0);
		}

		void testOperators()
		{
			Poly a;
			a.set(0, 1);

			TEST_ENSURE_OP(a.degree(), ==, 0);
			a <<= 1;
			TEST_ENSURE_OP(a.degree(), ==, 1);
			a <<= 2;
			TEST_ENSURE_OP(a.degree(), ==, 3);

			{
				Poly b;
				b.set(3, 1);
				TEST_ENSURE(a == b);
			}
			a >>= 3;
			{
				Poly b;
				b.set(0, 1);
				TEST_ENSURE(a == b);
			}

			Poly c;
			c.set(0, 3);
			c.set(1, 7);
			c.set(2, 5);

			Poly d;
			d.set(0, 1);
			d.set(1, 5);

			{
				Poly e;
				e.set(0, 3 + 1);
				e.set(1, 7 + 5);
				e.set(2, 5 + 0);

				TEST_ENSURE(e == c + d);
			}
			{
				Poly e;
				e.set(0, 3 - 1);
				e.set(1, 7 - 5);
				e.set(2, 5 - 0);

				TEST_ENSURE(e == c - d);
			}
			{
				Poly e;
				e.set(0, 3);
				e.set(1, 22);
				e.set(2, 40);
				e.set(3, 25);

				TEST_ENSURE(e == c * d);
			}
		}

		void testTools()
		{
			{
				Poly a;
				a.set(0, 3);
				a.set(1, 7);
				a.set(2, 5);

				differentiate(a);

				Poly b;
				b.set(0, 7);
				b.set(1, 10);

				TEST_ENSURE(a == b);
			}
			{
				Poly a;
				a.set(0, 3);
				a.set(1, 7);
				a.set(2, 5);

				Poly b;
				b.set(0, 1);
				b.set(1, 5);

				Poly quotient;
				Poly remainder;

				divide(a, b, quotient, remainder);

				Poly c;
				c.set(0, Real(6) / 5);
				c.set(1, 1);

				Poly d;
				d.set(0, Real(9) / 5);

				TEST_ENSURE(c == quotient);
				TEST_ENSURE(d == remainder);
			}
			{
				const integer maxDegree = 10;

				integer failings = 0;
				for (integer i = 0;i < 10000;++i)
				{
					Poly poly = randomPolynomial<Real>(randomInteger(maxDegree + 1));
					Poly divider = randomPolynomial<Real>(randomInteger(maxDegree + 1));

					Poly quotient;
					Poly remainder;
					divide(poly, divider, quotient, remainder);

					Poly error = poly - (quotient * divider + remainder);

					if (error.degree() != 0)
					{
						++failings;
					}
				}

				TEST_ENSURE_OP(failings, <=, 200);
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
		testRunner().add("Polynomial", test);
	}

	CallFunction run(addTest);

}
