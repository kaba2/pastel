// Description: Testing for distance between points
// DocumentationOf: distance_point_point.h

#include "test/test_init.h"

#include <pastel/sys/point/point_concept.h>
#include <pastel/sys/locator.h>

#include <pastel/geometry/distance/distance_point_point.h>
#include <pastel/math/distance.h>
#include <pastel/sys/vector.h>
#include <pastel/math/norm.h>

namespace
{

	class Custom_Point
	{
	public:
		Custom_Point(dreal x_, dreal y_)
			: x(x_)
			, y(y_)
		{

		}

		dreal x;
		dreal y;
	};

	class Custom_Locator
	{
	public:
		static constexpr integer N = 2;
		using Real = dreal;
		using Point = Custom_Point;

		void swap(Custom_Locator& that)
		{
		}

		const dreal& operator()(const Point& point, integer i) const
		{
			return i != 0 ? point.y : point.x;
		}

		integer n() const
		{
			return N;
		}

		integer n(const Point& point) const
		{
			return N;
		}
	};

	PASTEL_CONCEPT_CHECK(Custom_Locator, Locator_Concept);

	template <typename Point>
	void testCase(
		const Point& a,
		const Point& b)
	{
		using Real = dreal;

		auto keepGoing = [&](const auto& that)
		{
			return ~that < 6;
		};

		{
			Real correct = 6 + 2;

			auto norm = Manhattan_Norm<Real>();

			REQUIRE(~distance2(a, b, PASTEL_TAG(norm), norm) == correct);
			REQUIRE(~distance2(b, a, PASTEL_TAG(norm), norm) == correct);
			REQUIRE(~distance2(a, b, PASTEL_TAG(norm), norm, PASTEL_TAG(keepGoing), keepGoing) >= 6);
		}

		{
			Real correct = std::max(6, 2);
			auto norm = Maximum_Norm<Real>();

			REQUIRE(~distance2(a, b, PASTEL_TAG(norm), norm) == correct);
			REQUIRE(~distance2(b, a, PASTEL_TAG(norm), norm) == correct);
			REQUIRE(~distance2(a, b, PASTEL_TAG(norm), norm, PASTEL_TAG(keepGoing), keepGoing) >= 6);
		}

		{
			Real correct = square(6) + square(2);
			auto norm = Minkowski_Norm<Real>(2);

			REQUIRE(~distance2(a, b, PASTEL_TAG(norm), norm) == correct);
			REQUIRE(~distance2(b, a, PASTEL_TAG(norm), norm) == correct);
			REQUIRE(~distance2(a, b, PASTEL_TAG(norm), norm, PASTEL_TAG(keepGoing), keepGoing) >= 6);
		}

		{
			Real correct = square(6) + square(2);
			auto norm = Euclidean_Norm<Real>();

			REQUIRE(~distance2(a, b, PASTEL_TAG(norm), norm) == correct);
			REQUIRE(~distance2(b, a, PASTEL_TAG(norm), norm) == correct);
			REQUIRE(~distance2(a, b, PASTEL_TAG(norm), norm, PASTEL_TAG(keepGoing), keepGoing) >= square(6));
		}

		{
			Real correct = 6 + 2;
			auto norm = Minkowski_Norm<Real>(1);

			REQUIRE(~distance2(a, b, PASTEL_TAG(norm), norm) == correct);
			REQUIRE(~distance2(b, a, PASTEL_TAG(norm), norm) == correct);
			REQUIRE(~distance2(a, b, PASTEL_TAG(norm), norm, PASTEL_TAG(keepGoing), keepGoing) >= 6);
		}
	}
	
}

TEST_CASE("Custom (distance_point_point)")
{
	Custom_Point a(1, 4);
	Custom_Point b(-5, 2);

	REQUIRE(~distance2(
		location(a, Custom_Locator()), 
		location(b, Custom_Locator()),
		PASTEL_TAG(norm), Manhattan_Norm<dreal>()) == 6 + 2);
}

TEST_CASE("Pointer (distance_point_point)")
{
	using Point = dreal*;

	dreal data[] = { 1, 4, -5, 2 };

	Point a = &data[0];
	Point b = &data[2];

	testCase(pointerPoint(a, 2), pointerPoint(b, 2));
}

TEST_CASE("Array (distance_point_point)")
{
	using Point = std::array<dreal, 2>;

	Point a = { {1, 4} };
	Point b = { {-5, 2} };

	testCase(a, b);
}

TEST_CASE("Vector (distance_point_point)")
{
	using Point = Vector<dreal, 2>;

	Point a(1, 4);
	Point b(-5, 2);

	testCase(a, b);
}

