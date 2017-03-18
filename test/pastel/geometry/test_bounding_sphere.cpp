// Description: Testing for bounding sphere
// Documentation: unit_testing.txt

#include "test/test_init.h"

#include <pastel/sys/locator.h>
#include <pastel/sys/set.h>

#include <pastel/geometry/bounding/bounding_sphere_pointset.h>

TEST_CASE("boundingSphere (boundingSphere)")
{
	{
		std::array<real, 2> a = { {-1, 0} };
		std::array<real, 2> b = { {1, 0} };
		Sphere<real, 2> bound = boundingSphere(a, b);

		REQUIRE(abs(bound.radius() - 1) < 0.001);
		REQUIRE(norm(bound.position()) < 0.001);
	}
	{
		auto check = [&](
			std::initializer_list<Vector2> pointSet,
			real radius,
			Vector2 position)
		{
			using PointSet = decltype(pointSet);
			PASTEL_CONCEPT_CHECK(PointSet, PointSet_Concept);
			PASTEL_CONCEPT_REJECT(PointSet, Point_Concept);

			integer n = PointSet_Dimension<PointSet>::value;

			Sphere2 bound = boundingSphere(pointSet);

			return abs(bound.radius() - radius) < 0.001 &&
				norm(bound.position() - position) < 0.001;
		};

		REQUIRE(check({ }, 0, { 0, 0 }));
		REQUIRE(check({ {-5, 3} }, 0, { -5, 3 }));
		REQUIRE(check({ {-1, 0}, {1, 0} }, 1, { 0, 0 }));
		REQUIRE(check({ {-1, 0}, {1, 0}, {0, 1}, {0, -1} }, 1, { 0, 0 }));
		REQUIRE(check({ {-1 - 3, 0 + 5}, {1 - 3, 0 + 5}, {0 - 3, 1 + 5}, {0 - 3, -1 + 5} }, 1, { -3, 5 }));
	}
}

