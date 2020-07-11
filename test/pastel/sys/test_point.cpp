// Description: Testing for points
// DocumentationOf: point_concept.h

#include "test/test_init.h"

#include <pastel/sys/point.h>
#include <pastel/sys/locator.h>
#include <pastel/sys/rational.h>
#include <pastel/sys/vector.h>

#include <string>

TEST_CASE("Concept (point)")
{
	PASTEL_STATIC_ASSERT(Point_HasMemberPointAxis<Point_Archetype>::value);
	PASTEL_STATIC_ASSERT(!Point_HasMemberCall<Point_Archetype>::value);
	PASTEL_STATIC_ASSERT(!Point_HasIndexing<Point_Archetype>::value);
	PASTEL_STATIC_ASSERT(Point_HasMemberDimension<Point_Archetype>::value);

	Point_Archetype point;
	PASTEL_CONCEPT_CHECK(Point_Real<Point_Archetype>, Real_Ring_Concept);
	PASTEL_CONCEPT_CHECK(Point_Archetype, Point_Concept);

	dimension(addConst(point));
	pointAxis(addConst(point), 2);
	pointLocator(addConst(point));

	PASTEL_CONCEPT_REJECT(dreal*, Point_Concept);
	PASTEL_STATIC_ASSERT(Point_Concept<std::array<dreal, 2>>);
	PASTEL_STATIC_ASSERT(Point_Concept<Vector<dreal, 2>>);
	PASTEL_STATIC_ASSERT(Real_Concept<dreal>);
	PASTEL_STATIC_ASSERT(Point_Concept<dreal>);

	PASTEL_CONCEPT_CHECK(dreal, Real_Concept);
	PASTEL_CONCEPT_CHECK(dreal, Point_Concept);
	PASTEL_CONCEPT_CHECK(integer, Real_Ring_Concept);
	PASTEL_CONCEPT_CHECK(integer, Point_Concept);
	PASTEL_CONCEPT_CHECK(Rational<integer>, Point_Concept);

	struct Something_Else {};
	PASTEL_CONCEPT_REJECT(Something_Else, Point_Concept);

	PASTEL_STATIC_ASSERT((IsEqualDim<1, Point_N<Vector<dreal, 1>>::value>));
	PASTEL_STATIC_ASSERT((IsEqualDim<1, Point_N<Vector<dreal, -1>>::value>));
	PASTEL_STATIC_ASSERT((IsEqualDim<-1, Point_N<Vector<dreal, 1>>::value>));
	PASTEL_STATIC_ASSERT((IsEqualDim<-1, Point_N<Vector<dreal, -1>>::value>));

	PASTEL_STATIC_ASSERT((IsEqualDim<1, 1>));
	PASTEL_STATIC_ASSERT((IsEqualDim<0, 0>));

	PASTEL_STATIC_ASSERT((IsEqualDim<0, 0>));
	PASTEL_STATIC_ASSERT((IsEqualDim<0, -1>));
	PASTEL_STATIC_ASSERT((IsEqualDim<-1, 0>));
	PASTEL_STATIC_ASSERT((IsEqualDim<-1, -1>));

	PASTEL_STATIC_ASSERT((IsEqualDim<5, 5>));
	PASTEL_STATIC_ASSERT((IsEqualDim<-1, 5>));
	PASTEL_STATIC_ASSERT((IsEqualDim<5, -1>));

	PASTEL_STATIC_ASSERT(!(IsEqualDim<0, 1>));
	PASTEL_STATIC_ASSERT(!(IsEqualDim<1, 0>));
	PASTEL_STATIC_ASSERT(!(IsEqualDim<1, 2>));
	PASTEL_STATIC_ASSERT(!(IsEqualDim<2, 1>));
}

TEST_CASE("AsVector (point)")
{
	std::array<dreal, 2> p = { {1, 2} };
	Vector<dreal, 2> q = pointAsVector(p);
	REQUIRE((q == Vector<dreal, 2>(1, 2)));
}

TEST_CASE("Point (point)")
{
	// Default-locator for native arithmetic types.
	PASTEL_STATIC_ASSERT(
		(std::is_same<Point_PointId<integer>, integer>::value));
	PASTEL_STATIC_ASSERT(
		(std::is_same<Point_PointId<dreal>, dreal>::value));

	// Default-locator for rational numbers.
	PASTEL_STATIC_ASSERT(
		(std::is_same<Point_PointId<Rational<integer>>, Rational<integer>>::value));
}

TEST_CASE("Real (point)")
{
	// Default-locator for native arithmetic types.
	PASTEL_STATIC_ASSERT(
		(std::is_same<Point_Real<integer>, integer>::value));
	PASTEL_STATIC_ASSERT(
		(std::is_same<Point_Real<dreal>, dreal>::value));

	// Default-locator for rational numbers.
	PASTEL_STATIC_ASSERT(
		(std::is_same<Point_Real<Rational<integer>>, Rational<integer>>::value));

	// Multiple points. Here the type should be promoted
	// to the one which can hold them all.
	PASTEL_STATIC_ASSERT(
		(std::is_same<Point_Real<char, short, long>, long>::value));
}

TEST_CASE("Axis (point)")
{
	{
		std::array<int, 2> x = { {1, 2} };

		REQUIRE(pointAxis(x, 0) == x[0]);
		REQUIRE(pointAxis(x, 1) == x[1]);

		REQUIRE(pointAxis(&x[0], 0) == x[0]);
		REQUIRE(pointAxis(&x[0], 1) == x[1]);

		REQUIRE(pointAxis(x, 0) == x[0]);
		REQUIRE(pointAxis(x, 1) == x[1]);
	}
	{
		dreal x = 5;

		REQUIRE(pointAxis(x, 0) == 5);
		REQUIRE(pointAxis(x, 1) == 5);
	}
	{
		Vector<int, 2> x = { 1, 2 };

		REQUIRE(pointAxis(x, 0) == x[0]);
		REQUIRE(pointAxis(x, 1) == x[1]);
	}
	{
		Vector<int> x(ofDimension(2));

		REQUIRE(pointAxis(x, 0) == x[0]);
		REQUIRE(pointAxis(x, 1) == x[1]);
	}
}

TEST_CASE("Dimension (point)")
{
	{
		using Point = std::array<int, 2>;
		Point x = { {1, 2} };
		REQUIRE(dimension(x) == 2);
		PASTEL_STATIC_ASSERT(Point_N<Point>::value == 2);
	}
	{
		using Point = dreal;
		Point x = 5;
		REQUIRE(dimension(x) == 1);
		PASTEL_STATIC_ASSERT(Point_N<Point>::value == 1);
	}
	{
		using Point = Vector<int, 2>;
		Point x = { 1, 2 };
		REQUIRE(dimension(x) == 2);
		PASTEL_STATIC_ASSERT(Point_N<Point>::value == 2);
	}
	{
		using Point = Vector<int>;
		Point x(ofDimension(2));
		REQUIRE(dimension(x) == 2);
		PASTEL_STATIC_ASSERT(Point_N<Point>::value == Dynamic);
	}
}
