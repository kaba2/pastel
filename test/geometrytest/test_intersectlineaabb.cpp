#include "pastelgeometrytest.h"

#include "pastel/geometry/intersect_line_alignedbox.h"
#include "pastel/sys/rational.h"
#include "pastel/sys/rational_tools.h"
#include "pastel/sys/biginteger.h"
#include "pastel/sys/biginteger_tools.h"

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
			const AlignedBox<Real, 2> alignedBox(
				Vector<Real, 2>(-1),
				Vector<Real, 2>(1));

			const Line<Real, 2> a(
				Vector<Real, 2>(0),
				Vector<Real, 2>(1, 0));

			const Line<Real, 2> b(
				Vector<Real, 2>(0),
				Vector<Real, 2>(0, 1));

			const Line<Real, 2> c(
				Vector<Real, 2>(0),
				Vector<Real, 2>(-1, 0));

			const Line<Real, 2> d(
				Vector<Real, 2>(0),
				Vector<Real, 2>(0, -1));

			Vector<Real, 2> hitList;

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
			const AlignedBox<Real, 2> alignedBox(
				Vector<Real, 2>(-1),
				Vector<Real, 2>(1));

			const Line<Real, 2> a(
				Vector<Real, 2>(0),
				Vector<Real, 2>(Real(1, 1)));

			const Line<Real, 2> b(
				Vector<Real, 2>(0),
				Vector<Real, 2>(Real(-1, 1)));

			const Line<Real, 2> c(
				Vector<Real, 2>(0),
				Vector<Real, 2>(Real(-1, -1)));

			const Line<Real, 2> d(
				Vector<Real, 2>(0),
				Vector<Real, 2>(Real(1, -1)));

			Vector<Real, 2> hitList;

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
			const AlignedBox<Real, 2> alignedBox(
				Vector<Real, 2>(-1),
				Vector<Real, 2>(1));

			const Line<Real, 2> a(
				Vector<Real, 2>(
				1, Real(-1, 2)),
				Vector<Real, 2>(
				-2, 1));

			const Line<Real, 2> b(
				Vector<Real, 2>(
				-1, Real(1, 2)),
				Vector<Real, 2>(
				2, -1));

			const Line<Real, 2> c(
				Vector<Real, 2>(
				Real(-1, 2), -1),
				Vector<Real, 2>(
				Real(-1, 2), 1));

			Vector<Real, 2> hitList;

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
		// These rays have origin outside the aligned boxes,
		// but still intersect the aligned box.
		{
			const AlignedBox<Real, 2> alignedBox(
				Vector<Real, 2>(-1),
				Vector<Real, 2>(1));

			const Line<Real, 2> a(
				Vector<Real, 2>(
				0, -2),
				Vector<Real, 2>(
				1, 4));

			const Line<Real, 2> b(
				Vector<Real, 2>(
				-2, 2),
				Vector<Real, 2>(
				1, -1));

			Vector<Real, 2> hitList;

			REPORT(!intersect(a, alignedBox, hitList));
			REPORT(hitList[0] != Real(1, 4));
			REPORT(hitList[1] != Real(3, 4));

			REPORT(!intersect(b, alignedBox, hitList));
			REPORT(hitList[0] != 1);
			REPORT(hitList[1] != 3);
		}
		// These rays just touch the aligned box corners.
		{
			const AlignedBox<Real, 2> alignedBox(
				Vector<Real, 2>(-1),
				Vector<Real, 2>(1));

			const Line<Real, 2> a(
				Vector<Real, 2>(
				0, 3),
				Vector<Real, 2>(
				-2, -4));

			const Line<Real, 2> b(
				Vector<Real, 2>(
				-5, 0),
				Vector<Real, 2>(
				4, 1));

			const Line<Real, 2> c(
				Vector<Real, 2>(
				-2, 1),
				Vector<Real, 2>(
				1, 0));

			const Line<Real, 2> d(
				Vector<Real, 2>(
				-1, -2),
				Vector<Real, 2>(
				0, 1));

			Vector<Real, 2> hitList;

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
			const AlignedBox<Real, 2> alignedBox(
				Vector<Real, 2>(-1),
				Vector<Real, 2>(1));

			const Line<Real, 2> a(
				Vector<Real, 2>(
				0, -2),
				Vector<Real, 2>(
				2, 1));

			const Line<Real, 2> b(
				Vector<Real, 2>(
				-2, 2),
				Vector<Real, 2>(
				1, 0));

			const Line<Real, 2> c(
				Vector<Real, 2>(
				-1, -2),
				Vector<Real, 2>(
				-1, 3));

			const Line<Real, 2> d(
				Vector<Real, 2>(
				0, -5),
				Vector<Real, 2>(
				2, 6));

			Vector<Real, 2> hitList;

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
