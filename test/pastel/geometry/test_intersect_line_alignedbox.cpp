// Description: Testing for line-alignedbox intersection
// DocumentationOf: intersect_line_alignedbox.h

#include "test/test_init.h"

#include "pastel/sys/rational.h"
#include "pastel/geometry/intersect/intersect_line_alignedbox.h"

#include <iostream>

namespace
{

	using Real = Rational<integer>;

}

TEST_CASE("Positive (intersect_line_alignedbox)")
{
	// Rays are inside the aligned box.
	// The rays are axis-aligned.
	// The aligned box is centered on origin.
	// Send rays from the origin and
	// make sure they intersect the aligned box
	// as they should.
	{
		AlignedBox<Real, 2> alignedBox(
			Vector<Real, 2>(-1),
			Vector<Real, 2>(1));

		Line<Real, 2> a(
			Vector<Real, 2>(0),
			Vector<Real, 2>(1, 0));

		Line<Real, 2> b(
			Vector<Real, 2>(0),
			Vector<Real, 2>(0, 1));

		Line<Real, 2> c(
			Vector<Real, 2>(0),
			Vector<Real, 2>(-1, 0));

		Line<Real, 2> d(
			Vector<Real, 2>(0),
			Vector<Real, 2>(0, -1));

		Vector<Real, 2> hitList;

		REQUIRE(intersect(a, alignedBox, hitList));
		REQUIRE(hitList[0] == -1);
		REQUIRE(hitList[1] == 1);

		REQUIRE(intersect(b, alignedBox, hitList));
		REQUIRE(hitList[0] == -1);
		REQUIRE(hitList[1] == 1);

		REQUIRE(intersect(c, alignedBox, hitList));
		REQUIRE(hitList[0] == -1);
		REQUIRE(hitList[1] == 1);

		REQUIRE(intersect(d, alignedBox, hitList));
		REQUIRE(hitList[0] == -1);
		REQUIRE(hitList[1] == 1);
	}
	// Rays are inside the aligned box.
	// The rays are diagonal.
	// The aligned box is centered on origin.
	// Send rays from the origin and
	// make sure they intersect the aligned box
	// as they should.
	{
		AlignedBox<Real, 2> alignedBox(
			Vector<Real, 2>(-1),
			Vector<Real, 2>(1));

		Line<Real, 2> a(
			Vector<Real, 2>(0),
			Vector<Real, 2>(Real(1, 1)));

		Line<Real, 2> b(
			Vector<Real, 2>(0),
			Vector<Real, 2>(Real(-1, 1)));

		Line<Real, 2> c(
			Vector<Real, 2>(0),
			Vector<Real, 2>(Real(-1, -1)));

		Line<Real, 2> d(
			Vector<Real, 2>(0),
			Vector<Real, 2>(Real(1, -1)));

		Vector<Real, 2> hitList;

		REQUIRE(intersect(a, alignedBox, hitList));
		REQUIRE(hitList[0] == -1);
		REQUIRE(hitList[1] == 1);

		REQUIRE(intersect(b, alignedBox, hitList));
		REQUIRE(hitList[0] == -1);
		REQUIRE(hitList[1] == 1);

		REQUIRE(intersect(c, alignedBox, hitList));
		REQUIRE(hitList[0] == -1);
		REQUIRE(hitList[1] == 1);

		REQUIRE(intersect(d, alignedBox, hitList));
		REQUIRE(hitList[0] == -1);
		REQUIRE(hitList[1] == 1);
	}

	// The aligned box is centered on origin.
	// The rays originate from the boundary
	// of the aligned box.
	// Make sure the rays
	// intersect the aligned box as they should.
	{
		AlignedBox<Real, 2> alignedBox(
			Vector<Real, 2>(-1),
			Vector<Real, 2>(1));

		Line<Real, 2> a(
			Vector<Real, 2>(
			1, Real(-1, 2)),
			Vector<Real, 2>(
			-2, 1));

		Line<Real, 2> b(
			Vector<Real, 2>(
			-1, Real(1, 2)),
			Vector<Real, 2>(
			2, -1));

		Line<Real, 2> c(
			Vector<Real, 2>(
			Real(-1, 2), -1),
			Vector<Real, 2>(
			Real(-1, 2), 1));

		Vector<Real, 2> hitList;

		REQUIRE(intersect(a, alignedBox, hitList));
		REQUIRE(hitList[0] == 0);
		REQUIRE(hitList[1] == 1);

		REQUIRE(intersect(b, alignedBox, hitList));
		REQUIRE(hitList[0] == 0);
		REQUIRE(hitList[1] == 1);

		REQUIRE(intersect(c, alignedBox, hitList));
		REQUIRE(hitList[0] == 0);
		REQUIRE(hitList[1] == 1);
	}
	// These rays have origin outside the aligned boxes,
	// but still intersect the aligned box.
	{
		AlignedBox<Real, 2> alignedBox(
			Vector<Real, 2>(-1),
			Vector<Real, 2>(1));

		Line<Real, 2> a(
			Vector<Real, 2>(
			0, -2),
			Vector<Real, 2>(
			1, 4));

		Line<Real, 2> b(
			Vector<Real, 2>(
			-2, 2),
			Vector<Real, 2>(
			1, -1));

		Vector<Real, 2> hitList;

		REQUIRE(intersect(a, alignedBox, hitList));
		REQUIRE(hitList[0] == Real(1, 4));
		REQUIRE(hitList[1] == Real(3, 4));

		REQUIRE(intersect(b, alignedBox, hitList));
		REQUIRE(hitList[0] == 1);
		REQUIRE(hitList[1] == 3);
	}
	// These rays just touch the aligned box corners.
	{
		AlignedBox<Real, 2> alignedBox(
			Vector<Real, 2>(-1),
			Vector<Real, 2>(1));

		Line<Real, 2> a(
			Vector<Real, 2>(
			0, 3),
			Vector<Real, 2>(
			-2, -4));

		Line<Real, 2> b(
			Vector<Real, 2>(
			-5, 0),
			Vector<Real, 2>(
			4, 1));

		Line<Real, 2> c(
			Vector<Real, 2>(
			-2, 1),
			Vector<Real, 2>(
			1, 0));

		Line<Real, 2> d(
			Vector<Real, 2>(
			-1, -2),
			Vector<Real, 2>(
			0, 1));

		Vector<Real, 2> hitList;

		REQUIRE(intersect(a, alignedBox, hitList));
		REQUIRE(hitList[0] == Real(1, 2));
		REQUIRE(hitList[1] == Real(1, 2));

		REQUIRE(intersect(b, alignedBox, hitList));
		REQUIRE(hitList[0] == 1);
		REQUIRE(hitList[1] == 1);

		REQUIRE(intersect(c, alignedBox, hitList));
		REQUIRE(hitList[0] == 1);
		REQUIRE(hitList[1] == 3);

		REQUIRE(intersect(d, alignedBox, hitList));
		REQUIRE(hitList[0] == 1);
		REQUIRE(hitList[1] == 3);
	}
}

TEST_CASE("Negative (intersect_line_alignedbox)")
{
	{
		AlignedBox<Real, 2> alignedBox(
			Vector<Real, 2>(-1),
			Vector<Real, 2>(1));

		Line<Real, 2> a(
			Vector<Real, 2>(
			0, -2),
			Vector<Real, 2>(
			2, 1));

		Line<Real, 2> b(
			Vector<Real, 2>(
			-2, 2),
			Vector<Real, 2>(
			1, 0));

		Line<Real, 2> c(
			Vector<Real, 2>(
			-1, -2),
			Vector<Real, 2>(
			-1, 3));

		Line<Real, 2> d(
			Vector<Real, 2>(
			0, -5),
			Vector<Real, 2>(
			2, 6));

		Vector<Real, 2> hitList;

		REQUIRE(!intersect(a, alignedBox, hitList));
		REQUIRE(!intersect(b, alignedBox, hitList));
		REQUIRE(!intersect(c, alignedBox, hitList));
		REQUIRE(!intersect(d, alignedBox, hitList));
	}
}

