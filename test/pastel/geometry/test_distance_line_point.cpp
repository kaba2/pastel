// Description: Testing template
// Documentation: unit_testing.txt

#include "test/test_init.h"

#include "pastel/geometry/distance/distance_line_point.h"
#include "pastel/sys/rational.h"

TEST_CASE("distance_line_point")
{
	using Real = Rational<integer>;
	using Line = Line<Real, 2>;
	using Point = Vector<Real, 2>;
	using Vector = Vector<Real, 2>;

	{
		auto line = Line(Point(0, 0), Vector(1, 0));
		auto point = Point(2, 0);
		REQUIRE(distance2(line, point) == 0);
	}
	{
		auto line = Line(Point(0, 0), Vector(-1, 0));
		auto point = Point(2, 0);
		REQUIRE(distance2(line, point) == 0);
	}
	{
		auto line = Line(Point(0, 0), Vector(0, 1));
		auto point = Point(2, 0);
		REQUIRE(distance2(line, point) == 2);
	}
	{
		auto line = Line(Point(0, 0), Vector(1, 0));
		auto point = Point(2, 0);
		REQUIRE(distance2(line, point) == 0);
	}
	{
		auto line = Line(Point(0, 0), Vector(1, 1));
		auto point = Point(1, -1);
		REQUIRE(~distance2(line, point) == 1*1 + 1*1);
	}
	{
		auto line = Line(Point(0, 0), Vector(1, 1));
		auto point = Point(-1, 1);
		REQUIRE(~distance2(line, point) == 1*1 + 1*1);
	}
	{
		auto line = Line(Point(0 + 5, 0 + 7), Vector(1, 1));
		auto point = Point(-1 + 5, 1 + 7);
		REQUIRE(~distance2(line, point) == 1*1 + 1*1);
	}
}
