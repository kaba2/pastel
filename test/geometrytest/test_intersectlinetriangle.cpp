#include "pastelgeometrytest.h"

#include <pastel/geometry/intersect_line_triangle.h>
#include "pastel/math/rational.h"
#include "pastel/math/rational_tools.h"
#include "pastel/math/biginteger.h"
#include "pastel/math/biginteger_tools.h"

#include <iostream>

using namespace Pastel;

namespace
{

	typedef Rational<BigInteger> Real;

	void testPositive2()
	{
		/*
		const Triangle<Real, 2> triangle(
			Vector<Real, 2>(1, 1),
			Vector<Real, 2>(3, 1),
			Vector<Real, 2>(3, 3));

		const Line<Real, 2> a(
			Vector<Real, 2>(-4, 2),
			Vector<Real, 2>(2, 0));

		const Line<Real, 2> b(
			Vector<Real, 2>(0, 4),
			Vector<Real, 2>(1, -1));

		const Line<Real, 2> c(
			Vector<Real, 2>(2, -1),
			Vector<Real, 2>(0, 2));

		const Line<Real, 2> d(
			Vector<Real, 2>(2, 7),
			Vector<Real, 2>(0, -3));

		const Line<Real, 2> e(
			Vector<Real, 2>(4, 6),
			Vector<Real, 2>(-1, -3));

		const Line<Real, 2> f(
			Vector<Real, 2>(6, -2),
			Vector<Real, 2>(-2, 2));

		const Line<Real, 2> g(
			Vector<Real, 2>(6, 3),
			Vector<Real, 2>(-2, -1));

		const Line<Real, 2> h(
			Vector<Real, 2>(6, 5),
			Vector<Real, 2>(-2, -2));

		Real t(0);
		REPORT(!intersect(a, triangle, t));
		REPORT(t != 3);
		REPORT(!intersect(b, triangle, t));
		REPORT(t != 2);
		REPORT(!intersect(c, triangle, t));
		REPORT(t != 1);
		REPORT(!intersect(d, triangle, t));
		REPORT(t != Real(5, 3));
		REPORT(!intersect(e, triangle, t));
		REPORT(t != 1);
		REPORT(!intersect(f, triangle, t));
		REPORT(t != Real(3, 2));
		REPORT(!intersect(g, triangle, t));
		REPORT(t != Real(3, 2));
		REPORT(!intersect(h, triangle, t));
		REPORT(t != Real(3, 2));
		*/
	}

	void testNegative2()
	{
		/*
		const Triangle<Real, 2> triangle(
			Vector<Real, 2>(1, 1),
			Vector<Real, 2>(3, 1),
			Vector<Real, 2>(3, 3));

		const Line<Real, 2> a(
			Vector<Real, 2>(0, 0),
			Vector<Real, 2>(1, 1));

		const Line<Real, 2> b(
			Vector<Real, 2>(1, -1),
			Vector<Real, 2>(2, 0));

		const Line<Real, 2> c(
			Vector<Real, 2>(2, -4),
			Vector<Real, 2>(0, 2));

		const Line<Real, 2> d(
			Vector<Real, 2>(-2, 2),
			Vector<Real, 2>(-2, 0));

		const Line<Real, 2> e(
			Vector<Real, 2>(5, 2),
			Vector<Real, 2>(2, 0));

		const Line<Real, 2> f(
			Vector<Real, 2>(6, 1),
			Vector<Real, 2>(-1, 0));

		const Line<Real, 2> g(
			Vector<Real, 2>(4, 1),
			Vector<Real, 2>(0, 2));

		const Line<Real, 2> h(
			Vector<Real, 2>(4, 4),
			Vector<Real, 2>(-2, 0));

		const Line<Real, 2> i(
			Vector<Real, 2>(2, 6),
			Vector<Real, 2>(0, 2));

		const Line<Real, 2> j(
			Vector<Real, 2>(-1, 3),
			Vector<Real, 2>(0, -2));

		const Line<Real, 2> k(
			Vector<Real, 2>(2, 0),
			Vector<Real, 2>(-2, -2));

		const Line<Real, 2> l(
			Vector<Real, 2>(2, 0),
			Vector<Real, 2>(2, 2));

		const Line<Real, 2> m(
			Vector<Real, 2>(1, 2),
			Vector<Real, 2>(1, 1));

		const Line<Real, 2> n(
			Vector<Real, 2>(3, 0),
			Vector<Real, 2>(0, 1));

		Real t(0);
		REPORT(intersect(a, triangle, t));
		REPORT(intersect(b, triangle, t));
		REPORT(intersect(c, triangle, t));
		REPORT(intersect(d, triangle, t));
		REPORT(intersect(e, triangle, t));
		REPORT(intersect(f, triangle, t));
		REPORT(intersect(g, triangle, t));
		REPORT(intersect(h, triangle, t));
		REPORT(intersect(i, triangle, t));
		REPORT(intersect(j, triangle, t));
		REPORT(intersect(k, triangle, t));
		REPORT(intersect(l, triangle, t));
		REPORT(intersect(m, triangle, t));
		REPORT(intersect(n, triangle, t));
		*/
	}

	void testPositive3()
	{
		const Triangle<Real, 3> triangle(
			Vector<Real, 3>(1, 1, 1),
			Vector<Real, 3>(3, 1, 1),
			Vector<Real, 3>(3, 3, 1));

		const Line<Real, 3> a(
			Vector<Real, 3>(1, 1, 0),
			Vector<Real, 3>(0, 0, 1));

		const Line<Real, 3> b(
			Vector<Real, 3>(2, 1, 0),
			Vector<Real, 3>(0, 0, 1));

		const Line<Real, 3> c(
			Vector<Real, 3>(3, 1, 0),
			Vector<Real, 3>(0, 0, 1));

		const Line<Real, 3> d(
			Vector<Real, 3>(3, 2, 0),
			Vector<Real, 3>(0, 0, 1));

		const Line<Real, 3> e(
			Vector<Real, 3>(3, 3, 0),
			Vector<Real, 3>(0, 0, 1));

		const Line<Real, 3> f(
			Vector<Real, 3>(2, 2, 0),
			Vector<Real, 3>(0, 0, 1));

		const Line<Real, 3> g(
			Vector<Real, 3>(Real(5, 2), Real(3, 2), 0),
			Vector<Real, 3>(0, 0, 1));

		Real t(0);
		Real u(0);
		Real v(0);
		REPORT(!intersect(a, triangle, t, u, v));
		REPORT(t != 1);
		REPORT(!intersect(b, triangle, t, u, v));
		REPORT(t != 1);
		REPORT(!intersect(c, triangle, t, u, v));
		REPORT(t != 1);
		REPORT(!intersect(d, triangle, t, u, v));
		REPORT(t != 1);
		REPORT(!intersect(e, triangle, t, u, v));
		REPORT(t != 1);
		REPORT(!intersect(f, triangle, t, u, v));
		REPORT(t != 1);
		REPORT(!intersect(g, triangle, t, u, v));
		REPORT(t != 1);
	}

	void testNegative3()
	{
		const Triangle<Real, 3> triangle(
			Vector<Real, 3>(1, 1, 1),
			Vector<Real, 3>(3, 1, 1),
			Vector<Real, 3>(3, 3, 1));

		const Line<Real, 3> a(
			Vector<Real, 3>(4, 2, 1),
			Vector<Real, 3>(-1, 0, 0));

		const Line<Real, 3> b(
			Vector<Real, 3>(4, 2, 1),
			Vector<Real, 3>(1, 0, 0));

		const Line<Real, 3> c(
			Vector<Real, 3>(0, 2, 1),
			Vector<Real, 3>(1, 0, 0));

		const Line<Real, 3> d(
			Vector<Real, 3>(1, 0, 1),
			Vector<Real, 3>(1, 1, 0));

		const Line<Real, 3> e(
			Vector<Real, 3>(1, 3, 0),
			Vector<Real, 3>(0, 0, 1));

		const Line<Real, 3> f(
			Vector<Real, 3>(2, 0, 0),
			Vector<Real, 3>(0, 0, 1));

		const Line<Real, 3> g(
			Vector<Real, 3>(4, 1, 0),
			Vector<Real, 3>(0, 0, 1));

		Real t(0);
		Real u(0);
		Real v(0);
		REPORT(intersect(a, triangle, t, u, v));
		REPORT(intersect(b, triangle, t, u, v));
		REPORT(intersect(c, triangle, t, u, v));
		REPORT(intersect(d, triangle, t, u, v));
		REPORT(intersect(e, triangle, t, u, v));
		REPORT(intersect(f, triangle, t, u, v));
		REPORT(intersect(g, triangle, t, u, v));
	}

	void testBegin()
	{
		testPositive2();
		testNegative2();
		testPositive3();
		testNegative3();
	}

	void testAdd()
	{
		geometryTestList().add("IntersectLineTriangle", testBegin);
	}

	CallFunction run(testAdd);

}
