// Description: Testing for Polynomial
// DocumentationOf: polynomial.h

#include "test/test_init.h"

#include "pastel/math/polynomial.h"
#include "pastel/sys/rational.h"

namespace
{

	using Real = real;
	using Poly = Polynomial<Real>;

}

TEST_CASE("Basic (Polynomial)")
{
	Poly a;
	REQUIRE(a.size() == 1);
	REQUIRE(a[0] == 0);
	REQUIRE(a.degree() == 0);

	a[0] = 1;
	REQUIRE(a.degree() == 0);

	a.set(1, 0);
	REQUIRE(a.size() == 2);
	REQUIRE(a.degree() == 0);

	a.set(5, 0);
	REQUIRE(a.size() == 6);
	REQUIRE(a.degree() == 0);
	a.set(5, 1);
	REQUIRE(a.size() == 6);
	REQUIRE(a.degree() == 5);

	a.setSize(10);
	REQUIRE(a.size() == 10);
	REQUIRE(a.degree() == 5);

	a.setSize(4);
	REQUIRE(a.size() == 4);
	REQUIRE(a.degree() == 0);
}

TEST_CASE("Operators (Polynomial)")
{
	Poly a;
	a.set(0, 1);

	REQUIRE(a.degree() == 0);
	a <<= 1;
	REQUIRE(a.degree() == 1);
	a <<= 2;
	REQUIRE(a.degree() == 3);

	{
		Poly b;
		b.set(3, 1);
		REQUIRE(a == b);
	}
	a >>= 3;
	{
		Poly b;
		b.set(0, 1);
		REQUIRE(a == b);
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

		REQUIRE(e == c + d);
	}
	{
		Poly e;
		e.set(0, 3 - 1);
		e.set(1, 7 - 5);
		e.set(2, 5 - 0);

		REQUIRE(e == c - d);
	}
	{
		Poly e;
		e.set(0, 3);
		e.set(1, 22);
		e.set(2, 40);
		e.set(3, 25);

		REQUIRE(e == c * d);
	}
}

TEST_CASE("Tools (Polynomial)")
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

		REQUIRE(a == b);
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

		REQUIRE(c == quotient);
		REQUIRE(d == remainder);
	}
	{
		integer maxDegree = 10;

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

		REQUIRE(failings <= 200);
	}
}
