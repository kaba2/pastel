// Description: Testing for line-triangle intersection
// DocumentationOf: intersect_line_triangle.h

#include "test_pastelgeometry.h"

#include "pastel/geometry/intersect/intersect_line_triangle.h"
#include "pastel/sys/rational.h"
#include "pastel/sys/integer/biginteger.h"

#include <iostream>

using namespace Pastel;

namespace
{

	using Real = Rational<BigInteger>;

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
			testPositive2();
			testNegative2();
			testPositive3();
			testNegative3();
		}

		void testPositive2()
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
			TEST_ENSURE(intersect(a, triangle, t));
			TEST_ENSURE(t == 3);
			TEST_ENSURE(intersect(b, triangle, t));
			TEST_ENSURE(t == 2);
			TEST_ENSURE(intersect(c, triangle, t));
			TEST_ENSURE(t == 1);
			TEST_ENSURE(intersect(d, triangle, t));
			TEST_ENSURE(t == Real(5, 3));
			TEST_ENSURE(intersect(e, triangle, t));
			TEST_ENSURE(t == 1);
			TEST_ENSURE(intersect(f, triangle, t));
			TEST_ENSURE(t == Real(3, 2));
			TEST_ENSURE(intersect(g, triangle, t));
			TEST_ENSURE(t == Real(3, 2));
			TEST_ENSURE(intersect(h, triangle, t));
			TEST_ENSURE(t == Real(3, 2));

			*/
		}

		void testNegative2()
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
			TEST_ENSURE(!intersect(a, triangle, t));
			TEST_ENSURE(!intersect(b, triangle, t));
			TEST_ENSURE(!intersect(c, triangle, t));
			TEST_ENSURE(!intersect(d, triangle, t));
			TEST_ENSURE(!intersect(e, triangle, t));
			TEST_ENSURE(!intersect(f, triangle, t));
			TEST_ENSURE(!intersect(g, triangle, t));
			TEST_ENSURE(!intersect(h, triangle, t));
			TEST_ENSURE(!intersect(i, triangle, t));
			TEST_ENSURE(!intersect(j, triangle, t));
			TEST_ENSURE(!intersect(k, triangle, t));
			TEST_ENSURE(!intersect(l, triangle, t));
			TEST_ENSURE(!intersect(m, triangle, t));
			TEST_ENSURE(!intersect(n, triangle, t));

			*/
		}

		void testPositive3()
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
			TEST_ENSURE(intersect(a, triangle, t, u, v));
			TEST_ENSURE(t == 1);
			TEST_ENSURE(intersect(b, triangle, t, u, v));
			TEST_ENSURE(t == 1);
			TEST_ENSURE(intersect(c, triangle, t, u, v));
			TEST_ENSURE(t == 1);
			TEST_ENSURE(intersect(d, triangle, t, u, v));
			TEST_ENSURE(t == 1);
			TEST_ENSURE(intersect(e, triangle, t, u, v));
			TEST_ENSURE(t == 1);
			TEST_ENSURE(intersect(f, triangle, t, u, v));
			TEST_ENSURE(t == 1);
			TEST_ENSURE(intersect(g, triangle, t, u, v));
			TEST_ENSURE(t == 1);
		}

		void testNegative3()
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
			TEST_ENSURE(!intersect(a, triangle, t, u, v));
			TEST_ENSURE(!intersect(b, triangle, t, u, v));
			TEST_ENSURE(!intersect(c, triangle, t, u, v));
			TEST_ENSURE(!intersect(d, triangle, t, u, v));
			TEST_ENSURE(!intersect(e, triangle, t, u, v));
			TEST_ENSURE(!intersect(f, triangle, t, u, v));
			TEST_ENSURE(!intersect(g, triangle, t, u, v));
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("intersect_line_triangle", test);
	}

	CallFunction run(addTest);

}
