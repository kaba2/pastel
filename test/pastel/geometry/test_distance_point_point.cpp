// Description: Testing for distance between points
// DocumentationOf: distance_point_point.h

#include "test/test_init.h"

#include <pastel/sys/point/point_concept.h>
#include <pastel/sys/locator.h>

#include <pastel/geometry/distance/distance_point_point.h>
#include <pastel/math/normbijection.h>

namespace
{

	class Custom_Point
	{
	public:
		Custom_Point(real x_, real y_)
			: x(x_)
			, y(y_)
		{

		}

		real x;
		real y;
	};

	class Custom_Locator
	{
	public:
		static constexpr integer N = 2;
		using Real = real;
		using Point = Custom_Point;

		void swap(Custom_Locator& that)
		{
		}

		const real& operator()(const Point& point, integer i) const
		{
			return i != 0 ? point.y : point.x;
		}

		integer n() const
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
		using Real = real;

		auto keepGoing = [&](const Real& that)
		{
			return that < 6;
		};

		{
			Real correct = 6 + 2;

			REQUIRE(distance2(a, b, Manhattan_NormBijection<Real>()) == correct);
			REQUIRE(distance2(b, a, Manhattan_NormBijection<Real>()) == correct);
			REQUIRE(distance2(a, b, Manhattan_NormBijection<Real>(), keepGoing) >= 6);
		}

		{
			Real correct = std::max(6, 2);

			REQUIRE(distance2(a, b, Maximum_NormBijection<Real>()) == correct);
			REQUIRE(distance2(b, a, Maximum_NormBijection<Real>()) == correct);
			REQUIRE(distance2(a, b, keepGoing, Maximum_NormBijection<Real>()) >= 6);
		}

		{
			Real correct = square(6) + square(2);

			REQUIRE(distance2(a, b, Minkowski_NormBijection<Real>(2)) == correct);
			REQUIRE(distance2(b, a, Minkowski_NormBijection<Real>(2)) == correct);
			REQUIRE(distance2(a, b, Minkowski_NormBijection<Real>(2), keepGoing) >= 6);
		}

		{
			Real correct = square(6) + square(2);

			REQUIRE(distance2(a, b, Euclidean_NormBijection<Real>()) == correct);
			REQUIRE(distance2(b, a, Euclidean_NormBijection<Real>()) == correct);
			REQUIRE(distance2(a, b, Euclidean_NormBijection<Real>(), keepGoing) >= square(6));
		}

		{
			Real correct = 6 + 2;

			REQUIRE(distance2(a, b, Minkowski_NormBijection<Real>(1)) == correct);
			REQUIRE(distance2(b, a, Minkowski_NormBijection<Real>(1)) == correct);
			REQUIRE(distance2(a, b, Minkowski_NormBijection<Real>(1), keepGoing) >= 6);
		}
	}
	
}

TEST_CASE("Custom (distance_point_point)")
{
	Custom_Point a(1, 4);
	Custom_Point b(-5, 2);

	REQUIRE(distance2(
		location(a, Custom_Locator()), 
		location(b, Custom_Locator()),
		Manhattan_NormBijection<real>()) == 6 + 2);
}

TEST_CASE("Pointer (distance_point_point)")
{
	using Point = real*;

	real data[] = { 1, 4, -5, 2 };

	Point a = &data[0];
	Point b = &data[2];

	testCase(pointerPoint(a, 2), pointerPoint(b, 2));
}

TEST_CASE("Array (distance_point_point)")
{
	using Point = std::array<real, 2>;

	Point a = { {1, 4} };
	Point b = { {-5, 2} };

	testCase(arrayPoint(a), arrayPoint(b));
}

TEST_CASE("Vector (distance_point_point)")
{
	using Point = Vector<real, 2>;

	Point a(1, 4);
	Point b(-5, 2);

	testCase(a, b);
}

