// Description: Testing for points
// DocumentationOf: point_concept.h

#include "test/test_init.h"

#include <pastel/sys/point.h>
#include <pastel/sys/locator.h>
#include <pastel/sys/rational.h>
#include <pastel/sys/vector.h>

TEST_CASE("Concept (point)")
{
	PASTEL_STATIC_ASSERT(Point_HasMemberPointAxis<Point_Archetype>::value);
	PASTEL_STATIC_ASSERT(!Point_HasMemberCall<Point_Archetype>::value);
	PASTEL_STATIC_ASSERT(!Point_HasIndexing<Point_Archetype>::value);
	PASTEL_STATIC_ASSERT(Point_HasMemberDimension<Point_Archetype>::value);

	Point_Archetype point;
	PASTEL_CONCEPT_CHECK(Point_Real<Point_Archetype>, Real_Ring_Concept);

	dimension(addConst(point));
	pointAxis(addConst(point), 2);
	pointLocator(addConst(point));
	PASTEL_CONCEPT_CHECK(Point_Archetype, Point_Concept);

	PASTEL_CONCEPT_REJECT(real*, Point_Concept);
	PASTEL_STATIC_ASSERT((Models<std::array<real, 2>, Point_Concept>::value));
	PASTEL_STATIC_ASSERT((Models<Vector<real, 2>, Point_Concept>::value));

	PASTEL_CONCEPT_CHECK(real, Real_Concept);
	PASTEL_CONCEPT_CHECK(real, Point_Concept);
	PASTEL_CONCEPT_CHECK(integer, Real_Ring_Concept);
	PASTEL_CONCEPT_CHECK(integer, Point_Concept);
	PASTEL_CONCEPT_CHECK(Rational<integer>, Point_Concept);

	struct Something_Else {};
	PASTEL_CONCEPT_REJECT(Something_Else, Point_Concept);

	PASTEL_STATIC_ASSERT((EqualDimension<IntegerConstant<1>, Point_N<Vector<real, 1>>>::value));
	PASTEL_STATIC_ASSERT((EqualDimension<IntegerConstant<1>, Point_N<Vector<real, -1>>>::value));
	PASTEL_STATIC_ASSERT((EqualDimension<IntegerConstant<-1>, Point_N<Vector<real, 1>>>::value));
	PASTEL_STATIC_ASSERT((EqualDimension<IntegerConstant<-1>, Point_N<Vector<real, -1>>>::value));

	PASTEL_STATIC_ASSERT((EqualDimension_C<1, 1>::value));
	PASTEL_STATIC_ASSERT((EqualDimension_C<0, 0>::value));

	PASTEL_STATIC_ASSERT((EqualDimension_C<0, 0>::value));
	PASTEL_STATIC_ASSERT((EqualDimension_C<0, -1>::value));
	PASTEL_STATIC_ASSERT((EqualDimension_C<-1, 0>::value));
	PASTEL_STATIC_ASSERT((EqualDimension_C<-1, -1>::value));

	PASTEL_STATIC_ASSERT((EqualDimension_C<5, 5>::value));
	PASTEL_STATIC_ASSERT((EqualDimension_C<-1, 5>::value));
	PASTEL_STATIC_ASSERT((EqualDimension_C<5, -1>::value));

	PASTEL_STATIC_ASSERT(!(EqualDimension_C<0, 1>::value));
	PASTEL_STATIC_ASSERT(!(EqualDimension_C<1, 0>::value));
	PASTEL_STATIC_ASSERT(!(EqualDimension_C<1, 2>::value));
	PASTEL_STATIC_ASSERT(!(EqualDimension_C<2, 1>::value));
}

TEST_CASE("AsVector (point)")
{
	std::array<real, 2> p = { {1, 2} };
	Vector<real, 2> q = pointAsVector(p);
	REQUIRE((q == Vector<real, 2>(1, 2)));
}

TEST_CASE("Point (point)")
{
	// Default-locator for native arithmetic types.
	PASTEL_STATIC_ASSERT(
		(std::is_same<Point_PointId<integer>, integer>::value));
	PASTEL_STATIC_ASSERT(
		(std::is_same<Point_PointId<real>, real>::value));

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
		(std::is_same<Point_Real<real>, real>::value));

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

		REQUIRE(pointAxis(arrayPoint(x), 0) == x[0]);
		REQUIRE(pointAxis(arrayPoint(x), 1) == x[1]);
	}
	{
		real x = 5;

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
		using Point = real;
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
