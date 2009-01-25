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
		const Triangle<2, Real> triangle(
			Point<2, Real>(1, 1),
			Point<2, Real>(3, 1),
			Point<2, Real>(3, 3));

		const Line<2, Real> a(
			Point<2, Real>(-4, 2),
			Vector<2, Real>(2, 0));

		const Line<2, Real> b(
			Point<2, Real>(0, 4),
			Vector<2, Real>(1, -1));

		const Line<2, Real> c(
			Point<2, Real>(2, -1),
			Vector<2, Real>(0, 2));

		const Line<2, Real> d(
			Point<2, Real>(2, 7),
			Vector<2, Real>(0, -3));

		const Line<2, Real> e(
			Point<2, Real>(4, 6),
			Vector<2, Real>(-1, -3));

		const Line<2, Real> f(
			Point<2, Real>(6, -2),
			Vector<2, Real>(-2, 2));

		const Line<2, Real> g(
			Point<2, Real>(6, 3),
			Vector<2, Real>(-2, -1));

		const Line<2, Real> h(
			Point<2, Real>(6, 5),
			Vector<2, Real>(-2, -2));

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
		const Triangle<2, Real> triangle(
			Point<2, Real>(1, 1),
			Point<2, Real>(3, 1),
			Point<2, Real>(3, 3));

		const Line<2, Real> a(
			Point<2, Real>(0, 0),
			Vector<2, Real>(1, 1));

		const Line<2, Real> b(
			Point<2, Real>(1, -1),
			Vector<2, Real>(2, 0));

		const Line<2, Real> c(
			Point<2, Real>(2, -4),
			Vector<2, Real>(0, 2));

		const Line<2, Real> d(
			Point<2, Real>(-2, 2),
			Vector<2, Real>(-2, 0));

		const Line<2, Real> e(
			Point<2, Real>(5, 2),
			Vector<2, Real>(2, 0));

		const Line<2, Real> f(
			Point<2, Real>(6, 1),
			Vector<2, Real>(-1, 0));

		const Line<2, Real> g(
			Point<2, Real>(4, 1),
			Vector<2, Real>(0, 2));

		const Line<2, Real> h(
			Point<2, Real>(4, 4),
			Vector<2, Real>(-2, 0));

		const Line<2, Real> i(
			Point<2, Real>(2, 6),
			Vector<2, Real>(0, 2));

		const Line<2, Real> j(
			Point<2, Real>(-1, 3),
			Vector<2, Real>(0, -2));

		const Line<2, Real> k(
			Point<2, Real>(2, 0),
			Vector<2, Real>(-2, -2));

		const Line<2, Real> l(
			Point<2, Real>(2, 0),
			Vector<2, Real>(2, 2));

		const Line<2, Real> m(
			Point<2, Real>(1, 2),
			Vector<2, Real>(1, 1));

		const Line<2, Real> n(
			Point<2, Real>(3, 0),
			Vector<2, Real>(0, 1));

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
		const Triangle<3, Real> triangle(
			Point<3, Real>(1, 1, 1),
			Point<3, Real>(3, 1, 1),
			Point<3, Real>(3, 3, 1));

		const Line<3, Real> a(
			Point<3, Real>(1, 1, 0),
			Vector<3, Real>(0, 0, 1));

		const Line<3, Real> b(
			Point<3, Real>(2, 1, 0),
			Vector<3, Real>(0, 0, 1));

		const Line<3, Real> c(
			Point<3, Real>(3, 1, 0),
			Vector<3, Real>(0, 0, 1));

		const Line<3, Real> d(
			Point<3, Real>(3, 2, 0),
			Vector<3, Real>(0, 0, 1));

		const Line<3, Real> e(
			Point<3, Real>(3, 3, 0),
			Vector<3, Real>(0, 0, 1));

		const Line<3, Real> f(
			Point<3, Real>(2, 2, 0),
			Vector<3, Real>(0, 0, 1));

		const Line<3, Real> g(
			Point<3, Real>(Real(5, 2), Real(3, 2), 0),
			Vector<3, Real>(0, 0, 1));

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
		const Triangle<3, Real> triangle(
			Point<3, Real>(1, 1, 1),
			Point<3, Real>(3, 1, 1),
			Point<3, Real>(3, 3, 1));

		const Line<3, Real> a(
			Point<3, Real>(4, 2, 1),
			Vector<3, Real>(-1, 0, 0));

		const Line<3, Real> b(
			Point<3, Real>(4, 2, 1),
			Vector<3, Real>(1, 0, 0));

		const Line<3, Real> c(
			Point<3, Real>(0, 2, 1),
			Vector<3, Real>(1, 0, 0));

		const Line<3, Real> d(
			Point<3, Real>(1, 0, 1),
			Vector<3, Real>(1, 1, 0));

		const Line<3, Real> e(
			Point<3, Real>(1, 3, 0),
			Vector<3, Real>(0, 0, 1));

		const Line<3, Real> f(
			Point<3, Real>(2, 0, 0),
			Vector<3, Real>(0, 0, 1));

		const Line<3, Real> g(
			Point<3, Real>(4, 1, 0),
			Vector<3, Real>(0, 0, 1));

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
