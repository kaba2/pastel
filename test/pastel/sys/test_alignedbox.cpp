// Description: Testing for AlignedBox
// DocumentationOf: alignedbox.h

#include "test/test_init.h"

#include <pastel/sys/alignedbox.h>

using Real_ = real;

TEST_CASE("1d", "[AlignedBox]")
{
	AlignedBox<Real_, 1> a;
	{
		REQUIRE(a.width() == -infinity<Real_>());
	}

	a.set(1, 2);
	{
		REQUIRE(a.min().x() == 1);
		REQUIRE(a.max().x() == 2);
		REQUIRE(a.width() == 1);
	}
}

void test2d()
{
	AlignedBox<Real_, 2> a;
	{
		REQUIRE(a.width() == -infinity<Real_>());
		REQUIRE(a.height() == -infinity<Real_>());
	}

	a.set(1, 2, 3, 5);
	{
		REQUIRE(a.min().x() == 1);
		REQUIRE(a.min().y() == 2);
		REQUIRE(a.max().x() == 3);
		REQUIRE(a.max().y() == 5);
		REQUIRE(a.width() == 2);
		REQUIRE(a.height() == 3);
	}
}

void test3d()
{
	AlignedBox<Real_, 3> a;
	{
		REQUIRE(a.width() == -infinity<Real_>());
		REQUIRE(a.height() == -infinity<Real_>());
		REQUIRE(a.depth() == -infinity<Real_>());
	}

	a.set(1, 2, 3, 5, 7, 9);
	{
		REQUIRE(a.min().x() == 1);
		REQUIRE(a.min().y() == 2);
		REQUIRE(a.min().z() == 3);
		REQUIRE(a.max().x() == 5);
		REQUIRE(a.max().y() == 7);
		REQUIRE(a.max().z() == 9);
		REQUIRE(a.width() == 4);
		REQUIRE(a.height() == 5);
		REQUIRE(a.depth() == 6);
	}
}

void test4d()
{
	AlignedBox<Real_, 4> a;
	{
		REQUIRE(a.width() == -infinity<Real_>());
		REQUIRE(a.height() == -infinity<Real_>());
		REQUIRE(a.depth() == -infinity<Real_>());
	}

	a.set(1, 2, 3, 4, 5, 7, 9, 11);
	{
		REQUIRE(a.min().x() == 1);
		REQUIRE(a.min().y() == 2);
		REQUIRE(a.min().z() == 3);
		REQUIRE(a.min().w() == 4);
		REQUIRE(a.max().x() == 5);
		REQUIRE(a.max().y() == 7);
		REQUIRE(a.max().z() == 9);
		REQUIRE(a.max().w() == 11);
		REQUIRE(a.width() == 4);
		REQUIRE(a.height() == 5);
		REQUIRE(a.depth() == 6);
		REQUIRE(a.extent().w() == 7);
	}
}

