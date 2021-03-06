// Description: Testing for alignedbox-alignedbox containment
// DocumentationOf: contains_alignedbox_alignedbox.h

#include "test/test_init.h"

#include "pastel/geometry/containment/contains_alignedbox_alignedbox.h"

#include "pastel/sys/rational.h"

#include <iostream>
using std::cout;
using std::endl;

namespace
{

	using Real = Rational<integer>;

}

TEST_CASE("Same (contains_alignedbox_alignedbox)")
{
	AlignedBox<Real, 2> aBox(0, 0, 1, 1);
	AlignedBox<Real, 2> bBox(0, 0, 1, 1);

	REQUIRE(contains(aBox, bBox));
	REQUIRE(contains(bBox, aBox));

	aBox.maxTopology().set(Topology::Closed);
	REQUIRE(contains(aBox, bBox));
	REQUIRE(!contains(bBox, aBox));
}

TEST_CASE("Easy (contains_alignedbox_alignedbox)")
{
	AlignedBox<Real, 2> aBox(0, 0, 3, 3);
	AlignedBox<Real, 2> bBox(1, 1, 2, 2);

	REQUIRE(contains(aBox, bBox));
	REQUIRE(!contains(bBox, aBox));
}

TEST_CASE("Empty (contains_alignedbox_alignedbox)")
{
	{
		AlignedBox<Real, 2> aBox(0, 0, 2, 2);
		AlignedBox<Real, 2> bBox(1, 1, 1, 1);

		REQUIRE(contains(aBox, bBox));
		REQUIRE(!contains(bBox, aBox));
	}
	{
		AlignedBox<Real, 2> aBox(0, 0, 2, 2);
		AlignedBox<Real, 2> bBox(-1, -3, -1, -3);

		REQUIRE(contains(aBox, bBox));
		REQUIRE(!contains(bBox, aBox));
	}
	{
		AlignedBox<Real, 2> aBox(0, 0, 3, 3);
		AlignedBox<Real, 2> bBox(1, 1, -1, 2);

		REQUIRE(contains(aBox, bBox));
		REQUIRE(!contains(bBox, aBox));
	}
	{
		AlignedBox<Real, 2> aBox(0, 0, 0, 0);
		AlignedBox<Real, 2> bBox(0, 0, 0, 0);

		REQUIRE(contains(aBox, bBox));
		REQUIRE(contains(bBox, aBox));
	}
	{
		AlignedBox<Real, 2> aBox(0, 0, 0, 0);
		AlignedBox<Real, 2> bBox(1, 1, 1, 1);

		REQUIRE(contains(aBox, bBox));
		REQUIRE(contains(bBox, aBox));
	}
}

TEST_CASE("Partial (contains_alignedbox_alignedbox)")
{
	{
		AlignedBox<Real, 2> aBox(0, 0, 3, 3);
		AlignedBox<Real, 2> bBox(1, 1, 4, 2);

		REQUIRE(!contains(aBox, bBox));
		REQUIRE(!contains(bBox, aBox));
	}
	{
		AlignedBox<Real, 2> aBox(0, 0, 3, 3);
		AlignedBox<Real, 2> bBox(-1, 1, 2, 2);

		REQUIRE(!contains(aBox, bBox));
		REQUIRE(!contains(bBox, aBox));
	}
	{
		AlignedBox<Real, 2> aBox(0, 0, 3, 3);
		AlignedBox<Real, 2> bBox(1, 1, 2, 4);

		REQUIRE(!contains(aBox, bBox));
		REQUIRE(!contains(bBox, aBox));
	}
	{
		AlignedBox<Real, 2> aBox(0, 0, 3, 3);
		AlignedBox<Real, 2> bBox(1, -1, 2, 2);

		REQUIRE(!contains(aBox, bBox));
		REQUIRE(!contains(bBox, aBox));
	}
}
