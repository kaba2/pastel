// Description: Testing for bounding aligned box
// DocumentationOf: bounding_alignedbox.h

#include "test/test_init.h"

#include <pastel/sys/locator.h>
#include <pastel/sys/vector.h>
#include <pastel/sys/set.h>
#include <pastel/sys/rational.h>

#include <pastel/geometry/bounding/bounding_alignedbox.h>

TEST_CASE("TransformedSphere (boundingAlignedBox)")
{
	using Real = dreal;
	using Point = Vector<Real, 2>;

	{
		Sphere<Real, 2> sphere(Point(0, 0), 4);

		AffineTransformation<Real> transform =
			affineScaling(Point(2, 1));								

		AlignedBox<Real, 2> bound =
			boundingAlignedBox(sphere, transform);

		REQUIRE(bound.min() == Point(0 - 2 * 4, 0 - 1 * 4));
		REQUIRE(bound.max() == Point(0 + 2 * 4, 0 + 1 * 4));
	}
	{
		Sphere<Real, 2> sphere(Point(3, 5), 4);

		AffineTransformation<Real> transform =
			affineScaling(Point(2, 1));								

		AlignedBox<Real, 2> bound =
			boundingAlignedBox(sphere, transform);

		REQUIRE(bound.min() == Point(3 * 2 - 2 * 4, 5 - 1 * 4));
		REQUIRE(bound.max() == Point(3 * 2 + 2 * 4, 5 + 1 * 4));
	}
	{
		Sphere<Real, 2> sphere(Point(3, 5), 4);

		AffineTransformation<Real> transform =
			affineTranslation(Point(1, -4)) * affineScaling(Point(2, 1));

		AlignedBox<Real, 2> bound =
			boundingAlignedBox(sphere, transform);

		REQUIRE(bound.min() == Point((3 * 2 + 1) - 2 * 4, (5 - 4) - 1 * 4));
		REQUIRE(bound.max() == Point((3 * 2 + 1) + 2 * 4, (5 - 4) + 1 * 4));
	}
}

TEST_CASE("Sphere (boundingAlignedBox)")
{
	using Point = Vector2;

	{
		Sphere2 sphere(Point(3, 5), 4);
		AlignedBox2 bound =
			boundingAlignedBox(sphere);

		REQUIRE(bound.min() == Point(3 - 4, 5 - 4));
		REQUIRE(bound.max() == Point(3 + 4, 5 + 4));
	}
}

TEST_CASE("Points (boundingAlignedBox)")
{
	using Point = Vector2i;

	{
		std::vector<Point> pointSet;

		AlignedBox2i bound =
			boundingAlignedBox(pointSet);

		REQUIRE(bound.min() == Point((integer)Infinity()));
		REQUIRE(bound.max() == Point(-(integer)Infinity()));
	}
	{
		std::vector<Point> pointSet =
		{
			{3, 5}
		};

		AlignedBox2i bound =
			boundingAlignedBox(pointSet);

		REQUIRE(bound.min() == Point(3, 5));
		REQUIRE(bound.max() == Point(3, 5));
	}
	{
		// 5 
		// 4  *
		// 3   * *
		// 2  *
		// 1    *
		// 0*
		//  0123456

		std::vector<Point> pointSet =
		{
			{0, 0},
			{2, 2},
			{2, 4},
			{3, 3},
			{4, 1},
			{5, 3}
		};

		AlignedBox2i bound =
			boundingAlignedBox(pointSet);

		REQUIRE(bound.min() == Point(0, 0));
		REQUIRE(bound.max() == Point(5, 4));
	}
}

