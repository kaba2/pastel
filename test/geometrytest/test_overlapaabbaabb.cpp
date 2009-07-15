#include "pastelgeometrytest.h"

#include "pastel/geometry/overlaps_alignedbox_alignedbox.h"

#include "pastel/math/rational.h"
#include "pastel/math/biginteger.h"
#include "pastel/sys/nativeinteger.h"
#include "pastel/math/biginteger_tools.h"
#include "pastel/math/rational_tools.h"

#include <iostream>
using std::cout;
using std::endl;

using namespace Pastel;

namespace
{

	typedef Rational<BigInteger> Real;
	//typedef Rational<NativeInteger<integer> > Real;

	void testNegative()
	{
		// Nine non-intersecting aligned boxes have been placed
		// in a square grid like this:
		// g h i
		// d e f
		// a b c
		// Make sure the algorithm agrees that e
		// does not intersect any other aligned box.

		AlignedBox<2, Real> a(
			Point<2, Real>(0, 0),
			Point<2, Real>(1, 1));

		AlignedBox<2, Real> b(
			Point<2, Real>(2, 0),
			Point<2, Real>(3, 1));

		AlignedBox<2, Real> c(
			Point<2, Real>(4, 0),
			Point<2, Real>(5, 1));

		AlignedBox<2, Real> d(
			Point<2, Real>(0, 2),
			Point<2, Real>(1, 3));

		AlignedBox<2, Real> e(
			Point<2, Real>(2, 2),
			Point<2, Real>(3, 3));

		AlignedBox<2, Real> f(
			Point<2, Real>(4, 2),
			Point<2, Real>(5, 3));

		AlignedBox<2, Real> g(
			Point<2, Real>(0, 4),
			Point<2, Real>(1, 5));

		AlignedBox<2, Real> h(
			Point<2, Real>(2, 4),
			Point<2, Real>(3, 5));

		AlignedBox<2, Real> i(
			Point<2, Real>(4, 4),
			Point<2, Real>(5, 5));

		REPORT(overlaps(e, a));
		REPORT(overlaps(e, b));
		REPORT(overlaps(e, c));
		REPORT(overlaps(e, d));

		REPORT(overlaps(e, f));
		REPORT(overlaps(e, g));
		REPORT(overlaps(e, h));
		REPORT(overlaps(e, i));

		REPORT(overlaps(a, e));
		REPORT(overlaps(b, e));
		REPORT(overlaps(c, e));
		REPORT(overlaps(d, e));

		REPORT(overlaps(f, e));
		REPORT(overlaps(g, e));
		REPORT(overlaps(h, e));
		REPORT(overlaps(i, e));
	}

	void testPositive()
	{
		AlignedBox<2, Real> a(
			Point<2, Real>(0, 0),
			Point<2, Real>(10, 20));

		// AlignedBox fully contained in another.

		AlignedBox<2, Real> b(
			Point<2, Real>(5, 4),
			Point<2, Real>(8, 15));

		REPORT(!overlaps(a, b));
		REPORT(!overlaps(b, a));

		// AlignedBoxs partially contained
		// in another.

		AlignedBox<2, Real> c(
			Point<2, Real>(6, 3),
			Point<2, Real>(15, 18));

		REPORT(!overlaps(a, c));
		REPORT(!overlaps(c, a));

		AlignedBox<2, Real> d(
			Point<2, Real>(1, -5),
			Point<2, Real>(8, 25));
		REPORT(!overlaps(a, d));
		REPORT(!overlaps(d, a));
	}

	void testSingular()
	{
		AlignedBox<2, Real> a(
			Point<2, Real>(0, 0),
			Point<2, Real>(1, 1));

		// AlignedBoxs share a corner point.

		AlignedBox<2, Real> b(
			Point<2, Real>(1, 1),
			Point<2, Real>(2, 2));

		REPORT(!overlaps(a, b));
		REPORT(!overlaps(b, a));

		// AlignedBoxs share an edge

		AlignedBox<2, Real> c(
			Point<2, Real>(1, 0),
			Point<2, Real>(2, 1));

		REPORT(!overlaps(a, c));
		REPORT(!overlaps(c, a));
	}

	void testMoving()
	{
	}

	void testBegin()
	{
		testNegative();
		testPositive();
		testSingular();
		testMoving();
	}

	void testAdd()
	{
		geometryTestList().add("overlapsAlignedboxAlignedbox", testBegin);
	}

	CallFunction run(testAdd);

}
