// Description: Testing for line-triangle intersection
// DocumentationOf: intersect_line_triangle.h

#include "test/test_init.h"

#include "pastel/geometry/intersect/intersect_line_triangle.h"
#include "pastel/sys/rational.h"

#include <iostream>

namespace
{

	using Real = Rational<integer>;

}

TEST_CASE("Positive2 (intersect_line_triangle)")
{
	/*
	PASTEL_TRIANGLE(Real, 2) triangle(
		Vector<Real, 2>(1, 1),
		Vector<Real, 2>(3, 1),
		Vector<Real, 2>(3, 3));

	Line<Real, 2> a(
		Vector<Real, 2>(-4, 2),
		Vector<Real, 2>(2, 0));

	Line<Real, 2> b(
		Vector<Real, 2>(0, 4),
		Vector<Real, 2>(1, -1));

	Line<Real, 2> c(
		Vector<Real, 2>(2, -1),
		Vector<Real, 2>(0, 2));

	Line<Real, 2> d(
		Vector<Real, 2>(2, 7),
		Vector<Real, 2>(0, -3));

	Line<Real, 2> e(
		Vector<Real, 2>(4, 6),
		Vector<Real, 2>(-1, -3));

	Line<Real, 2> f(
		Vector<Real, 2>(6, -2),
		Vector<Real, 2>(-2, 2));

	Line<Real, 2> g(
		Vector<Real, 2>(6, 3),
		Vector<Real, 2>(-2, -1));

	Line<Real, 2> h(
		Vector<Real, 2>(6, 5),
		Vector<Real, 2>(-2, -2));

	Real t(0);
	REQUIRE(intersect(a, triangle, t));
	REQUIRE(t == 3);
	REQUIRE(intersect(b, triangle, t));
	REQUIRE(t == 2);
	REQUIRE(intersect(c, triangle, t));
	REQUIRE(t == 1);
	REQUIRE(intersect(d, triangle, t));
	REQUIRE(t == Real(5, 3));
	REQUIRE(intersect(e, triangle, t));
	REQUIRE(t == 1);
	REQUIRE(intersect(f, triangle, t));
	REQUIRE(t == Real(3, 2));
	REQUIRE(intersect(g, triangle, t));
	REQUIRE(t == Real(3, 2));
	REQUIRE(intersect(h, triangle, t));
	REQUIRE(t == Real(3, 2));

	*/
}

TEST_CASE("Negative2 (intersect_line_triangle)")
{
	/*
	PASTEL_TRIANGLE(Real, 2) triangle(
		Vector<Real, 2>(1, 1),
		Vector<Real, 2>(3, 1),
		Vector<Real, 2>(3, 3));

	Line<Real, 2> a(
		Vector<Real, 2>(0, 0),
		Vector<Real, 2>(1, 1));

	Line<Real, 2> b(
		Vector<Real, 2>(1, -1),
		Vector<Real, 2>(2, 0));

	Line<Real, 2> c(
		Vector<Real, 2>(2, -4),
		Vector<Real, 2>(0, 2));

	Line<Real, 2> d(
		Vector<Real, 2>(-2, 2),
		Vector<Real, 2>(-2, 0));

	Line<Real, 2> e(
		Vector<Real, 2>(5, 2),
		Vector<Real, 2>(2, 0));

	Line<Real, 2> f(
		Vector<Real, 2>(6, 1),
		Vector<Real, 2>(-1, 0));

	Line<Real, 2> g(
		Vector<Real, 2>(4, 1),
		Vector<Real, 2>(0, 2));

	Line<Real, 2> h(
		Vector<Real, 2>(4, 4),
		Vector<Real, 2>(-2, 0));

	Line<Real, 2> i(
		Vector<Real, 2>(2, 6),
		Vector<Real, 2>(0, 2));

	Line<Real, 2> j(
		Vector<Real, 2>(-1, 3),
		Vector<Real, 2>(0, -2));

	Line<Real, 2> k(
		Vector<Real, 2>(2, 0),
		Vector<Real, 2>(-2, -2));

	Line<Real, 2> l(
		Vector<Real, 2>(2, 0),
		Vector<Real, 2>(2, 2));

	Line<Real, 2> m(
		Vector<Real, 2>(1, 2),
		Vector<Real, 2>(1, 1));

	Line<Real, 2> n(
		Vector<Real, 2>(3, 0),
		Vector<Real, 2>(0, 1));

	Real t(0);
	REQUIRE(!intersect(a, triangle, t));
	REQUIRE(!intersect(b, triangle, t));
	REQUIRE(!intersect(c, triangle, t));
	REQUIRE(!intersect(d, triangle, t));
	REQUIRE(!intersect(e, triangle, t));
	REQUIRE(!intersect(f, triangle, t));
	REQUIRE(!intersect(g, triangle, t));
	REQUIRE(!intersect(h, triangle, t));
	REQUIRE(!intersect(i, triangle, t));
	REQUIRE(!intersect(j, triangle, t));
	REQUIRE(!intersect(k, triangle, t));
	REQUIRE(!intersect(l, triangle, t));
	REQUIRE(!intersect(m, triangle, t));
	REQUIRE(!intersect(n, triangle, t));

	*/
}

TEST_CASE("Positive3 (intersect_line_triangle)")
{
	PASTEL_TRIANGLE(Real, 3) triangle(
		Vector<Real, 3>(1, 1, 1),
		Vector<Real, 3>(3, 1, 1),
		Vector<Real, 3>(3, 3, 1));

	Line<Real, 3> a(
		Vector<Real, 3>(1, 1, 0),
		Vector<Real, 3>(0, 0, 1));

	Line<Real, 3> b(
		Vector<Real, 3>(2, 1, 0),
		Vector<Real, 3>(0, 0, 1));

	Line<Real, 3> c(
		Vector<Real, 3>(3, 1, 0),
		Vector<Real, 3>(0, 0, 1));

	Line<Real, 3> d(
		Vector<Real, 3>(3, 2, 0),
		Vector<Real, 3>(0, 0, 1));

	Line<Real, 3> e(
		Vector<Real, 3>(3, 3, 0),
		Vector<Real, 3>(0, 0, 1));

	Line<Real, 3> f(
		Vector<Real, 3>(2, 2, 0),
		Vector<Real, 3>(0, 0, 1));

	Line<Real, 3> g(
		Vector<Real, 3>(Real(5, 2), Real(3, 2), 0),
		Vector<Real, 3>(0, 0, 1));

	Real t(0);
	Real u(0);
	Real v(0);
	REQUIRE(intersect(a, triangle, t, u, v));
	REQUIRE(t == 1);
	REQUIRE(intersect(b, triangle, t, u, v));
	REQUIRE(t == 1);
	REQUIRE(intersect(c, triangle, t, u, v));
	REQUIRE(t == 1);
	REQUIRE(intersect(d, triangle, t, u, v));
	REQUIRE(t == 1);
	REQUIRE(intersect(e, triangle, t, u, v));
	REQUIRE(t == 1);
	REQUIRE(intersect(f, triangle, t, u, v));
	REQUIRE(t == 1);
	REQUIRE(intersect(g, triangle, t, u, v));
	REQUIRE(t == 1);
}

TEST_CASE("Negative3 (intersect_line_triangle)")
{
	PASTEL_TRIANGLE(Real, 3) triangle(
		Vector<Real, 3>(1, 1, 1),
		Vector<Real, 3>(3, 1, 1),
		Vector<Real, 3>(3, 3, 1));

	Line<Real, 3> a(
		Vector<Real, 3>(4, 2, 1),
		Vector<Real, 3>(-1, 0, 0));

	Line<Real, 3> b(
		Vector<Real, 3>(4, 2, 1),
		Vector<Real, 3>(1, 0, 0));

	Line<Real, 3> c(
		Vector<Real, 3>(0, 2, 1),
		Vector<Real, 3>(1, 0, 0));

	Line<Real, 3> d(
		Vector<Real, 3>(1, 0, 1),
		Vector<Real, 3>(1, 1, 0));

	Line<Real, 3> e(
		Vector<Real, 3>(1, 3, 0),
		Vector<Real, 3>(0, 0, 1));

	Line<Real, 3> f(
		Vector<Real, 3>(2, 0, 0),
		Vector<Real, 3>(0, 0, 1));

	Line<Real, 3> g(
		Vector<Real, 3>(4, 1, 0),
		Vector<Real, 3>(0, 0, 1));

	Real t(0);
	Real u(0);
	Real v(0);
	REQUIRE(!intersect(a, triangle, t, u, v));
	REQUIRE(!intersect(b, triangle, t, u, v));
	REQUIRE(!intersect(c, triangle, t, u, v));
	REQUIRE(!intersect(d, triangle, t, u, v));
	REQUIRE(!intersect(e, triangle, t, u, v));
	REQUIRE(!intersect(f, triangle, t, u, v));
	REQUIRE(!intersect(g, triangle, t, u, v));
}

