// Description: Testing for alignedbox-alignedbox overlap
// DocumentationOf: overlaps_alignedbox_alignedbox.h

#include "test/test_init.h"

#include "pastel/geometry/overlap/overlaps_alignedbox_alignedbox.h"

#include "pastel/sys/rational.h"

#include <iostream>
using std::cout;
using std::endl;

namespace
{

	using Real = Rational<integer>;

}

TEST_CASE("Grazing (overlaps_alignedbox_alignedbox)")
{
	// Nine aligned boxes have been placed
	// in a square grid like this:
	// g h i
	// d e f
	// a b c
	// Their boundaries touch each other.
	// Depending on the topology, there
	// either should or should not be
	// intersections.

	AlignedBox<Real, 2> a(0, 0, 1, 1);
	AlignedBox<Real, 2> b(1, 0, 2, 1);
	AlignedBox<Real, 2> c(2, 0, 3, 1);
	AlignedBox<Real, 2> d(0, 1, 1, 2);
	AlignedBox<Real, 2> e(1, 1, 2, 2);
	AlignedBox<Real, 2> f(2, 1, 3, 2);
	AlignedBox<Real, 2> g(0, 2, 1, 3);
	AlignedBox<Real, 2> h(1, 2, 2, 3);
	AlignedBox<Real, 2> i(2, 2, 3, 3);

	// Since the default topology is
	// min-closed, max-open, there should
	// be no overlaps.

	REQUIRE(!overlaps(e, a));
	REQUIRE(!overlaps(e, b));
	REQUIRE(!overlaps(e, c));
	REQUIRE(!overlaps(e, d));

	REQUIRE(!overlaps(e, f));
	REQUIRE(!overlaps(e, g));
	REQUIRE(!overlaps(e, h));
	REQUIRE(!overlaps(e, i));

	REQUIRE(!overlaps(a, e));
	REQUIRE(!overlaps(b, e));
	REQUIRE(!overlaps(c, e));
	REQUIRE(!overlaps(d, e));

	REQUIRE(!overlaps(f, e));
	REQUIRE(!overlaps(g, e));
	REQUIRE(!overlaps(h, e));
	REQUIRE(!overlaps(i, e));

	e.maxTopology().set(Topology::Closed);

	// Now the e-box should overlap
	// with some of the boxes.

	REQUIRE(!overlaps(e, a));
	REQUIRE(!overlaps(e, b));
	REQUIRE(!overlaps(e, c));
	REQUIRE(!overlaps(e, d));

	REQUIRE(overlaps(e, f));
	REQUIRE(!overlaps(e, g));
	REQUIRE(overlaps(e, h));
	REQUIRE(overlaps(e, i));

	REQUIRE(!overlaps(a, e));
	REQUIRE(!overlaps(b, e));
	REQUIRE(!overlaps(c, e));
	REQUIRE(!overlaps(d, e));

	REQUIRE(overlaps(f, e));
	REQUIRE(!overlaps(g, e));
	REQUIRE(overlaps(h, e));
	REQUIRE(overlaps(i, e));

	e.minTopology().set(Topology::Open);

	REQUIRE(!overlaps(e, a));
	REQUIRE(!overlaps(e, b));
	REQUIRE(!overlaps(e, c));
	REQUIRE(!overlaps(e, d));

	REQUIRE(overlaps(e, f));
	REQUIRE(!overlaps(e, g));
	REQUIRE(overlaps(e, h));
	REQUIRE(overlaps(e, i));

	REQUIRE(!overlaps(a, e));
	REQUIRE(!overlaps(b, e));
	REQUIRE(!overlaps(c, e));
	REQUIRE(!overlaps(d, e));

	REQUIRE(overlaps(f, e));
	REQUIRE(!overlaps(g, e));
	REQUIRE(overlaps(h, e));
	REQUIRE(overlaps(i, e));

	e.maxTopology().set(Topology::Open);

	REQUIRE(!overlaps(e, a));
	REQUIRE(!overlaps(e, b));
	REQUIRE(!overlaps(e, c));
	REQUIRE(!overlaps(e, d));

	REQUIRE(!overlaps(e, f));
	REQUIRE(!overlaps(e, g));
	REQUIRE(!overlaps(e, h));
	REQUIRE(!overlaps(e, i));

	REQUIRE(!overlaps(a, e));
	REQUIRE(!overlaps(b, e));
	REQUIRE(!overlaps(c, e));
	REQUIRE(!overlaps(d, e));

	REQUIRE(!overlaps(f, e));
	REQUIRE(!overlaps(g, e));
	REQUIRE(!overlaps(h, e));
	REQUIRE(!overlaps(i, e));
}

TEST_CASE("Negative (overlaps_alignedbox_alignedbox)")
{
	// Nine non-intersecting aligned boxes have been placed
	// in a square grid like this:
	// g h i
	// d e f
	// a b c
	// Make sure the algorithm agrees that e
	// does not intersect any other aligned box.

	AlignedBox<Real, 2> a(0, 0, 1, 1);
	AlignedBox<Real, 2> b(2, 0, 3, 1);
	AlignedBox<Real, 2> c(4, 0, 5, 1);
	AlignedBox<Real, 2> d(0, 2, 1, 3);
	AlignedBox<Real, 2> e(2, 2, 3, 3);
	AlignedBox<Real, 2> f(4, 2, 5, 3);
	AlignedBox<Real, 2> g(0, 4, 1, 5);
	AlignedBox<Real, 2> h(2, 4, 3, 5);
	AlignedBox<Real, 2> i(4, 4, 5, 5);

	REQUIRE(!overlaps(e, a));
	REQUIRE(!overlaps(e, b));
	REQUIRE(!overlaps(e, c));
	REQUIRE(!overlaps(e, d));

	REQUIRE(!overlaps(e, f));
	REQUIRE(!overlaps(e, g));
	REQUIRE(!overlaps(e, h));
	REQUIRE(!overlaps(e, i));

	REQUIRE(!overlaps(a, e));
	REQUIRE(!overlaps(b, e));
	REQUIRE(!overlaps(c, e));
	REQUIRE(!overlaps(d, e));

	REQUIRE(!overlaps(f, e));
	REQUIRE(!overlaps(g, e));
	REQUIRE(!overlaps(h, e));
	REQUIRE(!overlaps(i, e));
}

TEST_CASE("Positive (overlaps_alignedbox_alignedbox)")
{
	// B fully contained in A.
	{
		AlignedBox<Real, 2> a(
			0, 0, 10, 20);
		AlignedBox<Real, 2> b(
			5, 4, 8, 15);

		REQUIRE(overlaps(a, b));
		REQUIRE(overlaps(b, a));
	}

	// A overlaps B
	{
		AlignedBox<Real, 2> a(
			0, 0, 10, 20);
		AlignedBox<Real, 2> b(
			6, 3, 15, 18);

		REQUIRE(overlaps(a, b));
		REQUIRE(overlaps(b, a));
	}

	// A overlaps B
	{
		AlignedBox<Real, 2> a(
			0, 0, 10, 20);
		AlignedBox<Real, 2> b(
			1, -5, 8, 25);

		REQUIRE(overlaps(a, b));
		REQUIRE(overlaps(b, a));
	}
}

TEST_CASE("Singular (overlaps_alignedbox_alignedbox)")
{
	// Boxes share a corner point.
	{
		AlignedBox<Real, 2> a(0, 0, 1, 1);
		AlignedBox<Real, 2> b(1, 1, 2, 2);

		REQUIRE(!overlaps(a, b));
		REQUIRE(!overlaps(b, a));

		a.maxTopology().set(Topology::Closed);

		REQUIRE(overlaps(a, b));
		REQUIRE(overlaps(b, a));
	}

	// Boxes share an edge
	{
		AlignedBox<Real, 2> a(0, 0, 1, 1);
		AlignedBox<Real, 2> b(1, 0, 2, 1);

		REQUIRE(!overlaps(a, b));
		REQUIRE(!overlaps(b, a));

		a.maxTopology().set(Topology::Closed);

		REQUIRE(overlaps(a, b));
		REQUIRE(overlaps(b, a));
	}
}
