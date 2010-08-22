#include "pastelgeometrytest.h"

#include "pastel/geometry/overlaps_alignedbox_alignedbox.h"

#include "pastel/sys/rational.h"
#include "pastel/sys/biginteger.h"
#include "pastel/sys/nativeinteger.h"
#include "pastel/sys/biginteger_tools.h"
#include "pastel/sys/rational_tools.h"

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

		AlignedBox<Real, 2> a(
			Vector<Real, 2>(0, 0),
			Vector<Real, 2>(1, 1));

		AlignedBox<Real, 2> b(
			Vector<Real, 2>(2, 0),
			Vector<Real, 2>(3, 1));

		AlignedBox<Real, 2> c(
			Vector<Real, 2>(4, 0),
			Vector<Real, 2>(5, 1));

		AlignedBox<Real, 2> d(
			Vector<Real, 2>(0, 2),
			Vector<Real, 2>(1, 3));

		AlignedBox<Real, 2> e(
			Vector<Real, 2>(2, 2),
			Vector<Real, 2>(3, 3));

		AlignedBox<Real, 2> f(
			Vector<Real, 2>(4, 2),
			Vector<Real, 2>(5, 3));

		AlignedBox<Real, 2> g(
			Vector<Real, 2>(0, 4),
			Vector<Real, 2>(1, 5));

		AlignedBox<Real, 2> h(
			Vector<Real, 2>(2, 4),
			Vector<Real, 2>(3, 5));

		AlignedBox<Real, 2> i(
			Vector<Real, 2>(4, 4),
			Vector<Real, 2>(5, 5));

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
		AlignedBox<Real, 2> a(
			Vector<Real, 2>(0, 0),
			Vector<Real, 2>(10, 20));

		// AlignedBox fully contained in another.

		AlignedBox<Real, 2> b(
			Vector<Real, 2>(5, 4),
			Vector<Real, 2>(8, 15));

		REPORT(!overlaps(a, b));
		REPORT(!overlaps(b, a));

		// AlignedBoxs partially contained
		// in another.

		AlignedBox<Real, 2> c(
			Vector<Real, 2>(6, 3),
			Vector<Real, 2>(15, 18));

		REPORT(!overlaps(a, c));
		REPORT(!overlaps(c, a));

		AlignedBox<Real, 2> d(
			Vector<Real, 2>(1, -5),
			Vector<Real, 2>(8, 25));
		REPORT(!overlaps(a, d));
		REPORT(!overlaps(d, a));
	}

	void testSingular()
	{
		AlignedBox<Real, 2> a(
			Vector<Real, 2>(0, 0),
			Vector<Real, 2>(1, 1));

		// AlignedBoxs share a corner point.

		AlignedBox<Real, 2> b(
			Vector<Real, 2>(1, 1),
			Vector<Real, 2>(2, 2));

		REPORT(!overlaps(a, b));
		REPORT(!overlaps(b, a));

		// AlignedBoxs share an edge

		AlignedBox<Real, 2> c(
			Vector<Real, 2>(1, 0),
			Vector<Real, 2>(2, 1));

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
