#include "pastelgeometrytest.h"

#include "pastel/geometry/intersect_line_alignedbox.h"
#include "pastel/math/rational.h"
#include "pastel/math/rational_tools.h"
#include "pastel/math/biginteger.h"
#include "pastel/math/biginteger_tools.h"

#include <iostream>

using namespace Pastel;

namespace
{

	typedef Rational<BigInteger> Real;

	void testPositive()
	{
		// Rays are inside the aligned box.
		// The rays are axis-aligned.
		// The aligned box is centered on origin.
		// Send rays from the origin and
		// make sure they intersect the aligned box
		// as they should.
		{
			const AlignedBox<2, Real> alignedBox(
				Point<2, Real>(-1),
				Point<2, Real>(1));

			const Line<2, Real> a(
				Point<2, Real>(0),
				Vector<2, Real>(1, 0));

			const Line<2, Real> b(
				Point<2, Real>(0),
				Vector<2, Real>(0, 1));

			const Line<2, Real> c(
				Point<2, Real>(0),
				Vector<2, Real>(-1, 0));

			const Line<2, Real> d(
				Point<2, Real>(0),
				Vector<2, Real>(0, -1));

			Vector<2, Real> hitList;

			REPORT(!intersect(a, alignedBox, hitList));
			REPORT(hitList[0] != -1);
			REPORT(hitList[1] != 1);

			REPORT(!intersect(b, alignedBox, hitList));
			REPORT(hitList[0] != -1);
			REPORT(hitList[1] != 1);

			REPORT(!intersect(c, alignedBox, hitList));
			REPORT(hitList[0] != -1);
			REPORT(hitList[1] != 1);

			REPORT(!intersect(d, alignedBox, hitList));
			REPORT(hitList[0] != -1);
			REPORT(hitList[1] != 1);
		}
		// Rays are inside the aligned box.
		// The rays are diagonal.
		// The aligned box is centered on origin.
		// Send rays from the origin and
		// make sure they intersect the aligned box
		// as they should.
		{
			const AlignedBox<2, Real> alignedBox(
				Point<2, Real>(-1),
				Point<2, Real>(1));

			const Line<2, Real> a(
				Point<2, Real>(0),
				Vector<2, Real>(Real(1, 1)));

			const Line<2, Real> b(
				Point<2, Real>(0),
				Vector<2, Real>(Real(-1, 1)));

			const Line<2, Real> c(
				Point<2, Real>(0),
				Vector<2, Real>(Real(-1, -1)));

			const Line<2, Real> d(
				Point<2, Real>(0),
				Vector<2, Real>(Real(1, -1)));

			Vector<2, Real> hitList;

			REPORT(!intersect(a, alignedBox, hitList));
			REPORT(hitList[0] != -1);
			REPORT(hitList[1] != 1);

			REPORT(!intersect(b, alignedBox, hitList));
			REPORT(hitList[0] != -1);
			REPORT(hitList[1] != 1);

			REPORT(!intersect(c, alignedBox, hitList));
			REPORT(hitList[0] != -1);
			REPORT(hitList[1] != 1);

			REPORT(!intersect(d, alignedBox, hitList));
			REPORT(hitList[0] != -1);
			REPORT(hitList[1] != 1);
		}

		// The aligned box is centered on origin.
		// The rays originate from the boundary
		// of the aligned box.
		// Make sure the rays
		// intersect the aligned box as they should.
		{
			const AlignedBox<2, Real> alignedBox(
				Point<2, Real>(-1),
				Point<2, Real>(1));

			const Line<2, Real> a(
				Point<2, Real>(
				1, Real(-1, 2)),
				Vector<2, Real>(
				-2, 1));

			const Line<2, Real> b(
				Point<2, Real>(
				-1, Real(1, 2)),
				Vector<2, Real>(
				2, -1));

			const Line<2, Real> c(
				Point<2, Real>(
				Real(-1, 2), -1),
				Vector<2, Real>(
				Real(-1, 2), 1));

			Vector<2, Real> hitList;

			REPORT(!intersect(a, alignedBox, hitList));
			REPORT(hitList[0] != 0);
			REPORT(hitList[1] != 1);

			REPORT(!intersect(b, alignedBox, hitList));
			REPORT(hitList[0] != 0);
			REPORT(hitList[1] != 1);

			REPORT(!intersect(c, alignedBox, hitList));
			REPORT(hitList[0] != 0);
			REPORT(hitList[1] != 1);
		}
		// These rays have origin outside the alignedBoxs,
		// but still intersect the aligned box.
		{
			const AlignedBox<2, Real> alignedBox(
				Point<2, Real>(-1),
				Point<2, Real>(1));

			const Line<2, Real> a(
				Point<2, Real>(
				0, -2),
				Vector<2, Real>(
				1, 4));

			const Line<2, Real> b(
				Point<2, Real>(
				-2, 2),
				Vector<2, Real>(
				1, -1));

			Vector<2, Real> hitList;

			REPORT(!intersect(a, alignedBox, hitList));
			REPORT(hitList[0] != Real(1, 4));
			REPORT(hitList[1] != Real(3, 4));

			REPORT(!intersect(b, alignedBox, hitList));
			REPORT(hitList[0] != 1);
			REPORT(hitList[1] != 3);
		}
		// These rays just touch the aligned box corners.
		{
			const AlignedBox<2, Real> alignedBox(
				Point<2, Real>(-1),
				Point<2, Real>(1));

			const Line<2, Real> a(
				Point<2, Real>(
				0, 3),
				Vector<2, Real>(
				-2, -4));

			const Line<2, Real> b(
				Point<2, Real>(
				-5, 0),
				Vector<2, Real>(
				4, 1));

			const Line<2, Real> c(
				Point<2, Real>(
				-2, 1),
				Vector<2, Real>(
				1, 0));

			const Line<2, Real> d(
				Point<2, Real>(
				-1, -2),
				Vector<2, Real>(
				0, 1));

			Vector<2, Real> hitList;

			REPORT(!intersect(a, alignedBox, hitList));
			REPORT(hitList[0] != Real(1, 2));
			REPORT(hitList[1] != Real(1, 2));

			REPORT(!intersect(b, alignedBox, hitList));
			REPORT(hitList[0] != 1);
			REPORT(hitList[1] != 1);

			REPORT(!intersect(c, alignedBox, hitList));
			REPORT(hitList[0] != 1);
			REPORT(hitList[1] != 3);

			REPORT(!intersect(d, alignedBox, hitList));
			REPORT(hitList[0] != 1);
			REPORT(hitList[1] != 3);
		}
	}

	void testNegative()
	{
		{
			const AlignedBox<2, Real> alignedBox(
				Point<2, Real>(-1),
				Point<2, Real>(1));

			const Line<2, Real> a(
				Point<2, Real>(
				0, -2),
				Vector<2, Real>(
				2, 1));

			const Line<2, Real> b(
				Point<2, Real>(
				-2, 2),
				Vector<2, Real>(
				1, 0));

			const Line<2, Real> c(
				Point<2, Real>(
				-1, -2),
				Vector<2, Real>(
				-1, 3));

			const Line<2, Real> d(
				Point<2, Real>(
				0, -5),
				Vector<2, Real>(
				2, 6));

			Vector<2, Real> hitList;

			REPORT(intersect(a, alignedBox, hitList));
			REPORT(intersect(b, alignedBox, hitList));
			REPORT(intersect(c, alignedBox, hitList));
			REPORT(intersect(d, alignedBox, hitList));
		}
	}

	void testBegin()
	{
		testPositive();
		testNegative();
	}

	void testAdd()
	{
		geometryTestList().add("IntersectLineAlignedBox", testBegin);
	}

	CallFunction run(testAdd);

}
