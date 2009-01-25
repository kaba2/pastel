#include "pastelmathtest.h"

#include "pastel/math/polynomial.h"
#include "pastel/math/polynomial_tools.h"
#include "pastel/math/rational.h"

using namespace Pastel;

namespace
{
	
	typedef real Real;
	typedef Polynomial<Real> Poly;

	void testBasic()
	{
		Poly a;
		REPORT1(a.size() != 1, a.size());
		REPORT(a[0] != 0);
		REPORT1(a.degree() != 0, a.degree());

		a[0] = 1;
		REPORT1(a.degree() != 0, a.degree());
		
		a.set(1, 0);
		REPORT1(a.size() != 2, a.size());
		REPORT1(a.degree() != 0, a.degree());
		
		a.set(5, 0);
		REPORT1(a.size() != 6, a.size());
		REPORT1(a.degree() != 0, a.degree());
		a.set(5, 1);
		REPORT1(a.size() != 6, a.size());
		REPORT1(a.degree() != 5, a.degree());

		a.setSize(10);
		REPORT1(a.size() != 10, a.size());
		REPORT1(a.degree() != 5, a.degree());

		a.setSize(4);
		REPORT1(a.size() != 4, a.size());
		REPORT1(a.degree() != 0, a.degree());
	}

	void testOperators()
	{
		Poly a;
		a.set(0, 1);

		REPORT1(a.degree() != 0, a.degree());
		a <<= 1;
		REPORT1(a.degree() != 1, a.degree());
		a <<= 2;
		REPORT1(a.degree() != 3, a.degree());

		{
			Poly b;
			b.set(3, 1);
			REPORT(a != b);
		}
		a >>= 3;
		{
			Poly b;
			b.set(0, 1);
			REPORT(a != b);
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

			REPORT(e != c + d);
		}
		{
			Poly e;
			e.set(0, 3 - 1);
			e.set(1, 7 - 5);
			e.set(2, 5 - 0);

			REPORT(e != c - d);
		}
		{
			Poly e;
			e.set(0, 3);
			e.set(1, 22);
			e.set(2, 40);
			e.set(3, 25);

			REPORT(e != c * d);
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

			REPORT(a != b);
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

			REPORT(c != quotient);
			REPORT(d != remainder);
		}
		{
			const integer maxDegree = 10;

			integer failings = 0;
			for (integer i = 0;i < 10000;++i)
			{
				Poly poly = randomPolynomial<Real>(randomInteger() % (maxDegree + 1));
				Poly divider = randomPolynomial<Real>(randomInteger() % (maxDegree + 1));
				
				Poly quotient;
				Poly remainder;
				divide(poly, divider, quotient, remainder);

				Poly error = poly - (quotient * divider + remainder);
				
				if (error.degree() != 0)
				{
					++failings;
				}
			}
			
			REPORT1(failings > 200, failings);
		}
	}

	void testBegin()
	{
		testBasic();
		testOperators();
		testTools();
	}

	void testAdd()
	{
		mathTestList().add("Polynomial", testBegin);
	}

	CallFunction run(testAdd);

}
